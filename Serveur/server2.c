#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>
#include "server2.h"
#include "client2.h"
#include "awale.h"

char clientNames[MAX_CLIENTS][MAX_NAME_LENGTH];
int nbClients = 0;
Client  clients[MAX_CLIENTS];


static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}




int addClients(const char *name){
   if(nbClients < MAX_CLIENTS){
      strncpy(clientNames[nbClients], name, MAX_NAME_LENGTH -1);
      clientNames[nbClients][MAX_NAME_LENGTH - 1] = '\0';
      nbClients ++; 

      FILE * f;
       char ligne[256];
       int trouve =0;

      f = fopen("statutsJoueurs.txt", "a+");
    if (f != NULL)
    {
      while(fgets(ligne,sizeof(name), f) != NULL){
         
         
        if(strstr(ligne, name)){
            trouve =1;
         }
        
         
      }
      if(trouve==0){
         fprintf(f, "%s %d\n", name, 1);
      }
      
      fclose(f);
    }
    else
        perror("statusJoueurs.txt");
      return 0; //Succès de la connexion avec le pseudo 
   }else{
      return -1; //Echec de la connexion au serveur avec le speudo
   }
}

Challenge createEmptyChallenge(){
   Challenge emptyChallenge; 
   emptyChallenge.senderSock = 0; 
   emptyChallenge.receiverSock = 0; 
   emptyChallenge.status = CHALLENGE_NONE; 

   return emptyChallenge; 
}

Client* findClientByName(const char *name, Client *clients, int nbJoueur){
   int j; 
   for(j = 0; j < nbJoueur; j++){
      if(strcmp(clients[j].name, name) == 0){
         return &clients[j]; 
      }
   }
   return NULL; 
}

static void app(void)
{
   SOCKET sock = init_connection();
   char buffer[BUF_SIZE];
   /* the index for the array */
   int actual = 0;
   int max = sock;
   /* an array for all clients */
   Client clients[MAX_CLIENTS];

   fd_set rdfs;

   while(1)
   {
      int i = 0;
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the connection socket */
      FD_SET(sock, &rdfs);

      /* add socket of each client */
      for(i = 0; i < actual; i++)
      {
         FD_SET(clients[i].sock, &rdfs);
         
         
      }

      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         /* stop process when type on keyboard */
         break;
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         /* new client */
         SOCKADDR_IN csin = { 0 };
         //size_t sinsize = sizeof csin;
         //int size = sizeof(sinsize);

         SOCKADDR_IN csin1 = { 0 };
         socklen_t sinsize = sizeof csin1;
         int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
         if(csock == SOCKET_ERROR)
         {
            perror("accept()");
            continue;
         }


         /* after connecting the client sends its name */
         if(read_client(csock, buffer) == -1)
         {
            /* disconnected */
            continue;
         }

         if(addClients(buffer) == 0){
            /* what is the new maximum fd ? */
            max = csock > max ? csock : max;

            FD_SET(csock, &rdfs);
            Challenge challengeVide = createEmptyChallenge(); 
            Client c = { csock, {0}, challengeVide };
            strncpy(c.name, buffer, BUF_SIZE - 1);
            clients[actual] = c;
            actual++;
            
            char* s = "Bienvenue dans l'application, choisissez une option\n 1 : Afficher les pseudos des autres joueurs connectés\n 2 : Discuter avec les autres joueurs\n 3 : Défiez l'adversaire de votre choix\n";
            write_client(c.sock, s);
         }else{
            char* s = "Connexion au serveur échoué en raison de trop forte affluence sur le serveur.\n";
            write_client(csock, s);
            closesocket(csock); 
         }
               
      }
      else
      {
         int i;
         for(i = 0; i < actual; i++)
         {
            /* a client is talking */
            if(FD_ISSET(clients[i].sock, &rdfs))
            {
               Client client = clients[i];
               int c = read_client(clients[i].sock, buffer);
               
               /* client disconnected */
               if(c == 0)
               {
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " Disconnected !", BUF_SIZE - strlen(buffer) - 1);
                  send_message_to_all_clients(clients, client, actual, buffer, 1);
                  FILE * f;
                  
                  

                  f = fopen("statutsJoueurs.txt", "a+");
                  if (f != NULL)
                  {
                     char ligne[256];  // Taille maximale d'une ligne
                     long positionDebut = -1;

                     // Trouve la position du début de la ligne à remplacer
                     while (fgets(ligne, sizeof(ligne), f) != NULL) {
                        if (strstr(ligne, clients[i].name) != NULL) {
                           positionDebut = ftell(f) - strlen(ligne);
                           break;
                        }
                     }

                     if (positionDebut != -1) {
                      // Positionne le curseur à la position du début de la ligne
                        fseek(f, positionDebut, SEEK_SET);

                        // Supprime la ligne
                        ftruncate(fileno(f), positionDebut);
                        
                        // Repositionne le curseur à la fin du fichier
                        fseek(f, 0, SEEK_END);

                        // Ajoute la nouvelle ligne à la fin du fichier
                        char * s =strcat(clients[i].name," 0");
                        fputs(s, f);

                     // Tronque le fichier à la nouvelle position si la nouvelle ligne est plus courte que l'ancienne
                     
                     
                     fclose(f);
                  }
                  else
                     perror("statusJoueurs.txt");

               }
               else{
                  
                  printf("%s", buffer);
                     
                        
                  // Handle client input
                  if (strcmp(buffer, "1") == 0){
                     char clientList[MAX_CLIENTS * (MAX_NAME_LENGTH + 2)];
                     clientList[0] = '\0';
                     int k; 
                     for(k = 0; k < nbClients; k++){
                        strcat(clientList, clientNames[k]);
                        strcat(clientList, "\n");
                     }
                     write_client(clients[i].sock, clientList);
                  
                  }else if ( strcmp(buffer, "2")==0){
                        // Otherwise, handle the input as usual
                        send_message_to_all_clients(clients, client, actual, buffer, 0);
                        
                  }else if(strcmp(buffer, "3") == 0) {
                     printf("buffer = %s", buffer);
                     write_client(clients[i].sock, "Entrez le pseudo de l'adversaire de votre choix.\n");
                     char receiverName[BUF_SIZE]; 

                     read_client(clients[i].sock, buffer); 
                     sscanf(buffer, "%s", receiverName); 
                     Client *receiver = findClientByName(receiverName, clients, nbClients); 
                     if(receiver != NULL){
                        write_client(clients[i].sock, "La demande a été envoyé. Nous attendons sa réponse.\n");
                        int result = sendAwaleChallenge(&clients[i], receiver);
                        if(result == 1){
                           launchGame(&clients[i],receiver);
                        }
                        
                     }else{
                        write_client(clients[i].sock, "Votre adversaire n'a pas été trouvé\n");
                     }
                     
                        // If client typed "2", send "rouge" back to the client
                        
                  }else if(strcmp(buffer, "4")==0){
                  
                  
                  }
               }
               break;
            }
         }
      }
   }}

   clear_clients(clients, actual);
   end_connection(sock);
}

static void clear_clients(Client *clients, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      closesocket(clients[i].sock);
   }
}

static void remove_client(Client *clients, int to_remove, int *actual)
{
   /* we remove the client in the array */
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   /* number client - 1 */
   (*actual)--;
}

static void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      /* we don't send message to the sender */
      if(sender.sock != clients[i].sock)
      {
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
         }
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(clients[i].sock, message);
      }
   }
}



static int init_connection(void)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);

   

}

static int read_client(SOCKET sock, char *buffer)
{
   int n = 0;
   

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
   }else{
      //printf("rcv  = %s",recv(sock, buffer, BUF_SIZE - 1, 0));
      printf("buff =%s", buffer);
   }
   
   buffer[n] = 0;

   return n;
}
static void write_client(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

int sendAwaleChallenge(Client *sender, Client *receiver){
   if( receiver->challenge.status != CHALLENGE_NONE){
      write_client(sender->sock, "Ce destinataire est déjà occupé avec une autre partie de Awale.\n"); 
      return 0; 
   }
   //envoie la demande à l'adversaire
   char message[2048];
   snprintf(message, sizeof(message), "%s vous a invité à jouer au jeu Awalé. Acceptez avec accept ou refusez avec refuse.\n", sender->name); 
   write_client(receiver->sock, message);

   //attend la réponse de l'adversaire
   char reponse[BUF_SIZE];
   read_client(receiver->sock, reponse); 

   //traiter la réponse de l'adversaire
   if(strcmp(reponse, "accept") == 0){
      write_client(sender->sock, "Votre invitation a été acceptée.\n"); 
      
      return 1;

   }else if(strcmp(reponse, "refuse") == 0){
      write_client(sender->sock, "Votre invitation a été refusée.\n"); 
      return 0;
   }else{
      write_client(sender->sock, "Réponse invalide, la demande a été annulée.\n"); 
      return 0;
   }

}

void launchGame(Client * joueur1, Client * joueur2){

   char* s = initialiserPlateau(joueur1->name, joueur2->name); 
   write_client(joueur1->sock, s); 
   write_client(joueur2->sock, s);

   int statut = finDePartie();
   int tour = aQuiLeTour(); 
   char *reponse = malloc(100); 
   int coup; 
   char *affichage;

   while(statut != 1){
      printf("Je rentre dans le while du jeu\n");
      if(tour == 1){
         write_client(joueur1->sock,"Joueur 1, c'est votre tour.Choisissez une case (de 0 a 5) \n"); 
         read_client(joueur1->sock, reponse);
         coup = atoi(reponse); 
         jouerCoup(coup);
         affichage = afficherPlateau(); 
         write_client(joueur1->sock, "Voici l'état du jeu :\n");
         write_client(joueur1->sock, affichage); 
         write_client(joueur2->sock, "Le joueur 1 a joué.\n"); 
         write_client(joueur2->sock, affichage); 
         
      }else{
         write_client(joueur2->sock,"Joueur 2, c'est votre tour.Choisissez une case (de 0 a 5) \n"); 
         read_client(joueur2->sock, reponse);
         coup = atoi(reponse); 
         jouerCoup(coup);
         affichage = afficherPlateau(); 
         write_client(joueur2->sock, "Voici l'état du jeu :\n");
         write_client(joueur2->sock, affichage); 
         write_client(joueur1->sock, "Le joueur 2 a joué.\n"); 
         write_client(joueur1->sock, affichage); 
         
      }
      statut = finDePartie();
      tour = aQuiLeTour(); 
      
   }
   free(reponse); 
}

int main()
{
   init();

   app();

   end();

   return EXIT_SUCCESS;
}
