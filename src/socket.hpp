#pragma once

namespace conn {

#if defined(Windows)
    using socket_t = size_t;
    using socketlen_t = int;
#elif defined(Linux)
    using socket_t = int;
    using socketlen_t = unsigned int;
#endif

    struct basic_socket
    {
        basic_socket();
        ~basic_socket();

        static int get_error();
        static std::string get_error_string(int error_code);

        static void print_error(const char* msg);
        static void print_errorg(const char* msg);
        static void assertf(bool op, const char* msg);
        static void assertg(bool op, const char* msg);
        
        /// @brief Used only on windows, it starts the WSA object
        void wsa_startup() const;

    protected:
        socket_t m_socket;
        struct sockaddr_in m_consocket;
    };

    struct sock
    {
        sock() = default;
        sock(socket_t fd);
        ~sock();

        void assign(sock other) { m_sock_fd = other.m_sock_fd; }

        void send(const char* msg) const;
        int recv(char* buffer, size_t packet_size) const;

        explicit operator bool() const { return m_sock_fd >= 0; };
        bool operator !() const { return !bool(*this); }

        socket_t get_fd() const noexcept { return m_sock_fd; }
        
    private:
        socket_t m_sock_fd;
    };

    struct client : public basic_socket
    {
    public:
        client(const char *ip, unsigned int port);

        /// @brief Initializes the connection with the socket information.
        /// @return If the connection is stablished.
        sock connect() const;
    };

    struct server : public basic_socket
    {
    public:
        server(unsigned int port);
        /// @brief Binds the created socket to the socket protocol in SO kernel
        void bind();
        /// @brief Puts server in passive mode while it waits for client connection.
        /// When the max size is raeched in buffer, the connection is refused.
        void listen(size_t max) const;

        /// @brief Used to accept the connection from client.
        /// Gets the first connection on the buffer to accept it's connection.
        socket_t accept() const;

    private:
        bool m_bount;
    };

}