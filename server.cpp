#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <pthread.h>
#define MAX_LEN 200

using namespace std;

struct client
{
	int id;
	string name;
	int socket;
	thread th;
};

vector<client> clients;
string def_col="\033[0m";
string colors="\033[0m";
int client_connected=0;


pthread_mutex_t cout_lock = PTHREAD_MUTEX_INITIALIZER ; 
pthread_mutex_t clients_lock = PTHREAD_MUTEX_INITIALIZER ; 



void handle_connection(int client_socket, int id);
int check(int exp, const char *msg);
void set_name(int id, char name[]);
void print(string str);
void sendToAll(string message, int sender_id);
void sendToAll(int num, int sender_id);
void endConnection(int id);


typedef struct sockaddr_in SA_IN ;
typedef struct sockaddr SA ;

#define SERVERPORT 8989
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100

int main(){
	int server_socket;
    SA_IN server_addr;

	//Socket Creation
    check((server_socket = socket(AF_INET , SOCK_STREAM , 0)),
            "Failed to create socket") ;
    
	//Address definition
    server_addr.sin_family = AF_INET ;
    server_addr.sin_addr.s_addr = INADDR_ANY ;
    server_addr.sin_port = htons(SERVERPORT) ;
    int opt=1;

	//Reuse the address and port number
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt))){
        cout<<"Setsocketopt error\n";
        exit(1);
    }

	//Bind the socket with the address and port
    check((bind(server_socket, (SA*)&server_addr , sizeof(server_addr))),
            "Bind Failed!") ;

	//Put the server in listning mode
    check(listen(server_socket, SERVER_BACKLOG),
            "Listen Failed!") ;

	SA client;
	int client_socket;
	unsigned int len=sizeof(SA);

	cout<<"\n\t  ----- Welcome Everyone -----   "<<endl;

	while(true){
		//Server accepting the new client connection
        check((client_socket=accept(server_socket,(struct sockaddr *)&client,&len))
            ,"Failed to accept client");

		client_connected++;

		//Thread creation
		thread t(handle_connection,client_socket,client_connected);

		//Mutex lock for accessing critical section
        pthread_mutex_lock(&clients_lock);
		clients.push_back({client_connected, string("Anonymous"),client_socket,(move(t))});
        pthread_mutex_unlock(&clients_lock);
    }
	//Wait untill all the thread has finished execution
	for(int i=0; i<clients.size(); i++){
		if(clients[i].th.joinable())
			clients[i].th.join();
	}
	//close the server
	close(server_socket);
	return 0;
}
//Handling the client
void handle_connection(int client_socket, int id){
	char name[MAX_LEN],str[MAX_LEN];

	//get the name of client
	recv(client_socket,name,sizeof(name),0);
	//set it
	set_name(id,name);	
	//send welcome message to all clients except itself 
	string welcome_message=string(name)+string(" has joined");
	sendToAll("#NULL",id);	
	sendToAll(id,id);								
	sendToAll(welcome_message,id);	
	print(colors+welcome_message+def_col);
	
	while(true)
	{
		int size_recv=recv(client_socket,str,sizeof(str),0);
		if(size_recv<=0)
			return;
		sendToAll(string(name),id);					
		sendToAll(id,id);		
		sendToAll(string(str),id);
		print(colors+name+" : "+def_col+str);		
	}	
}
//set the name of client
void set_name(int id, char name[]){
	for(int i=0; i<clients.size(); i++){
			if(clients[i].id==id)	clients[i].name=string(name);
	}	
}
// print the endline
void print(string str){
	//Mutex lock 
    pthread_mutex_lock(&cout_lock);
	cout<<str<<endl;
    pthread_mutex_unlock(&cout_lock);
}
// Send messages to all clients except itself
void sendToAll(string message, int sender_id){
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id!=sender_id){
			send(clients[i].socket,temp,sizeof(temp),0);
		}
	}		
}
// Send number to all clients except itself
void sendToAll(int num, int sender_id){
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id!=sender_id){
			send(clients[i].socket,&num,sizeof(num),0);
		}
	}		
}
//End the connection
void endConnection(int id){
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id==id){

			//Mutex Lock
            pthread_mutex_lock(&clients_lock);
			clients[i].th.detach();
			clients.erase(clients.begin()+i);
			close(clients[i].socket);
            pthread_mutex_unlock(&clients_lock);
			break;
		}
	}				
}
//print error and exit the code
int check(int exp, const char *msg) {
    if(exp == SOCKETERROR){
        perror(msg) ;
        exit(1) ;
    }
    return exp ;
}