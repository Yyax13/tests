
all: help

SRC := client.c
CC := gcc
CC_FLAGS := -O3
I_FLAGS := 
TARGET := client

build-client: ## Build the client
	$(CC) $(SRC) -o $(TARGET) $(CC_FLAGS) $(I_FLAGS)

SRC-SERVER := server.c
I_FLAGS-SERVER := 
TARGET-SERVER := server

build-server: ## Build the server
	$(CC) $(SRC-SERVER) -o $(TARGET-SERVER) $(CC_FLAGS) $(I_FLAGS-SERVER)

help: ## Print this help menu
	@grep -E '^[a-z.A-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'
