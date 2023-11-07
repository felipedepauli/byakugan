#include "WakeUP.hpp"

WakeUP::WakeUP(short port, bool debug_mode) : 
    threadPool(4),
    debug_mode_(debug_mode),
    acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    std::cout << "[WakeUP::Info] Server started on port " << port << std::endl;
    listening();
}

void WakeUP::run() {
    io_context_.run();
}

void WakeUP::listening() {
    std::cout << "[WakeUP::Info] Waiting for new connection" << std::endl;
    acceptor_.async_accept(std::bind(&WakeUP::handleSessionFunction, this, std::placeholders::_1, std::placeholders::_2));
}

void WakeUP::handleSessionFunction(boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
    if (!ec) {
        std::cout << "[WakeUP::Info] New connection accepted" << std::endl;
        auto new_session = std::make_shared<Session>(std::move(socket), debug_mode_);
        threadPool.enqueue([new_session]() {
            try {
                new_session->start();
            } catch (const std::exception& e) {
                std::cerr << "[WakeUP::Info] Client disconnected" << std::endl;
            } catch (...) {
                std::cerr << "[WakeUP::Error] Unknown exception occurred in session" << std::endl;
            }
        });
    } else {
        std::cerr << "[WakeUP::Error] Error accepting new connection: " << ec.message() << std::endl;
    }
    listening();
}
