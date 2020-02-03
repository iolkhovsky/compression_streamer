#pragma once

#include <vector>
#include <stddef.h>
#include <stdint.h>

namespace Protocol {

using std::vector;

struct Header {
    uint16_t frame_id;        // Identifier (counter) of current frame
    uint16_t packet_id;       // Identifier (counter) of current packet
    size_t packet_offset;     // Offset of current packet in the frame
    uint16_t image_x_size;
    uint16_t image_y_size;
    uint16_t pixel_size;
};

struct FrameDesc {
    uint16_t frame_id;
    size_t img_sz_x;
    size_t img_sz_y;
    size_t pixel_size;
    vector<uint8_t> payload;

    FrameDesc() = default;
    FrameDesc(FrameDesc&& other) {
        frame_id = other.frame_id;
        img_sz_x = other.img_sz_x;
        img_sz_y = other.img_sz_y;
        payload = move(other.payload);
    }
    FrameDesc(FrameDesc& other) {
        frame_id = other.frame_id;
        img_sz_x = other.img_sz_x;
        img_sz_y = other.img_sz_y;
        payload = other.payload;
    }
    void operator=(FrameDesc&& other) {
        frame_id = other.frame_id;
        img_sz_x = other.img_sz_x;
        img_sz_y = other.img_sz_y;
        payload = move(other.payload);
    }
};

class Manager {
public:
    Manager();
    vector<vector<uint8_t>> make_packets(FrameDesc tx);
    bool handle_packet(vector<uint8_t> payload);
    FrameDesc read_data_chunk();
private:
    uint16_t _frame_counter;
    FrameDesc _rx_buffer;
    FrameDesc _out_buffer;
    size_t _prev_frame_id;
    size_t _expected_size;
};

}



