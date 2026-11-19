CC       := gcc
CFLAGS   := -Wall -Wextra -pthread

TARGET_CLIENT := chat_client
TARGET_SERVER := chat_server

.PHONY: all clean

all: $(TARGET_CLIENT) $(TARGET_SERVER)

$(TARGET_CLIENT): chat_client.o port_utils.o
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET_SERVER): chat_server.o port_utils.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET_CLIENT) $(TARGET_SERVER)
