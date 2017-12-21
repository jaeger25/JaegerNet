#include "Client.h"

using namespace asio;
using namespace JaegerNet;
using namespace std;

using asio::ip::udp;

Client::Client(asio::io_service& service, std::string hostName, std::string port, std::vector<std::unique_ptr<IMessageHandler>>&& messageHandlers) :
    m_socket(service, udp::endpoint(udp::v4(), 0)),
    m_messageHandlers(std::move(messageHandlers))
{
    udp::resolver resolver(service);
    m_endpoint = *resolver.resolve({ udp::v4(), hostName, port });

    StartReceive();
}

Client::~Client()
{
}

void Client::Send(const JaegerNetRequest& message)
{
    if (!message.SerializeToArray(&m_sentData, message.ByteSize()))
    {
        // TODO: log
        return;
    }

    m_socket.async_send_to(
        asio::buffer(m_sentData, message.ByteSize()), m_endpoint,
        std::bind(&Client::OnDataSent, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void Client::StartReceive()
{
    m_socket.async_receive_from(
        asio::buffer(m_receivedData), m_serverEndpoint,
        std::bind(&Client::OnDataReceived, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void Client::OnDataReceived(const std::error_code& error, std::size_t bytesReceived)
{
    if (!error || bytesReceived > 0)
    {
        JaegerNetResponse message;
        if (!message.ParseFromArray(m_receivedData.data(), static_cast<int>(bytesReceived)))
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

void Client::OnDataSent(const std::error_code& error, std::size_t bytesReceived)
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
