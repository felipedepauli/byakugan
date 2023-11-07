#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include "Session.hpp"
#include "ThreadPool.hpp"

class WakeUP {
public:
    WakeUP(short port, bool debug_mode);
    void run();
    void listening();

private:
    void handleSessionFunction(boost::system::error_code ec, boost::asio::ip::tcp::socket socket);

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    bool debug_mode_;
    ThreadPool threadPool;
};
