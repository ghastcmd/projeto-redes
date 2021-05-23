#include <iostream>
#include <string>
#include <random>
#include <cstring>
#include <functional>
#include <thread>
#include <memory>

#include <time.h>

#if defined(__linux__)
#define Linux true
#elif defined(_WIN32)
#define Windows true
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