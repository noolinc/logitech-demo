CC ?= cc
CFLAGS := -std=c11 -Wall -Wextra -O2 -Iinclude -Ivendor/contracts/include

SRC := src/packet.c

test: bin/test_packet
	./bin/test_packet

vendor:
	./refresh-vendor.sh

bin:
	mkdir -p bin

bin/test_packet: $(SRC) tests/test_packet.c | bin
	$(CC) $(CFLAGS) $(SRC) tests/test_packet.c -o $@

clean:
	rm -rf bin
