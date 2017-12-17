#include "Server.h"

using namespace asio;
using namespace JaegerNet;
using namespace std;

using asio::ip::udp;

Server::Server(io_service& ioService, short port)
    : m_socket(ioService, udp::endpoint(udp::v4(), port))
{
    do_receive();
}

void Server::do_receive()
{
    m_socket.async_receive_from(
        asio::buffer(m_data), m_endpoint,
        [this](std::error_code errorCode, std::size_t bytesReceived)
    {
        if (!errorCode && bytesReceived > 0)
        {
            Send(bytesReceived);
        }
        else
        {
            do_receive();
        }
    });
}

void Server::Send(std::size_t length)
{
    m_socket.async_send_to(
        asio::buffer(m_data, length), m_endpoint,
        [this](std::error_code /*ec*/, std::size_t /*bytes_sent*/)
    {
        do_receive();
    });
}
