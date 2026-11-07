CC ?= cc
CFLAGS := -std=c11 -Wall -Wextra -O2 -Iinclude

test: bin/test_contract
	./bin/test_contract

bin:
	mkdir -p bin

bin/test_contract: tests/test_contract.c include/logi/proto.h | bin
	$(CC) $(CFLAGS) tests/test_contract.c -o $@

clean:
	rm -rf bin
