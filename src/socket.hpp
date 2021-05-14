#pragma once



namespace conn
{
#if defined(Windows)
    using socket_t = SOCKET;
#elif defined(Linux)
    using socket_t = int;
#endif

    struct client
    {
        client(const char *ip, unsigned int port);
        ~client();

        // initialize the connection with startup
        void connc() const;

        void sendc(const char *msg);
        /// @return The lenght of the output buffer
        int recc(char *buffer, int lenght);

    private:
        socket_t m_socket;
        struct sockaddr_in m_consocket;
    };

    struct server
    {
        server(unsigned int port);
        ~server();

        void bindc() const;
        void listenc(size_t max) const;

    private:
        socket_t m_socket;
        struct sockaddr_in m_consocket;
    };

}