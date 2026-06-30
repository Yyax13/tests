SRC := main.c
CC := gcc
CC_FLAGS := -O3
I_FLAGS := 
TARGET := main

all: help

build: ## Build the project
	$(CC) $(SRC) -o $(TARGET) $(CC_FLAGS) $(I_FLAGS)

help: ## Print this help menu
	@grep -E '^[a-z.A-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

