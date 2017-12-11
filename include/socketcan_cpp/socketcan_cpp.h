#pragma once

#include <string>
#include <socketcan_cpp/socketcan_cpp_export.h>

#ifndef HAVE_SOCKETCAN_HEADERS
#define CAN_MTU 0
#define CANFD_MTU 1
#else
#include <linux/can.h>
#endif

namespace scpp
{

    enum SocketMode
    {
        MODE_CAN_MTU = CAN_MTU,
        MODE_CANFD_MTU = CANFD_MTU
    };

    struct CanFrame
    {
        uint32_t id = 0;
        uint8_t len = 0;
        uint8_t flags = 0;
        uint8_t data[64];

    };

    enum SocketCanStatus
    {
        STATUS_OK = 1 << 0,
        STATUS_SOCKET_CREATE_ERROR = 1 << 2,
        STATUS_INTERFACE_NAME_TO_IDX_ERROR = 1 << 3,
        STATUS_MTU_ERROR = 1 << 4, /// maximum transfer unit
        STATUS_CANFD_NOT_SUPPORTED = 1 << 5, /// Flexible data-rate is not supported on this interface
        STATUS_ENABLE_FD_SUPPORT_ERROR = 1 << 6, /// Error on enabling fexible-data-rate support
        STATUS_WRITE_ERROR = 1 << 7,
        STATUS_READ_ERROR = 1 << 8,
        STATUS_BIND_ERROR = 1 << 9,
    };

    class SocketCan
    {
    public:
        SOCKETCAN_CPP_EXPORT SocketCan();
        SOCKETCAN_CPP_EXPORT SocketCan(const SocketCan &) = delete;
        SOCKETCAN_CPP_EXPORT SocketCan & operator=(const SocketCan &) = delete;
        SOCKETCAN_CPP_EXPORT SocketCanStatus open(const std::string & can_interface, int32_t read_timeout_ms = 3, SocketMode mode = MODE_CAN_MTU);
        SOCKETCAN_CPP_EXPORT SocketCanStatus write(const CanFrame & msg);
        SOCKETCAN_CPP_EXPORT SocketCanStatus read(CanFrame & msg);
        SOCKETCAN_CPP_EXPORT SocketCanStatus close();
        SOCKETCAN_CPP_EXPORT const std::string & interfaceName() const;
        SOCKETCAN_CPP_EXPORT ~SocketCan();
    private:
        int m_socket = -1;
        int32_t m_read_timeout_ms = 3;
        std::string m_interface;
        SocketMode m_socket_mode;
    };
}
