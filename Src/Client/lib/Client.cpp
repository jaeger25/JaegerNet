#include "Client.h"
#include <atomic>
#include <shared_mutex>

using namespace asio;
using namespace google::protobuf;
using namespace JaegerNet;
using namespace std;

using asio::ip::udp;

std::shared_mutex ClientLock;
std::unique_ptr<IClient> ClientInstance;
std::thread ClientServiceThread;

void JaegerNet::CreateClient(const char* const hostname, const char* port, std::vector<std::unique_ptr<IMessageHandler>>&& messageHandlers)
{
    std::lock_guard<std::shared_mutex> lock(ClientLock);

    ClientInstance = std::make_unique<Client>(hostname, port, std::move(messageHandlers));
}

void JaegerNet::DestroyClient(void)
{
    std::lock_guard<std::shared_mutex> lock(ClientLock);

    ClientInstance.reset();
}

IClient* const JaegerNet::GetClient(void) noexcept
{
    std::shared_lock<std::shared_mutex> lock(ClientLock);
    return ClientInstance.get();
}

Client::Client(std::string hostName, std::string port, std::vector<std::unique_ptr<IMessageHandler>>&& messageHandlers) :
    m_service(),
    m_socket(m_service, udp::endpoint(udp::v4(), 0)),
    m_messageHandlers(std::move(messageHandlers))
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

        auto responseCallbackIter = m_responseCallbackMap.find(message.messageid());
        if (responseCallbackIter != m_responseCallbackMap.end())
        {
            responseCallbackIter->second(message);
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
