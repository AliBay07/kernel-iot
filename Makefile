
# Copyright: Olivier Gruber (olivier dot gruber at acm dot org)
#
# This program is free software: you can redistribute it and/or modify it under the terms
# of the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program.
# If not, see <https://www.gnu.org/licenses/>.

#----------------- Configuration ------------------
BOARD       = versatile
CPU         = cortex-a8
TOOLCHAIN   = arm-none-eabi
DEBUG       ?= yes
BUILD       = build/
BUILD_ASM   = $(BUILD)asm/
BUILD_SRC   = $(BUILD)src/
SRC_DIR     = src
ASM_DIR     = asm
INC_DIR     = include

SRC_C       = $(wildcard $(SRC_DIR)/*.c)
SRC_ASM     = $(wildcard $(ASM_DIR)/*.s)

OBJS_ASM    = $(patsubst $(ASM_DIR)/%.s,$(BUILD_ASM)/%.o,$(SRC_ASM))
OBJS_C      = $(patsubst $(SRC_DIR)/%.c,$(BUILD_SRC)/%.o,$(SRC_C))

OBJS        = $(OBJS_ASM) $(OBJS_C)

#--------------------------------------------------
# Board-specific settings for Versatile Board
ifeq ($(BOARD),versatile)
  VGA       = -nographic
  SERIAL    = -serial mon:stdio
  MEMSIZE   = 32
  MEMORY    = "$(MEMSIZE)K"
  MACHINE   = versatileab
  QEMU      = qemu-system-arm
  QEMU_ARGS = -M $(MACHINE) -cpu $(CPU) -m $(MEMORY) $(VGA) $(SERIAL)

  CFLAGS    = -c -mcpu=$(CPU) -nostdlib -ffreestanding -I$(INC_DIR)
  CFLAGS   += -DCPU=$(CPU) -DMEMORY="($(MEMSIZE)*1024)"
  ASFLAGS   = -mcpu=$(CPU)
  LDFLAGS = -T ld/kernel.ld -nostdlib -static
endif

ifeq ($(DEBUG),yes)
  CFLAGS   += -ggdb
  LDFLAGS  += -g
  ASFLAGS  += -g
endif

ifndef MACHINE
  $(error Must choose a board (e.g. Versatile AB or PB))
endif

#----------------- Build Rules -------------------
build:
	@mkdir -p $(BUILD) $(BUILD_ASM) $(BUILD_SRC)

# Compile Assembly files
$(BUILD_ASM)/%.o: $(ASM_DIR)/%.s | build
	$(TOOLCHAIN)-as $(ASFLAGS) -o $@ $<

# Compile C files
$(BUILD_SRC)/%.o: $(SRC_DIR)/%.c | build
	$(TOOLCHAIN)-gcc $(CFLAGS) -o $@ $<

# Link kernel
$(BUILD)kernel.elf: $(OBJS)
	$(TOOLCHAIN)-ld $(LDFLAGS) $(OBJS) -o $@

# Convert ELF to binary
$(BUILD)kernel.bin: $(BUILD)kernel.elf
	$(TOOLCHAIN)-objcopy -O binary $< $@

all: $(BUILD)kernel.bin

clean:
	rm -rf $(BUILD)

#----------------- QEMU Targets ------------------
ifeq ($(BOARD),versatile)
run: all
	@echo "\n\nBoard: Versatile Board...\n"
	$(QEMU) $(QEMU_ARGS) -device loader,file=$(BUILD)kernel.elf

debug: all
	@echo "\n\nBoard: Versatile Board (Debug Mode)...\n"
	$(QEMU) $(QEMU_ARGS) -device loader,file=$(BUILD)kernel.elf -gdb tcp::1234 -S
endif

