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

        int get_error() const;
        void print_error(const char* msg) const;
        void print_errorg(const char* msg) const;
        void send(const char* msg) const;
        
        /// @brief Used only on windows, it starts the WSA object
        void wsa_startup() const;

    protected:
        socket_t m_socket;
        struct sockaddr_in m_consocket;
    private:
        bool m_print_error_str = false;
    };

    struct client : public basic_socket
    {
    public:
        client(const char *ip, unsigned int port);

        /// @brief Initializes the connection with the socket information.
        /// @return If the connection is stablished.
        bool connect() const;

        /// @return The lenght of the output buffer
        int recv(char *buffer, int lenght);
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

        /// @brief Use the socket returned from accept() as file descriptor
        /// @return The lenght of the output buffer
        int recv(socket_t fd, char *buffer, int buffer_size) const;

    private:
        bool m_bount;
    };

}