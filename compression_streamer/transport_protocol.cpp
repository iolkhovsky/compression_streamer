#include "transport_protocol.h"
#include "paginator.h"

namespace Protocol {


vector<vector<uint8_t>> Manager::make_packets(FrameDesc tx) {
    vector<vector<uint8_t>> out;

    auto begin = tx.payload.begin();
    auto end = begin + tx.payload.size();
    streamer::Paginator<vector<uint8_t>::iterator> paginator({begin, end}, 1280);

    size_t offset = 0;
    size_t packet_id = 0;
    for (auto &page: paginator) {
        size_t video_data_size = page.get_size();
        size_t header_size = sizeof(Header);
        vector<uint8_t> packet(video_data_size + header_size);

        packet[0] = _frame_counter & 0xff;
        packet[1] = (_frame_counter >> 8) & 0xff;
        packet[2] = packet_id & 0xff;
        packet[3] = (packet_id >> 8) & 0xff;
        packet[4] = offset & 0xff;
        packet[5] = (offset >> 8) & 0xff;
        packet[6] = (offset >> 16) & 0xff;
        packet[7] = (offset >> 25) & 0xff;
        packet[8] = tx.img_sz_x & 0xff;
        packet[9] = (tx.img_sz_x >> 8) & 0xff;
        packet[10] = tx.img_sz_y & 0xff;
        packet[11] = (tx.img_sz_y >> 8) & 0xff;
        packet[12] = tx.pixel_size & 0xff;
        packet[13] = (tx.pixel_size >> 8) & 0xff;
        packet[14] = tx.compression;
        packet[15] = tx.compressed_size & 0xff;
        packet[16] = (tx.compressed_size >> 8) & 0xff;
        packet[17] = (tx.compressed_size >> 16) & 0xff;
        packet[18] = (tx.compressed_size >> 25) & 0xff;

        std::copy(page.begin(), page.end(), next(packet.begin(), sizeof(Header)));
        offset += page.get_size();

        out.push_back(move(packet));
    }
    _frame_counter++;
    return out;
}

bool Manager::handle_packet(vector<uint8_t> payload) {
    bool ready_frame = false;
    Header preambule;

    preambule.frame_id = payload[0] + (static_cast<uint16_t>(payload[1]) << 8);
    preambule.packet_id = payload[2] + (static_cast<uint16_t>(payload[3]) << 8);
    preambule.packet_offset = payload[4] + (static_cast<uint16_t>(payload[5]) << 8)
            + (static_cast<uint16_t>(payload[6]) << 16) + (static_cast<uint16_t>(payload[7]) << 24);
    preambule.image_x_size = payload[8] + (static_cast<uint16_t>(payload[9]) << 8);
    preambule.image_y_size = payload[10] + (static_cast<uint16_t>(payload[11]) << 8);
    preambule.pixel_size = payload[12] + (static_cast<uint16_t>(payload[13]) << 8);
    preambule.compression = payload[14];
    preambule.compressed_size = payload[15] + (static_cast<uint16_t>(payload[16]) << 8)
            + (static_cast<uint16_t>(payload[17]) << 16) + (static_cast<uint16_t>(payload[18]) << 24);

    _received_data_volume += payload.size();

    if (preambule.compression) {
        _target_data_size = preambule.compressed_size;

        if (preambule.frame_id != _rx_buffer.frame_id) {
            if (_rx_buffer.payload.size()) {
                _rx_buffer.integrity = static_cast<double>(_received_data_volume) / _target_data_size;
                _target_data_size = 0;
                _received_data_volume = 0;
                _out_buffer = std::move(_rx_buffer);
                ready_frame = true;
            }
        }

        if (_target_data_size != _rx_buffer.payload.size())
            _rx_buffer.payload.resize(_target_data_size);
        if (_target_data_size == 0)
            return false;

        std::copy(next(payload.begin(), sizeof(Header)), payload.end(), next(_rx_buffer.payload.begin(), preambule.packet_offset));
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
                _target_data_size = 0;
                _received_data_volume = 0;
                _out_buffer = std::move(_rx_buffer);
                ready_frame = true;
            }
        }

        if (_target_data_size != _rx_buffer.payload.size())
            _rx_buffer.payload.resize(_target_data_size);
        if (_target_data_size == 0)
            return false;

        std::copy(next(payload.begin(), sizeof(Header)), payload.end(), next(_rx_buffer.payload.begin(), preambule.packet_offset));
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
