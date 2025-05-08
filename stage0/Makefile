
CC   := ${CROSS_COMPILE}gcc
AS   := ${CROSS_COMPILE}gcc
AR   := ${CROSS_COMPILE}ar
LD   := ${CROSS_COMPILE}ld
OBJC := ${CROSS_COMPILE}objcopy
OBJD := ${CROSS_COMPILE}objdump
NM   := ${CROSS_COMPILE}nm
SIZE := ${CROSS_COMPILE}size

ARCH ?= riscv

O = build

ASFLAGS = \
	-D__ASSEMBLY__ \
	-march=rv64imafdcv_xtheadcmo_xtheadsync -mstrict-align \
	-mcmodel=medany \
	-mabi=lp64d \
	-ffreestanding  \
	-Wa,--fatal-warnings

CFLAGS = \
	-march=rv64imafdcv_xtheadcmo_xtheadsync -mstrict-align \
	-mcmodel=medany \
	-mabi=lp64d \
	-ffreestanding  \
	-Wa,--fatal-warnings

LDFLAGS = \
	--fatal-warnings -Os \
	--gc-sections

ARCH_ASRC := $(wildcard arch/$(ARCH)/*.S)
ARCH_CSRC := $(wildcard arch/$(ARCH)/*.c)
COMMON_CSRC := $(shell find common/ -name '*.c')
INCS := common/include arch/$(ARCH)/

CSRC := $(ARCH_CSRC) $(COMMON_CSRC)
ASRC := $(ARCH_ASRC)
SRCS := $(CSRC) $(ASRC)

OBJS := $(SRCS:%=$(O)/%.o)

INCLUDES := $(addprefix -I,$(INCS))

NAME := stage0

.PHONY: clean all
all: $(O)/$(NAME).elf $(O)/$(NAME).bin

clean:
	rm -r $(O)

$(O)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

$(O)/%.S.o: %.S
	mkdir -p $(dir $@)
	$(AS) $(INCLUDES) $(CFLAGS) $(ASFLAGS) -c $< -o $@

$(O)/arch/$(ARCH)/stage0.ld: arch/$(ARCH)/ld/stage0.ld.S
	$(CC) $(INCLUDES) -E -o $@ $<

$(O)/$(NAME).elf: $(O)/arch/$(ARCH)/stage0.ld $(OBJS)
	$(LD) $(LDFLAGS) --script $(O)/arch/$(ARCH)/stage0.ld -o $@ $(OBJS)

$(O)/$(NAME).bin: $(O)/$(NAME).elf
	@$(SIZE) $<
	$(OBJC) -O binary $< $@
