CC := gcc
CFLAGS := -Wall -fPIC

LIBRARY_NAME := libAppLatency.so
SRC_DIR := AppLatency/src
INCLUDE_DIR := AppLatency/include
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, %.o, $(SOURCES))

.PHONY: all clean

all: $(LIBRARY_NAME)

$(LIBRARY_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -shared -o $@ $^ -lopen62541
	@echo "Shared library '$(LIBRARY_NAME)' created."

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I$(INCLUDE_DIR)

clean:
	rm -f $(OBJECTS) $(LIBRARY_NAME)

