#include "transport_protocol.h"
#include "paginator.h"

namespace Protocol {


Manager::Manager() {
    _rx_buffer.payload.resize(1920*1080*4);
    _expected_size = 0;
}

vector<vector<uint8_t>> Manager::make_packets(FrameDesc tx) {
    vector<vector<uint8_t>> out;

    auto begin = tx.payload.begin();
    auto end = begin + tx.payload.size();
    Paginator<vector<uint8_t>::iterator> paginator({begin, end}, 640);

    size_t offset = 0;
    size_t packet_id = 0;
    for (auto &page: paginator) {
        size_t video_data_size = page.get_size();
        size_t header_size = sizeof(Header);
        vector<uint8_t> packet(video_data_size + header_size);
        Header* preambule = reinterpret_cast<Header*>(packet.data());

        preambule->frame_id = _frame_counter;
        preambule->image_x_size = tx.img_sz_x;
        preambule->image_y_size = tx.img_sz_y;
        preambule->packet_id = packet_id++;
        preambule->packet_offset = offset;
        preambule->pixel_size = tx.pixel_size;

        std::copy(page.begin(), page.end(), next(packet.begin(), sizeof(Header)));
        offset += page.get_size();

        out.push_back(move(packet));
    }
    _frame_counter++;
    return out;
}

bool Manager::handle_packet(vector<uint8_t> payload) {
    bool ready_frame = false;
    Header* preambule = reinterpret_cast<Header*>(payload.data());

    if (preambule->frame_id != _rx_buffer.frame_id) {
        if (_expected_size == _rx_buffer.pixel_size * _rx_buffer.img_sz_x * _rx_buffer.img_sz_y) {
            _out_buffer = std::move(_rx_buffer);
            _out_buffer.payload.resize(_expected_size);
            _expected_size = 0;
            _rx_buffer.payload.resize(1920*1080*4);
            ready_frame = true;
        } else {
            _expected_size = 0;
            ready_frame = false;
        }
    }

    std::copy(next(payload.begin(), sizeof(Header)), payload.end(), next(_rx_buffer.payload.begin(), preambule->packet_offset));
    _rx_buffer.img_sz_x = preambule->image_x_size;
    _rx_buffer.img_sz_y = preambule->image_y_size;
    _rx_buffer.frame_id = preambule->frame_id;
    _expected_size += payload.size() - sizeof(Header) + preambule->packet_offset;

    return ready_frame;
}

FrameDesc Manager::read_data_chunk() {
    return _out_buffer;
}


}
