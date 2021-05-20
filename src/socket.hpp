#pragma once

namespace conn {

#if defined(Windows)
    using socket_t = size_t;
#elif defined(Linux)
    using socket_t = int;
#endif

    struct basic_socket
    {
        int get_error() const;
        void send(const char* msg) const;
    protected:
        socket_t m_socket;
        struct sockaddr_in m_consocket;
    };

    struct client : public basic_socket
    {
    public:
        client(const char *ip, unsigned int port);
        ~client();

        /// @brief Initializes the connection with the socket information
        bool connect() const;

        /// @return The lenght of the output buffer
        int recv(char *buffer, int lenght);
    };

    struct server : public basic_socket
    {
    public:
        server(unsigned int port);
        ~server();
        /// @brief Binds the made socket to the socket protocol in SO
        void bind();
        /// @brief Puts server in passive mode while it waits for client connection.
        /// When the max size is raeched in buffer, the connection is refused.
        void listen(size_t max) const;

        /// @brief Used to accept the connectin from client
        socket_t accept() const;

        /// @brief Use the socket returned from accept() as file descriptor
        /// @return The lenght of the output buffer
        int recv(socket_t fd, char *buffer, int buffer_size) const;

    private:
        bool m_bount;
    };

}