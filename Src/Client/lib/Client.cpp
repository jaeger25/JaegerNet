#include "Client.h"
#include <atomic>
#include <shared_mutex>

using namespace asio;
using namespace google::protobuf;
using namespace JaegerNet;

using asio::ip::udp;

Client::Client(std::string hostName, std::string port) :
    m_service(),
    m_socket(m_service, udp::endpoint(udp::v4(), 0))
{
    udp::resolver resolver(m_service);
    m_endpoint = *resolver.resolve({ udp::v4(), hostName, port });

    StartReceive();
}

Client::~Client()
{
    m_service.stop();
    m_serviceThread.join();
}

void Client::Send(JaegerNetRequest& message, ResponseReceivedCallback&& callback)
{
    static std::atomic_uint64_t NextMessageId = 1;
    message.set_messageid(NextMessageId++);
    message.set_messagetype(JaegerNetMessageType::Request);

    if (!message.SerializeToArray(&m_sentData, message.ByteSize()))
    {
        // TODO: log
        return;
    }

    m_responseCallbackMap.emplace(message.messageid(), std::move(callback));

    m_socket.async_send_to(
        asio::buffer(m_sentData, message.ByteSize()), m_endpoint,
        std::bind(&Client::OnDataSent, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void Client::Run(bool runAsync)
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

int32_t Client::BroadcastReceived(BroadcastReceivedCallback&& callback)
{
    return m_broadcastReceivedEventSource.Add(std::move(callback));
}

void Client::BroadcastReceived(int32_t token)
{
    m_broadcastReceivedEventSource.Remove(token);
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
        if (JaegerNetResponse message; message.ParseFromArray(m_receivedData.data(), static_cast<int>(bytesReceived)) && message.messagetype() == JaegerNetMessageType::Response)
        {
            auto responseCallbackIter = m_responseCallbackMap.find(message.messageid());
            if (responseCallbackIter != m_responseCallbackMap.end())
            {
                responseCallbackIter->second(message);
                m_responseCallbackMap.erase(responseCallbackIter);
            }
        }
        else if (JaegerNetBroadcast broadcast; broadcast.ParseFromArray(m_receivedData.data(), static_cast<int>(bytesReceived)) && message.messagetype() == JaegerNetMessageType::Broadcast)
        {
            auto args = BroadcastReceivedEventArgs{ broadcast };
            m_broadcastReceivedEventSource.Invoke(args);
        }
        else
        {
            // TODO: Log
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
