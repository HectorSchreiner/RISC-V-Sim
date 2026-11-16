CC      := gcc
CFLAGS  := -std=c23 -Wall -Wextra -Wpedantic -O2 -g -Iinclude -MMD -MP

TARGET  := bin/riscv-sim

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=build/%.o)
DEPS := $(OBJS:.o=.d)

SIM_RUN = timeout $(TEST_TIMEOUT) ./$(TARGET) "$$in" "$$out"

ZIP := riscv-sim.zip

.PHONY: all run test clean dist

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) $(ARGS)

bin build:
	mkdir -p $@

clean:
	rm -rf build bin $(ZIP)

dist: clean
	zip -r $(ZIP) Makefile README.md src include

-include $(DEPS)
