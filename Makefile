client: server1 server2 server3 client/client.cpp 
	g++ client/client.cpp -o client/client 
server1: server_iterative_1/server_1.cpp
	g++ server_iterative_1/server_1.cpp -o server_iterative_1/server 
server3: server_multiprocess_3/server_3.cpp
	g++ server_multiprocess_3/server_3.cpp -o server_multiprocess_3/server 
server2: server_select_2/server_2.cpp
	g++ server_select_2/server_2.cpp -o server_select_2/server 
cleanall:
	rm client/client server_iterative_1/server server_multiprocess_3/server server_select_2/server server_iterative_1/user* server_multiprocess_3/user* server_select_2/user*   
