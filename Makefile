# RISC-V-Sim Makefile
# Layout: src/*.c -> build/*.o -> bin/riscv-sim, headers in include/
# Tests:  tests/**/NAME.bin (program) + tests/**/NAME.res (expected register dump)
#
# Assumed simulator interface:   bin/riscv-sim <program.bin> <output.res>
# (change the SIM_RUN line below if yours differs)

CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -O2 -g -Iinclude -MMD -MP
LDFLAGS :=
LDLIBS  :=

TARGET  := bin/riscv-sim

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=build/%.o)
DEPS := $(OBJS:.o=.d)

TEST_BINS    := $(sort $(wildcard tests/*.bin tests/*/*.bin tests/*/*/*.bin))
TEST_OUT     := build/test-out
TEST_TIMEOUT := 5
# How one test is run: $$in = program, $$out = where the dump should be written
SIM_RUN       = timeout $(TEST_TIMEOUT) ./$(TARGET) "$$in" "$$out"

ZIP := riscv-sim.zip

.PHONY: all run test clean dist

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

# make run ARGS="tests/task1/addlarge.bin out.res"
run: $(TARGET)
	./$(TARGET) $(ARGS)

# Runs every .bin under tests/ and compares the binary register dump with the .res next to it.
# On a mismatch, cmp's 1-based byte offset tells you the register: x = (byte - 1) / 4
test: $(TARGET)
	@mkdir -p $(TEST_OUT)
	@pass=0; fail=0; \
	for in in $(TEST_BINS); do \
	  exp="$${in%.bin}.res"; \
	  if [ ! -f "$$exp" ]; then echo "SKIP $$in (no .res)"; continue; fi; \
	  name=$$(echo "$${in%.bin}" | tr / _); \
	  out="$(TEST_OUT)/$$name.res"; \
	  rm -f "$$out"; \
	  if $(SIM_RUN) > "$(TEST_OUT)/$$name.log" 2>&1 && cmp -s "$$out" "$$exp"; then \
	    pass=$$((pass + 1)); \
	  else \
	    fail=$$((fail + 1)); \
	    if [ ! -f "$$out" ]; then echo "FAIL $$in (no output, see $(TEST_OUT)/$$name.log)"; \
	    else echo "FAIL $$in: $$(cmp "$$out" "$$exp" 2>&1 | head -n1)"; fi; \
	  fi; \
	done; \
	echo "$$pass passed, $$fail failed"; \
	[ $$fail -eq 0 ]

bin build:
	mkdir -p $@

clean:
	rm -rf build bin $(ZIP)

# Hand-in zip: source + Makefile only (no build output)
dist: clean
	zip -r $(ZIP) Makefile README.md src include

-include $(DEPS)
