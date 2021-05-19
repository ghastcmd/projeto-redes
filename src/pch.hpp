#include <iostream>
#include <string>
#include <random>
#include <cstring>
#include <thread>

#include <time.h>

#if defined(__linux__)
#define Linux
#elif defined(_WIN32)
#define Windows
#endif

// sockets related includes
#if defined(Linux)
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>

#elif defined(Windows)
#include <winsock2.h>

#endif