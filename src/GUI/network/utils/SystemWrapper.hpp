/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Wrapper pour les fonctions système
*/

#ifndef SYSTEMWRAPPER_HPP_
#define SYSTEMWRAPPER_HPP_

#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <memory>
#include <vector>
#include <cerrno>
#include <cstring>
#include <functional>

namespace SystemWrapper {

// =====================
// SafeSockAddr
// =====================
class SafeSockAddr {
public:
    SafeSockAddr() = default;
    explicit SafeSockAddr(const sockaddr& addr);
    explicit SafeSockAddr(const sockaddr_in& addr);

    sockaddr_in& getAddr();
    const sockaddr_in& getAddr() const;
    sockaddr* getSockAddr();
    const sockaddr* getSockAddr() const;
    socklen_t getSize() const;

private:
    sockaddr_in _addr{};
};

// =====================
// SafePollFd
// =====================
class SafePollFd {
public:
    SafePollFd() = default;
    explicit SafePollFd(int fd, int16_t events = POLLIN);

    pollfd& getPollFd();
    const pollfd& getPollFd() const;
    pollfd* getPollPtr();
    const pollfd* getPollPtr() const;

    int getFd() const;
    int16_t getEvents() const;
    int16_t getRevents() const;

    void setFd(int fd);
    void setEvents(int16_t events);
    void setRevents(int16_t revents);

private:
    pollfd _pollfd{};
};

// =====================
// SafeBuffer
// =====================
class SafeBuffer {
public:
    explicit SafeBuffer(size_t size);
    ~SafeBuffer() = default;

    std::string& data();
    const std::string& data() const;
    size_t size() const;
    void resize(size_t newSize);
    std::string toString() const;

private:
    std::string _buffer;
};

// =====================
// Fonctions utilitaires
// =====================

// Sockets
int createSocket(int domain, int type, int protocol);
int connectSocket(int sockfd, const SafeSockAddr& addr);
int closeSocket(int fd);
int setNonBlocking(int fd);

// Lecture/écriture
ssize_t readSocket(int fd, std::shared_ptr<SafeBuffer> buffer, size_t count);
ssize_t writeSocket(int fd, const SafeBuffer& buffer, size_t count);

// Poll
int pollSocket(const SafePollFd& pfd, int timeout);
int pollSocket(const std::vector<SafePollFd>& pollfds, int timeout);

// Options de socket (template)
template<typename T>
int getSocketOption(int sockfd, int level, int optname, T* optval) {
    socklen_t optlen = sizeof(T);
    return getsockopt(sockfd, level, optname, optval, &optlen);
}

// Conversion IP
bool inetPton(int af, const std::string& src, void* dst);
std::string inetNtop(int af, const void* src);

// Gestion des erreurs
std::string getErrorString();

// Allocation de tableaux dynamiques
template<typename T>
std::unique_ptr<T[]> makeUniqueArray(size_t size) {
    return std::make_unique<T[]>(size);
}

template<typename T>
std::shared_ptr<T[]> makeSharedArray(size_t size) {
    return std::shared_ptr<T[]>(new T[size], std::default_delete<T[]>());
}

// Exécution avec timeout
void executeWithTimeout(const std::function<bool()>& action, int timeoutMs);

} // namespace SystemWrapper

#endif /* !SYSTEMWRAPPER_HPP_ */
