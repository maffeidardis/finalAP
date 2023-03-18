CLIENT = client.o outils.o
SERVER = server.o outils.o
CC = c++
FLAGS = -lpthread

all: client server

server: $(SERVER)
	$(CC) -o server $(SERVER) $(FLAGS)

client: $(CLIENT)
	$(CC) -o client $(CLIENT) $(FLAGS)

-include $(SERVER:.o=.d)
-include $(CLIENT:.o=.d)

%.o: %.cpp
	$(CC) -c $*.cpp -o $*.o -Wall -Wshadow -Werror -O
	$(CC) -MM $*.cpp > $*.d