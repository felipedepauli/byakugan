#include "Session.hpp"
#include <iostream>
#include <boost/endian/conversion.hpp>
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/bind.hpp>
#include <chrono>
#include "SightGuard.hpp"

constexpr uint64_t expected_header = 0x45594553424752; // "EYESBGR"

Session::Session(boost::asio::ip::tcp::socket socket, bool debug)
: socket_(std::move(socket))
{
    evilEys_ = SightGuard::getInstance()->getResource(SightGuardResource::EYS);
    std::cout << "[Session::Info] Session created." << std::endl;

}

Session::~Session() {
    std::cout << "[Session::Info] Session finished." << std::endl;
}

void Session::start() {
    process();
}

int Session::process() {
    std::cout << "[Session::Info] Processing..." << std::endl;
    // Step 01. Open Sentinel's Eyes (camera)
    if (evilEys_->openEyes() != 0) {
        return -1;
    }
    while (true) {
        // Step 03. Get the frame from Sentinel's Eye
        cv::Mat frame = evilEys_->spark();

        if (frame.empty()) {
            std::cerr << "[Session::Info] Frame is empty!" << std::endl;
            break;
        }

        // Get start time
        // auto start = std::chrono::high_resolution_clock::now();

        // Step 04. Send the frame to client
        send_frame(frame);

        // Get end time
        // auto finish = std::chrono::high_resolution_clock::now();

        // Calculate duration
        // std::chrono::duration<double> elapsed = finish - start;
        // std::cout << "[Session::Info] Frame sent in " << elapsed.count() * 1000 << " milliseconds." << std::endl;
    }
    return 0;
}

void Session::send_frame(const cv::Mat& frame) {
    // Whenever a connection is established, we need to send a header to the client
    // indicating that the communication is starting. This is done so that the client knows
    // that the connection is established and it can start receiving data.
    // The header is a fixed value known by both sides of the connection.
    // To facilitate implementation, we'll use the fixed and sequential 64-bit value below:
    uint64_t header = expected_header;
    uint64_t header_network = htobe64(header);
    boost::asio::write(socket_, boost::asio::buffer(&header_network, sizeof(header_network)));

    // Whenever we send data to the client, we need to do a few things:
    // 1. Have a suitable buffer to store the data to be sent
    // 2. Convert the value to network byte order
    // 3. Send the buffer size to the client before sending the data
    // 4. Send the data
    // Let's follow these steps

    // Step 1. Have a suitable buffer to store the data to be sent
    // The buffer vector is the frame, but converted to jpg
    std::vector<uchar> buffer;
    cv::imencode(".jpg", frame, buffer);

    // Step 2. Convert the value to network byte order
    // This is necessary because the network byte order is different from the byte order of the computer.
    // Failing to convert the value to network byte order can cause communication errors.
    std::size_t bufferSize = buffer.size();
    uint64_t bufferSize_network = htobe64(bufferSize);

    // Step 3. Send the buffer size to the client before sending the data
    // This is necessary because the client needs to know how many bytes it should expect to receive
    // before attempting to read the data. In other words, it knows when the communication starts and when it ends.
    boost::asio::write(socket_, boost::asio::buffer(&bufferSize_network, sizeof(bufferSize_network)));

    // Step 4. Send the image data
    boost::asio::write(socket_, boost::asio::buffer(buffer.data(), bufferSize));
}
