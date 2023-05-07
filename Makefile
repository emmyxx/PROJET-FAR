CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build

CLIENT_TARGET = $(BUILD_DIR)/client
SERVER_TARGET = $(BUILD_DIR)/serveur

CLIENT_OBJ_FILES = $(BUILD_DIR)/client.o $(BUILD_DIR)/common.o $(BUILD_DIR)/formater_saisie_client.o
SERVER_OBJ_FILES = $(BUILD_DIR)/serveur.o $(BUILD_DIR)/common.o 

all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJ_FILES)
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_OBJ_FILES)

$(SERVER_TARGET): $(SERVER_OBJ_FILES)
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_OBJ_FILES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(CLIENT_TARGET) $(SERVER_TARGET) $(CLIENT_OBJ_FILES) $(SERVER_OBJ_FILES)
