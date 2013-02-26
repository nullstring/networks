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
#include <ctype.h>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;
//#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 200 // max number of bytes we can get at once 
#define MAXALL 4000

//splitting
std::vector<string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while(std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        return split(s, delim, elems);
}

//trimming
std::string trim(const std::string& str,
                 const std::string& whitespace = " \t")
{
    const auto int strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto int strEnd = str.find_last_not_of(whitespace);
    const auto int strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

bool is_digits(string str){
    //c11 not working in mac - gcc version problems
    //return std::all_of(s.begin(),s.end(),::isdigit);
    return str.find_first_not_of("0123456789") == std::string::npos;
}


size_t Recv(int remoteSocket,char* buffer, size_t size) {
        size_t total = 0, n = 0;
        while((n = ::recv(remoteSocket, buffer+total, size-total-1, 0)) > 0) {
            total += n;
        }
        buffer[total] = 0;
        return total;
}


//Various Semantic check functions
void checkDate(string date){

    if(!is_digits(date) || date.length() != 6){
        cout<< date <<" : should be 6 valid digits" <<endl;
        cout << "Operation not performed : please retry" <<endl;
        exit(1); 
    }
   
    int month = atoi(date.substr(0,2).c_str());
    int day = atoi(date.substr(2,2).c_str());
    int year = atoi(date.substr(4,2).c_str());
    /*
    //c11 not working in mac - gcc version problems
    int month = stoi(date.substr(0,2),10);
    int day = stoi(date.substr(2,2),10);
    int year = stoi(date.substr(4,2),10);
    */

    if(month > 12 || day > 31 || (day==31 && (month == 2 || month==4 || month == 6 || month == 9 || month == 11)) || (month == 2 && year%4 != 0 && day == 29)){
        cout<< date <<" : Date failed semantic checks" <<endl;
        cout << "Operation not performed : please retry" <<endl;
        exit(1);
    }
    
    return;
}

void checkTime(string str_time){

    //int itime = stoi(time);
    if(!is_digits(str_time) || str_time.length() != 4){
        cout<< str_time<<" : should be 4 valid digits" <<endl;
        cout << "Operation not performed : please retry" <<endl;
        exit(1); 
    }
    
    string hour = str_time.substr(0,2);
    string min = str_time.substr(2,2);
    
    if(hour>"24"){
        cout<< hour <<" : 00 <= hour <= 24" <<endl;
        cout << "Operation not performed : please retry" <<endl;
        exit(1);
    }
    else if(min>"59"){
        cout<< min <<" : 00 <= min < 60" <<endl;
        cout << "Operation not performed : please retry" <<endl;
        exit(1);
    }

    return;
}

void checkMutualTime(string stime, string etime){

    if(stime>=etime){
        cout<< "Time Constraints : start time < end time" <<endl;
        cout << "Operation not performed : please retry" <<endl;
        exit(1);
    }

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

	if (argc < 5) {
	    fprintf(stderr,"usage: client hostname port username function \n");
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
    
    if(FUNCTION == "getall"){
    
        int arg_size = 5;

        if (argc != arg_size) {
	    fprintf(stderr,"usage: client hostname port username remove date stime\n");
	    exit(1);
	    }

        
        string FUNCTION_CODE = "0";
        
        // TO: Take care of extra spaces at behind
        string string_to_send =  FUNCTION_CODE+ " " + USERNAME;  

        if (send(sockfd,string_to_send.c_str(), MAXDATASIZE, 0) == -1)
		    		perror("send");
    
        //recv from event_get
        string line;
        char buf[MAXALL]; 
        if ((numbytes = recv(sockfd, buf, MAXALL-1, 0)) == -1) {
	        perror("recv");
	        exit(1);
        }
        buf[numbytes]='\0';
        line = buf;
        line = trim(line," /");
        vector<string> getall_data = split(line,'/');
        cout << "Number of Entries for "<<USERNAME<<" : "<<getall_data.size()<<endl;

        for(int i=0 ; i<getall_data.size() ; i++){
            sleep(2);
            cout<<getall_data[i]<<endl;    
        }
    
    }
    else if(FUNCTION == "add"){

        int arg_size = 9;

     	if (argc != arg_size ) {
	    fprintf(stderr,"usage: client hostname port username add date stime etime etype\n");
	    exit(1);
	    }

        string DATE(argv[5]);
        checkDate(DATE);
        
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
        
        string DATE(argv[5]);
        checkDate(DATE);
        
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
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	        perror("1.recv");
	        exit(1);
        }
        buf[numbytes]='\0';
	    line = buf;
        cout <<line<<endl;
    }
    
    else if(FUNCTION == "update"){
 
        int arg_size = 9;

     	if (argc != arg_size ) {
	    fprintf(stderr,"usage: client hostname port username update date stime etime etype\n");
	    exit(1);
	    }

        string DATE(argv[5]);
        checkDate(DATE);
        
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
   
        //recv from event_update
        string line;
        char buf[MAXDATASIZE]; 
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	        perror("recv");
	        exit(1);
        }
        buf[numbytes]='\0';
	    line = buf;
        cout << line<<endl;

        //recv from event_add()
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	        perror("recv");
	        exit(1);
	    }
        buf[numbytes]='\0';
        printf("%s\n",buf);

   }
    
    else if(FUNCTION == "get"){
        
        int arg_size1 = 6;
        int arg_size2 = 7;

        if (argc != arg_size1 && argc != arg_size2) {
	    fprintf(stderr,"usage: client hostname port username remove date stime\n");
	    exit(1);
	    }

        string DATE(argv[5]);
        checkDate(DATE);
        
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
    
        //recv from event_get
        string line;
        char buf[MAXALL]; 
        if ((numbytes = recv(sockfd, buf, MAXALL-1, 0)) == -1) {
	        perror("recv");
	        exit(1);
        }
        buf[numbytes]='\0';
        line = buf;
        
        if(argc == arg_size2){
            cout << line<<endl;
        }
        else if(argc == arg_size1){

            line = trim(line," /");
            vector<string> getall_data = split(line,'/');
            cout << "Number of Entries for "<<USERNAME<<" : "<<getall_data.size()<<endl;

            for(int i=0 ; i<getall_data.size() ; i++){
                cout<<getall_data[i]<<endl;    
            }
        }

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
