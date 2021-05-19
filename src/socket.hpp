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

        // initialize the connection with startup
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
        server(unsigned int port);
        ~server();

        void bind() const;
        void listen(size_t max) const;

        void send(const char *msg) const;
        /// @return The lenght of the output buffer
        int recv(char *msg, int lenght) const;

    private:
        socket_t m_socket;
        struct sockaddr_in m_consocket;
    };

}