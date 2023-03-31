#ifndef IBN_NET_SOCKET_H
#define IBN_NET_SOCKET_H

#include "base/common/noncopyable.h"

// struct tcp_info is in <netinfo/tcp.h>
struct tcp_info;

namespace ibn {

namespace net {

class InetAddress;

// Wrapper of socket file descriptor.
// It closes the sockfd when desctructs.
// It's thread safe, all operations are delagated to OS.
class Socket : noncopyable {
public:
    explicit Socket(int sockfd) : sockfd_(sockfd) {}

    ~Socket();

    int fd() const { return sockfd_; }
    bool GetTcpInfo(struct tcp_info*) const;
    bool GetTcpInfoString(char* buf, int len) const;

    void BindAddress(const InetAddress& localaddr);
    void Listen();
    // On success, returns a non-negative integer that is
    // a descriptor for the accepted socket, which has been
    // set to non-blocking and close-on-exec.
    // On error, -1 is returned, and *peeraddr is untouched.
    int Accept(InetAddress* peeraddr);
    
    void ShutdownWrite();

    // Enable/disable TCP_NODELAY(disable/enable Nagle's algorithm).
    void SetTcpNoDelay(bool on);

    // Enable/disable SO_REFUSEADDR
    void SetReuseAddr(bool on);

    // Enable/disable SO_REUSEPORT
    void SetReusePort(bool on);

    // Enable/disable SO_KEEPALIVE
    void SetKeepAlive(bool on);

private:
    const int sockfd_;
};

} // !namespace net

} // !namespace ibn

#endif // !IBN_NET_SOCKET_H
