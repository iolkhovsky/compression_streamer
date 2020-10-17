#pragma once

#include <vector>
#include <stddef.h>
#include <stdint.h>

namespace Protocol {

using std::vector;

namespace default_settings {
    constexpr int header_size = 20;
}

constexpr int BitPerByte = 8;

struct Header {
    uint16_t frame_id;        // Identifier (counter) of current frame
    uint16_t packet_id;       // Identifier (counter) of current packet
    size_t packet_offset;     // Offset of current packet in the frame
    uint16_t image_x_size;
    uint16_t image_y_size;
    uint16_t pixel_size;
    uint8_t compression;
    size_t compressed_size;
};

struct FrameDesc {
    uint16_t frame_id;
    size_t img_sz_x;
    size_t img_sz_y;
    size_t pixel_size;
    uint8_t compression;
    size_t compressed_size;
    vector<uint8_t> payload;
    double integrity;

    FrameDesc() = default;
    FrameDesc(FrameDesc& other) = default;
    FrameDesc(FrameDesc&& other) {
        frame_id = other.frame_id;
        img_sz_x = other.img_sz_x;
        img_sz_y = other.img_sz_y;
        pixel_size = other.pixel_size;
        compression = other.compression;
        compressed_size = other.compressed_size;
        integrity = other.integrity;
        payload = move(other.payload);
    }
    void operator=(FrameDesc&& other) {
        frame_id = other.frame_id;
        img_sz_x = other.img_sz_x;
        img_sz_y = other.img_sz_y;
        pixel_size = other.pixel_size;
        compression = other.compression;
        compressed_size = other.compressed_size;
        integrity = other.integrity;
        payload = move(other.payload);
    }
};

class Manager {
public:
    Manager() = default;
    vector<vector<uint8_t>> make_packets(FrameDesc tx);
    bool handle_packet(const vector<uint8_t>& payload, int packet_size);
    FrameDesc read_data_chunk();
private:
    uint16_t _frame_counter;
    FrameDesc _rx_buffer;
    FrameDesc _out_buffer;
    size_t _prev_frame_id;
    size_t _expected_size;
    int _received_data_volume;
    size_t _target_data_size;
};

}



