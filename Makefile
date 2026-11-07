CC ?= cc
CFLAGS := -std=c11 -Wall -Wextra -O2 -Iinclude \
   -Ivendor/contracts/include \
   -Ivendor/mcu-protocol/include \
   -Ivendor/hal-core/include

FW_SRC := src/main.c src/motion.c src/buttons.c
VENDORS := vendor/mcu-protocol/src/packet.c \
        vendor/hal-core/src/gpio_mock.c \
        vendor/hal-core/src/led.c \
        vendor/hal-core/src/button.c \
        vendor/hal-core/src/sensor.c \
        vendor/hal-core/src/battery_mock.c \
        vendor/hal-core/src/radio_mock.c

# Vendor copies are refreshed with ./refresh-vendor.sh (keeps ghost-run
# validation repo-isolated and reproducible in CI).
test: bin/test_fw
	./bin/test_fw

sim: bin/fw_sim
	./bin/fw_sim 6

bin:
	mkdir -p bin

bin/test_fw: $(FW_SRC) $(VENDORS) tests/test_smoke.c | bin
	$(CC) $(CFLAGS) -DLOGI_FW_LIBONLY $(FW_SRC) $(VENDORS) tests/test_smoke.c -o $@

bin/fw_sim: $(FW_SRC) $(VENDORS) | bin
	$(CC) $(CFLAGS) $(FW_SRC) $(VENDORS) -o $@

vendor:
	./refresh-vendor.sh

clean:
	rm -rf bin
