#include "transport_protocol.h"
#include "paginator.h"


namespace Protocol {

vector<vector<uint8_t>> Manager::make_packets(FrameDesc tx) {
    vector<vector<uint8_t>> out;
    int header_sz = default_settings::header_size;

    auto begin = tx.payload.begin();
    auto end = begin + tx.payload.size();
    streamer::Paginator<vector<uint8_t>::iterator> paginator({begin, end}, 1280);

    size_t offset = 0;
    size_t packet_id = 0;
    for (auto &page: paginator) {
        size_t video_data_size = page.get_size();
        vector<uint8_t> packet(video_data_size + header_sz);

        packet[0] = _frame_counter & 0xff;
        packet[1] = (_frame_counter >> BitPerByte) & 0xff;
        packet[2] = packet_id & 0xff;
        packet[3] = (packet_id >> BitPerByte) & 0xff;
        packet[4] = offset & 0xff;
        packet[5] = (offset >> BitPerByte) & 0xff;
        packet[6] = (offset >> 2*BitPerByte) & 0xff;
        packet[7] = (offset >> 3*BitPerByte) & 0xff;
        packet[8] = tx.img_sz_x & 0xff;
        packet[9] = (tx.img_sz_x >> BitPerByte) & 0xff;
        packet[10] = tx.img_sz_y & 0xff;
        packet[11] = (tx.img_sz_y >> BitPerByte) & 0xff;
        packet[12] = tx.pixel_size & 0xff;
        packet[13] = (tx.pixel_size >> BitPerByte) & 0xff;
        packet[14] = tx.compression;
        packet[15] = tx.compressed_size & 0xff;
        packet[16] = (tx.compressed_size >> BitPerByte) & 0xff;
        packet[17] = (tx.compressed_size >> 2*BitPerByte) & 0xff;
        packet[18] = (tx.compressed_size >> 3*BitPerByte) & 0xff;

        int available_place = static_cast<int>(packet.size()) - header_sz;
        int page_size = std::distance(page.begin(), page.end());
        if ((available_place <= 0) || (page_size <= 0) || (available_place < page_size))
            throw std::runtime_error("Error in transmitter buffer - transport protocol");
        std::copy(page.begin(), page.end(), next(packet.begin(), header_sz));
        offset += page.get_size();

        out.push_back(move(packet));
    }
    _frame_counter++;
    return out;
}

bool Manager::handle_packet(const vector<uint8_t>& payload, int packet_size) {
    bool ready_frame = false;
    Header preambule;
    int header_sz = default_settings::header_size;
    auto payload_end = std::next(payload.begin(), packet_size);

    preambule.frame_id = payload[0] + (static_cast<uint16_t>(payload[1]) << BitPerByte);
    preambule.packet_id = payload[2] + (static_cast<uint16_t>(payload[3]) << BitPerByte);
    preambule.packet_offset = payload[4] + (static_cast<size_t>(payload[5]) << BitPerByte)
            + (static_cast<size_t>(payload[6]) << 2*BitPerByte) + (static_cast<size_t>(payload[7]) << 3*BitPerByte);
    preambule.image_x_size = payload[8] + (static_cast<uint16_t>(payload[9]) << BitPerByte);
    preambule.image_y_size = payload[10] + (static_cast<uint16_t>(payload[11]) << BitPerByte);
    preambule.pixel_size = payload[12] + (static_cast<uint16_t>(payload[13]) << BitPerByte);
    preambule.compression = payload[14];
    preambule.compressed_size = payload[15] + (static_cast<size_t>(payload[16]) << BitPerByte)
            + (static_cast<size_t>(payload[17]) << 2*BitPerByte) + (static_cast<size_t>(payload[18]) << 3*BitPerByte);

    _received_data_volume += packet_size - header_sz;

    if (preambule.compression) {
        _target_data_size = preambule.compressed_size;

        if (preambule.frame_id != _rx_buffer.frame_id) {
            if (_rx_buffer.payload.size()) {
                _rx_buffer.integrity = static_cast<double>(_received_data_volume) / _target_data_size;
                _received_data_volume = 0;
                _out_buffer = std::move(_rx_buffer);
                ready_frame = true;
            }
        }

        if (_target_data_size != _rx_buffer.payload.size())
            _rx_buffer.payload.resize(_target_data_size);
        if (_target_data_size == 0)
            return false;


        int data_size = packet_size - header_sz;
        int available_size = _rx_buffer.payload.size() - preambule.packet_offset;
        if ((data_size <= 0) || (available_size <= 0) || (available_size < data_size))
            throw std::runtime_error("Error in receiver buffer - transport protocol");
        std::copy(next(payload.begin(), header_sz), payload_end, next(_rx_buffer.payload.begin(), preambule.packet_offset));
        _rx_buffer.img_sz_x = preambule.image_x_size;
        _rx_buffer.img_sz_y = preambule.image_y_size;
        _rx_buffer.frame_id = preambule.frame_id;
        _rx_buffer.pixel_size = preambule.pixel_size;
        _rx_buffer.compression = preambule.compression;
        _rx_buffer.compressed_size = preambule.compressed_size;

        return ready_frame;
    } else {
        _target_data_size = preambule.pixel_size * preambule.image_x_size * preambule.image_y_size;

        if (preambule.frame_id != _rx_buffer.frame_id) {
            if (_rx_buffer.payload.size()) {
                _rx_buffer.integrity = static_cast<double>(_received_data_volume) / _target_data_size;
                _received_data_volume = 0;
                _out_buffer = std::move(_rx_buffer);
                ready_frame = true;
            }
        }

        if (_target_data_size != _rx_buffer.payload.size())
            _rx_buffer.payload.resize(_target_data_size);
        if (_target_data_size == 0)
            return false;

        int available_place = static_cast<int>(_rx_buffer.payload.size()) - preambule.packet_offset;
        int payload_size = packet_size - header_sz;
        if ((available_place <= 0) || (payload_size <= 0) || (available_place < payload_size))
            throw std::runtime_error("Error in receiver buffer - transport protocol");
        std::copy(next(payload.begin(), header_sz), payload_end, next(_rx_buffer.payload.begin(), preambule.packet_offset));
        _rx_buffer.img_sz_x = preambule.image_x_size;
        _rx_buffer.img_sz_y = preambule.image_y_size;
        _rx_buffer.frame_id = preambule.frame_id;
        _rx_buffer.pixel_size = preambule.pixel_size;
        _rx_buffer.compression = preambule.compression;
        _rx_buffer.compressed_size = preambule.compressed_size;

        return ready_frame;
    }
}

FrameDesc Manager::read_data_chunk() {
    return std::move(_out_buffer);
}

}
