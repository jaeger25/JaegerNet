#include "Server.h"
#include "ErrorCodes.h"
#include "MessageHandler.h"

using namespace asio;
using namespace JaegerNet;
using namespace std;

using asio::ip::udp;

Server::Server(asio::io_service& service, short port, std::vector<std::unique_ptr<IMessageHandler>>&& messageHandlers) :
    m_socket(service, udp::endpoint(udp::v4(), port)),
    m_messageHandlers(std::move(messageHandlers))
{
    StartReceive();
}

Server::~Server()
{
}

void Server::Send(const JaegerNetResponse& message)
{
    FAIL_FAST_IF(message.messageid() == 0);

    if (!message.SerializeToArray(&m_sentData, message.ByteSize()))
    {
        // TODO: log
        return;
    }

    m_socket.async_send_to(
        asio::buffer(m_sentData, message.ByteSize()), m_endpoint,
        std::bind(&Server::OnDataSent, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void Server::StartReceive()
{
    m_socket.async_receive_from(
        asio::buffer(m_receievedData), m_endpoint,
        std::bind(&Server::OnDataReceived, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void Server::OnDataReceived(const std::error_code& error, std::size_t bytesReceived)
{
    if (!error || bytesReceived > 0)
    {
        JaegerNetRequest message;
        if (!message.ParseFromArray(m_receievedData.data(), static_cast<int>(bytesReceived)))
        {
            // TODO: log / handle
            return;
        }

        auto args = MessageReceivedEventArgs{ message };
        for (auto&& handler : m_messageHandlers)
        {
            handler->OnMessageReceived(this, args);
        }

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
