CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build

CLIENT_TARGET = $(BUILD_DIR)/client
SERVER_TARGET = $(BUILD_DIR)/serveur

CLIENT_OBJ_FILES = $(BUILD_DIR)/client.o $(BUILD_DIR)/common.o $(BUILD_DIR)/formateur.o
SERVER_OBJ_FILES = $(BUILD_DIR)/serveur.o $(BUILD_DIR)/common.o 

all: $(BUILD_DIR) $(CLIENT_TARGET) $(SERVER_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	
$(CLIENT_TARGET): $(CLIENT_OBJ_FILES)
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_OBJ_FILES)

$(SERVER_TARGET): $(SERVER_OBJ_FILES)
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_OBJ_FILES)

$(BUILD_DIR)/client.o: $(SRC_DIR)/client/client.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/formateur.o: $(SRC_DIR)/client/formateur.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/serveur.o: $(SRC_DIR)/serveur/serveur.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/common.o: $(SRC_DIR)/common.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(CLIENT_TARGET) $(SERVER_TARGET) $(CLIENT_OBJ_FILES) $(SERVER_OBJ_FILES)
