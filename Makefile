client: server client_3.cpp 
	g++ client_3.cpp -o client -g
server: server_3.cpp
	g++ server_3.cpp -o server -g
clean:
	rm client server
