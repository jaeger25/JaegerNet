#pragma once

enum class JaegerNetError : int32_t
{
    Ok,
    MaxLobbiesExceeded,
    LobbyNotFound,
    LobbyCapacityExceeded,
};

constexpr google::protobuf::int32 JaegerErrorToProtobuf(JaegerNetError error)
{
    return static_cast<google::protobuf::int32>(error);
}

class JaegerNetException : public std::exception
{
public:
    JaegerNetException(JaegerNetError error) :
        m_error(error)
    {
    }

    JaegerNetError Error() const
    {
        return m_error;
    }

private:
    JaegerNetError m_error;
};
