#include "Server.h"
#include "ErrorCodes.h"
#include <atomic>

using namespace asio;
using namespace JaegerNet;
using namespace std;

using asio::ip::udp;

Server::Server(short port) :
    m_socket(m_service, udp::endpoint(udp::v4(), port))
{
    StartReceive();
}

Server::~Server()
{
    m_service.stop();
    m_serviceThread.join();
}

int32_t Server::RequestReceived(RequestReceivedCallback&& callback)
{
    return m_requestReceivedEventSource.Add(std::move(callback));
}

void Server::RequestReceived(int32_t token)
{
    m_requestReceivedEventSource.Remove(token);
}

void Server::Run(bool runAsync)
{
    if (runAsync)
    {
        m_serviceThread = std::thread([this]
        {
            m_service.run();
        });
    }
    else
    {
        m_service.run();
    }
}

void Server::Send(asio::ip::udp::endpoint& endpoint, JaegerNetBroadcast& message)
{
    static std::atomic_uint64_t NextMessageId = 1;
    message.set_messageid(NextMessageId++);
    message.set_messagetype(JaegerNetMessageType::Broadcast);

    if (!message.SerializeToArray(&m_sentData, message.ByteSize()))
    {
        // TODO: log
        return;
    }

    m_socket.async_send_to(
        asio::buffer(m_sentData, message.ByteSize()), endpoint,
        std::bind(&Server::OnDataSent, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void Server::Send(const JaegerNetResponse& response)
{
    if (!response.SerializeToArray(&m_sentData, response.ByteSize()))
    {
        // TODO: log
        return;
    }

    m_socket.async_send_to(
        asio::buffer(m_sentData, response.ByteSize()), m_endpoint,
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
        JaegerNetRequest request;
        if (!request.ParseFromArray(m_receievedData.data(), static_cast<int>(bytesReceived)))
        {
            // TODO: log / handle
            return;
        }

        JaegerNetResponse response;
        response.set_messageid(request.messageid());

        auto args = RequestReceivedEventArgs{ std::move(m_endpoint), request, response };
        m_requestReceivedEventSource.Invoke(args);

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
