CC ?= cc
CFLAGS := -std=c11 -Wall -Wextra -O2 -Iinclude \
   -I../../platform/contracts/include \
   -I../../platform/mcu-protocol/include \
   -I../../platform/hal-core/include

FW_SRC := src/main.c src/matrix.c src/report.c
DEPS := ../../platform/mcu-protocol/src/packet.c \
        ../../platform/hal-core/src/gpio_mock.c \
        ../../platform/hal-core/src/led.c \
        ../../platform/hal-core/src/button.c \
        ../../platform/hal-core/src/sensor.c \
        ../../platform/hal-core/src/battery_mock.c \
        ../../platform/hal-core/src/radio_mock.c

test: bin/test_fw
	./bin/test_fw

sim: bin/fw_sim
	./bin/fw_sim

bin:
	mkdir -p bin

bin/test_fw: $(FW_SRC) $(DEPS) tests/test_smoke.c | bin
	$(CC) $(CFLAGS) -DLOGI_FW_LIBONLY $(FW_SRC) $(DEPS) tests/test_smoke.c -o $@

bin/fw_sim: $(FW_SRC) $(DEPS) | bin
	$(CC) $(CFLAGS) $(FW_SRC) $(DEPS) -o $@

clean:
	rm -rf bin
