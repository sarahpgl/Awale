#ifndef SERVER2_H
#define SERVER2_H



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

typedef struct {
    int senderSock;
    int receiverSock; 
    int status;
}Challenge; 

#define CRLF        "\r\n"
#define PORT         1977
#define MAX_CLIENTS     20
#define CHALLENGE_NONE -1
#define MAX_NAME_LENGTH 20

#define BUF_SIZE    1024


#include "client2.h"
#include "awale.h"

static void init(void);
static void end(void);
int addClients(const char *name); 
static void app(void);
static int init_connection(void);
static void end_connection(int sock);
static int read_client(SOCKET sock, char *buffer);
static void write_client(SOCKET sock, const char *buffer);
static void send_message_to_all_clients(Client *clients, Client client, int actual, const char *buffer, char from_server);
static void remove_client(Client *clients, int to_remove, int *actual);
static void clear_clients(Client *clients, int actual);
int sendAwaleChallenge(Client *sender, Client *receiver); 
void launchGame(Client * joueur1, Client * joueur2);
Challenge createEmptyChallenge(); 
Client* findClientByName(const char *name, Client *clients, int nbJoueur); 

#endif /* guard */
