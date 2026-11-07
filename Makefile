CC ?= cc
CFLAGS := -std=c11 -Wall -Wextra -O2 -Iinclude \
   -Ivendor/contracts/include \
   -Ivendor/mcu-protocol/include \
   -Ivendor/hal-core/include

FW_SRC := src/main.c src/pairing.c src/host_fwd.c
DEPS := vendor/mcu-protocol/src/packet.c \
        vendor/hal-core/src/gpio_mock.c \
        vendor/hal-core/src/led.c \
        vendor/hal-core/src/button.c \
        vendor/hal-core/src/sensor.c \
        vendor/hal-core/src/battery_mock.c \
        vendor/hal-core/src/radio_mock.c

test: bin/test_fw
	./bin/test_fw

bin:
	mkdir -p bin

bin/test_fw: $(FW_SRC) $(DEPS) tests/test_smoke.c | bin
	$(CC) $(CFLAGS) -DLOGI_FW_LIBONLY $(FW_SRC) $(DEPS) tests/test_smoke.c -o $@

clean:
	rm -rf bin

vendor:
	./refresh-vendor.sh
