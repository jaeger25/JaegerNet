#include "Server.h"

using namespace asio;
using namespace JaegerNet;
using namespace std;

using asio::ip::udp;

Server::Server(MessageHandler messageHandler, io_service& ioService, short port) :
    m_socket(ioService, udp::endpoint(udp::v4(), port)),
    m_messageHandler(std::move(messageHandler))
{
    StartReceive();
}

void Server::Send(std::size_t length)
{
    m_socket.async_send_to(
        asio::buffer(m_data, length), m_endpoint,
        std::bind(&Server::OnDataSent, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void Server::StartReceive()
{
    m_socket.async_receive_from(
        asio::buffer(m_data), m_endpoint,
        std::bind(&Server::OnDataReceived, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void Server::OnDataReceived(const std::error_code& error, std::size_t bytesReceived)
{
    if (!error || bytesReceived > 0)
    {
        JaegerNetMessage message;
        if (!message.ParseFromArray(m_data.data(), static_cast<int>(bytesReceived)))
        {
            // TODO:
        }

        m_messageHandler.OnMessageReceived(message);

        StartReceive();
    }
}

void Server::OnDataSent(const std::error_code& error, std::size_t bytesReceived)
{
    if (!error || bytesReceived > 0)
    {
    }
}
