#pragma once

namespace conn {

#if defined(Windows)
    using socket_t = size_t;
#elif defined(Linux)
    using socket_t = int;
#endif

    struct client
    {
        client(const char *ip, unsigned int port);
        ~client();

        /// @brief Initializes the connection with the socket information
        bool connect() const;

        void send(const char *msg);
        /// @return The lenght of the output buffer
        int recv(char *buffer, int lenght);

    private:
        socket_t m_socket;
        struct sockaddr_in m_consocket;
    };

    struct server
    {
    private:
        int get_error() const;
    public:
        server(unsigned int port);
        ~server();

        void bind();
        /// @brief Puts server in passive mode while it waits for client connection.
        /// When the max size is raeched in buffer, the connection is refused.
        void listen(size_t max) const;

        socket_t accept() const;

        void send(const char *msg) const;
        /// @return The lenght of the output buffer
        int recv(char *msg, int lenght) const;

        int recv(socket_t fd, char *buffer, int buffer_size) const;

    private:
        socket_t m_socket;
        struct sockaddr_in m_consocket;
        bool m_bount;
    };

}