#pragma once
#include <exception>

namespace JaegerNet
{
    enum class JaegerNetError : int32_t
    {
        Success,
        MaxLobbiesExceeded,
        LobbyNotFound,
        LobbyCapacityExceeded,
        ControllerNotFound,
        PlayerNotFound,
    };

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
}

#define FAIL_FAST() \
{ \
    __debugbreak(); \
    std::terminate(); \
} \


#define JAEGERNET_CATCH_RETURN() \
    catch(JaegerNet::JaegerNetException& ex) \
    { \
        return ex.Error(); \
    } \
    catch (std::exception& /*ex*/) \
    { \
        FAIL_FAST() \
    } \

#define JAEGERNET_CATCH_LOG() \
    catch(JaegerNet::JaegerNetException& /*ex*/) \
    { \
    } \
    catch (std::exception& /*ex*/) \
    { \
        FAIL_FAST() \
    } \

#define FAIL_FAST_IF(conditional) \
{ \
    bool result = conditional; \
    if(result) \
    { \
        FAIL_FAST() \
    } \
} \
