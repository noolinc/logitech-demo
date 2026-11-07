CC ?= cc
CFLAGS := -std=c11 -Wall -Wextra -O2 -Iinclude

SRC := src/gpio_mock.c src/led.c src/button.c src/sensor.c \
       src/battery_mock.c src/radio_mock.c

test: bin/test_hal
	./bin/test_hal

bin:
	mkdir -p bin

bin/test_hal: $(SRC) tests/test_button.c | bin
	$(CC) $(CFLAGS) $(SRC) tests/test_button.c -o $@

clean:
	rm -rf bin
