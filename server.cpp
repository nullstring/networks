/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#define PORT "3490"  // the port users will be connecting to
#define MAXDATASIZE 200

#define BACKLOG 10	 // how many pending connections queue will hold

using namespace std;

vector <string> splitstr(string message){
    stringstream ss(message);
    string s;
    vector <string> str;
    while(ss>>s){
        //cout<<s<<endl;
        str.push_back(s);
    }
    return str;
}

//my code

//name date stime etime type
void event_add(int sock_fd,string message){

    vector<string> str = splitstr(message);
    
    string filename = "user_"+str[0];
    ofstream fpo;
    ifstream fpi;

    fpo.open(filename.c_str(), ofstream::app); // ofstream creates new file if not there, ordering of opening the file is important , dont change
    fpi.open(filename.c_str());

    string to_send = "";
    char buf[MAXDATASIZE];
    //fileformat: date stime etime type
    while(fpi.getline(buf,MAXDATASIZE)){
        
        string line = buf;
        vector<string> fpstr = splitstr(line);
        cout << "server while"<<endl; 
        //check for conflicts
        if(str[1] == fpstr[0] && ( str[2] < fpstr[2] && str[3] > fpstr[1]))
            to_send = "Conflict detected:  " + fpstr[0] + " " + fpstr[1] + " " + fpstr[2] + " " + fpstr[3];
    } 
   
    //no conflicts
    if(to_send.empty()){
        fpo << str[1] + " " + str[2] + " " + str[3] + " " + str[4] << endl;
        to_send = "Event added :  " + str[0] + " " + str[1] + " " + str[2] + " " + str[3] + " " + str[4] ;
    }

    if (send(sock_fd,to_send.c_str(),MAXDATASIZE, 0) == -1)
		perror("send");

    return;
}

void event_remove(int sock_fd,string message){

    vector<string> str = splitstr(message);
    
    string filename = "user_"+str[0];
    ifstream fpi;

    fpi.open(filename.c_str());

    string to_send = "";
    char buf[MAXDATASIZE];
    
    vector<string> file_buf;
    
    //fileformat: date stime etime type
    while(fpi.getline(buf,MAXDATASIZE)){
        
        string line = buf;
        vector<string> fpstr = splitstr(line);
        
        //check for conflicts
        if(str[1] == fpstr[0] && str[2] == fpstr[1]){
            to_send = "Entry Deleted:  " + fpstr[0] + " " + fpstr[1] + " " + fpstr[2] + " " + fpstr[3];
            
            if (send(sock_fd,to_send.c_str(),MAXDATASIZE, 0) == -1)
		        perror("send");
        }
        else{
            file_buf.push_back(line);
        }
    } 
   
    //no conflicts
    if(to_send.empty()){
        to_send = "No entry detected for :  " + str[0] + " " + str[1] + " " + str[2] + " " + str[3] + " " + str[4] ;
        
        if (send(sock_fd,to_send.c_str(),MAXDATASIZE, 0) == -1)
		    perror("send");
    }
    
    ofstream fpo;
    fpo.open(filename.c_str());
    
    for(int i = 0 ; i<file_buf.size() ;i++) 
    fpo << file_buf[i] <<endl;

    //recv killer : 0001000
    to_send = "0001000";
    if (send(sock_fd,to_send.c_str(),MAXDATASIZE, 0) == -1)
		perror("send");

    return;
}

void event_update(int sock_fd,string message){

    return;
}

void event_get(int sock_fd,string message){

    return;
}


//preprocessing step
//1. Parse string
//2. call appropriate function call
//name function date stime etime type
void preprocessingCall(int sock_fd, string message){
    
    int token = message.find(" ");
    string function_code = message.substr(0,token);
    string rest = message.substr(token+1);
   
    switch(function_code.c_str()[0]){
        case '1':
            printf("in add\n");
            event_add(sock_fd,rest);
            break;
        case '2':
            printf("in remove\n");
            event_remove(sock_fd,rest);
            break;
        case '3':
            printf("in update\n");
            event_update(sock_fd,rest);
            break;
        case '4':
            printf("in get\n");
            event_get(sock_fd,rest);
            break;
        default:
            printf("There was some error in transmitting function code/n");
            return;
    }


}

//my code ends

//not to change
void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

        /*
        //for linux
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
        */

        //for mac
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}


		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener



            //my code
            //1. Get data
            //2. Get what function to call
            //3. Call the function with remaining string

            char buf[MAXDATASIZE];
            int numbytes;

    	    if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) {
	            perror("recv");
	            exit(1);
	        }
	        buf[numbytes] = '\0';
            string message(buf);

            printf("server: received '%s'\n",buf);
       
            preprocessingCall(new_fd,message);


            
            //my code ends
            
           /* 
            if (send(new_fd, "Hello, world!", 13, 0) == -1)
				perror("send");
			*/

            close(new_fd);
			exit(0);
		}
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}

