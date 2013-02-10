client: server
	g++ client.cpp -o client
server:
	g++ server.cpp -o server
clean:
	rm client server
