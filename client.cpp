/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>

using namespace std;
//#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 200 // max number of bytes we can get at once 


//Various Semantic check functions
void checkDate(string date){

    return;
}

void checkTime(string time){

    return;
}

void checkMutualTime(string stime, string etime){

    return;
}

//Semantic Checks End

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	if (argc < 6) {
	    fprintf(stderr,"usage: client hostname port username function date\n");
	    exit(1);
	}

    memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure
    




    //my code
    //
    //1. Get input from user
    //2. parse function call
    //3. check & concatenate
    //4. send fully checked string

    string USERNAME(argv[3]);
    string FUNCTION(argv[4]);
    string DATE(argv[5]);

    checkDate(DATE);
    
    if(FUNCTION == "add"){

        int arg_size = 9;

     	if (argc != arg_size ) {
	    fprintf(stderr,"usage: client hostname port username add date stime etime etype\n");
	    exit(1);
	    }

        string FUNCTION_CODE = "1";
        string STIME = argv[6];
        string ENDTIME = argv[7];
        string ETYPE = argv[8];
        
        // Various semantic checks
        checkTime(STIME);
        checkTime(ENDTIME);
        checkMutualTime(STIME,ENDTIME);

        string string_to_send = FUNCTION_CODE+ " " + USERNAME + " " + DATE + " " + STIME + " " + ENDTIME + " " + ETYPE;  

        if (send(sockfd,string_to_send.c_str(), MAXDATASIZE, 0) == -1)
		    		perror("send");
	
        char buf[MAXDATASIZE]; 
        
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	        perror("recv");
	        exit(1);
	    }
        buf[numbytes]='\0';
        printf("%s\n",buf);
    }

    else if(FUNCTION == "remove"){
    	
        int arg_size = 7;

        if (argc != arg_size ) {
	    fprintf(stderr,"usage: client hostname port username remove date stime\n");
	    exit(1);
	    }

        string FUNCTION_CODE = "2";
        string STIME(argv[6]);
        string ENDTIME("");
        string ETYPE("");
        
        // Various semantic checks
        checkTime(STIME);
       
        // TO: Take care of extra spaces at behind
        string string_to_send =  FUNCTION_CODE+ " " + USERNAME+" "+DATE+" "+STIME+" "+ENDTIME+" "+ETYPE;  

        if (send(sockfd,string_to_send.c_str(), MAXDATASIZE, 0) == -1)
		    		perror("send");
    
        string line;
        char buf[MAXDATASIZE]; 
	    do{
            if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	            perror("recv");
	            exit(1);
            }

            buf[numbytes]='\0';
//            printf("%s\n",buf);
            line = buf;
            cout << line;
        }
        while(line != "0001000"); //recv killer
    }
    
    else if(FUNCTION == "update"){
 
        int arg_size = 9;

     	if (argc != arg_size ) {
	    fprintf(stderr,"usage: client hostname port username update date stime etime etype\n");
	    exit(1);
	    }

        string FUNCTION_CODE = "3";
        string STIME(argv[6]);
        string ENDTIME(argv[7]);
        string ETYPE(argv[8]);
        
        // Various semantic checks
        // check for conflicts will be done at server side
        checkTime(STIME);
        checkTime(ENDTIME);
        checkMutualTime(STIME,ENDTIME);

        string string_to_send =  FUNCTION_CODE+ " " + USERNAME+" "+DATE+" "+STIME+" "+ENDTIME+" "+ETYPE;  

        if (send(sockfd,string_to_send.c_str(), MAXDATASIZE, 0) == -1)
		    		perror("send");
   
    }
    
    else if(FUNCTION == "get"){
        
        int arg_size1 = 6;
        int arg_size2 = 7;

        if (argc != arg_size1 && argc != arg_size2) {
	    fprintf(stderr,"usage: client hostname port username remove date stime\n");
	    exit(1);
	    }

        string FUNCTION_CODE = "4";
        string STIME;
        
        if(argc==arg_size2){
            STIME = argv[6];
            checkTime(STIME);
        }
        else{
            STIME = "";
        }

        string ENDTIME("");
        string ETYPE("");
        
       
        // TO: Take care of extra spaces at behind
        string string_to_send =  FUNCTION_CODE+ " " + USERNAME+" "+DATE+" "+STIME+" "+ENDTIME+" "+ETYPE;  

        if (send(sockfd,string_to_send.c_str(), MAXDATASIZE, 0) == -1)
		    		perror("send");
    }
    
    else{
        printf("Unsupported function call/n");
        exit(1);
    }

    //my code ends

    /*
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}
    

	buf[numbytes] = '\0';

    printf("client: received '%s'\n",buf);
    */

	close(sockfd);

	return 0;
}
