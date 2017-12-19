#include "Server.h"

using namespace asio;
using namespace JaegerNet;
using namespace std;

using asio::ip::udp;

Server::Server(asio::io_service& service, short port) :
    m_socket(service, udp::endpoint(udp::v4(), port))
{
    StartReceive();
}

Server::~Server()
{
}

void Server::Send(const JaegerNetMessage& message)
{
    std::vector<std::byte> serializedMessage(message.ByteSize());

    if (!message.SerializeToArray(&serializedMessage, static_cast<int>(serializedMessage.size())))
    {
        // TODO: log
        return;
    }

    m_socket.async_send_to(
        asio::buffer(serializedMessage), m_endpoint,
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
            // TODO: log / handle
            return;
        }

        //m_messageHandler.OnMessageReceived(message);

        StartReceive();
    }
}

void Server::OnDataSent(const std::error_code& error, std::size_t bytesReceived)
{
    if (!error || bytesReceived > 0)
    {
        // TODO: Log
    }
    else
    {
        // TODO: Log
    }
}
