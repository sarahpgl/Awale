#ifndef CLIENT2_H
#define CLIENT2_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#define CRLF     "\r\n"
#define PORT     1977

#define BUF_SIZE 1024
#define MAX_NAME_LENGTH 20

static void init(void);
static void end(void);
static void send_username(SOCKET sock, const char *name);
static void app(const char *address);
static int init_connection(const char *address);
static void end_connection(int sock);
static int read_server(SOCKET sock, char *buffer);
static void write_server(SOCKET sock, const char *buffer);


#endif /* guard */
