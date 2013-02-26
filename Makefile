client: server client_2.cpp 
	g++ client_2.cpp -o client -g
server: server_2.cpp
	g++ server_2.cpp -o server -g
clean:
	rm client server
