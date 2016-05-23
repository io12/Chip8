CC = clang
CFLAGS = -g -Wall -std=gnu99
LDFLAGS = -lncurses
C_FILES = $(wildcard src/*.c)
H_FILES = $(wildcard src/*.h)
BUILD = c8

.PHONY = all install clean

$(BUILD): $(C_FILES) $(H_FILES)
	$(CC) $(CFLAGS) $(C_FILES) -o $@ $(LDFLAGS)

all: $(BUILD)

install: $(BUILD)
	cp $(BUILD) /usr/local/bin/

clean: rm -f $(BUILD)
