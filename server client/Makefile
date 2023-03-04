CLIENT = client.o outils.o
SERVER = server.o outils.o

all: client server

client: $(CLIENT)
	c++ -o client $(CLIENT)

server: $(SERVER)
	c++ -o server $(SERVER) -lpthread

-include $(CLIENT:.o=.d)
-include $(SERVER:.o=.d)

%.o: %.cpp
	c++ -c $*.cpp -o $*.o -Wall -Wshadow -Werror -O
	c++ -MM $*.cpp > $*.d