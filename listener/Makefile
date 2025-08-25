# Makefile
.DEFAULT_GOAL := build

#To be changed if we go with Ninja or a different out dir/config
GENERATOR ?= "NMake Makefiles"
BUILD_DIR ?= build
CONFIG ?= Release

configure:
	cmake -S . -B $(BUILD_DIR) -G $(GENERATOR)

build: configure
	cmake --build $(BUILD_DIR) --config $(CONFIG)

run: build
	# NMake generator puts binaries under build/$(CONFIG)/
	$(BUILD_DIR)/$(CONFIG)/receiver.exe

clean:
	- cmake --build $(BUILD_DIR) --target clean --config $(CONFIG)
	- cmake -E rm -rf $(BUILD_DIR)
    