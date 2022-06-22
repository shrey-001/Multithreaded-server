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
typedef struct sockaddr_in SA_IN ;
typedef struct sockaddr SA ;


using namespace std;

#define SERVERPORT 8989

thread t_send, t_recv;
string def_col="\033[0m";
string colors="\033[31m";

void catch_ctrl_c(int signal);
void send_message(int client_socket);
void recieve_message(int client_socket);

int client_socket;
bool exit_flag=false;

int main()
{
	int client_socket = 0;
    int client_fd;
    SA_IN server_address;

	//socket creation
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout<<"Socket creation error";
        return -1;
    }
 
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVERPORT);
 
	//convert IPv4 address to binary format
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)
        <= 0) {
        cout<<"Invalid address/ Address not supported";
        return -1;
    }

    if ((client_fd
         = connect(client_socket, (struct sockaddr*)&server_address,
                   sizeof(server_address)))< 0) {
        cout<<"Connection Failed: ";
        return -1;
    }
	char name[MAX_LEN];
	cout<<"Enter your name : ";
	cin.getline(name,MAX_LEN);
	send(client_socket,name,sizeof(name),0);

	cout<<"\n\t  ----- Welcome Everyone -----   "<<endl;

	thread t1(send_message, client_socket);
	thread t2(recieve_message, client_socket);

	t_send=move(t1);
	t_recv=move(t2);

	if(t_send.joinable()) t_send.join();
	if(t_recv.joinable()) t_recv.join();
			
	return 0;
}
// Send message to everyone
void send_message(int client_socket)
{
	while(true){
		cout<<colors<<"You : "<<def_col;
		char str[MAX_LEN];
		cin.getline(str,MAX_LEN);
		send(client_socket,str,sizeof(str),0);	
	}		
}
// Recieve message
void recieve_message(int client_socket)
{
	while(true)
	{
		if(exit_flag)
			return;
		char name[MAX_LEN], str[MAX_LEN];
		int color_code;
		int bytes_received=recv(client_socket,name,sizeof(name),0);
		if(bytes_received<=0)
			continue;
		recv(client_socket,&color_code,sizeof(color_code),0);
		recv(client_socket,str,sizeof(str),0);
		for(int i=0; i<6; i++){
		    cout<<"\b";
	    }
		if(strcmp(name,"#NULL")!=0)
			cout<<colors<<name<<" : "<<def_col<<str<<endl;
		else
			cout<<colors<<str<<endl;
		cout<<colors<<"You : "<<def_col;
		fflush(stdout);
	}	
}