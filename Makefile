
CC   := ${CROSS_COMPILE}gcc
AS   := ${CROSS_COMPILE}gcc
AR   := ${CROSS_COMPILE}ar
LD   := ${CROSS_COMPILE}ld
OBJC := ${CROSS_COMPILE}objcopy
OBJD := ${CROSS_COMPILE}objdump
NM   := ${CROSS_COMPILE}nm
SIZE := ${CROSS_COMPILE}size

ARCH ?= riscv
ARCH_UPPER := $(shell echo $(ARCH) | tr '[:lower:]' '[:upper:]')

O ?= build/$(ARCH)

BUILD := $(O)

ASFLAGS = \
	-D__ASSEMBLY__

CFLAGS = \
	-march=rv64imafdcv_xtheadcmo_xtheadsync -mstrict-align \
	-mcmodel=medany \
	-mabi=lp64d \
	-ffreestanding  \
	-ffunction-sections -fdata-sections \
	-Wa,--fatal-warnings \
	-D$(ARCH_UPPER)

LDFLAGS = \
	--fatal-warnings -Os \
	--gc-sections

COMMON_CSRC := $(shell find common/driver -name '*.c') $(shell find common/arch/$(ARCH) -name '*.c')
COMMON_OBJS := $(COMMON_CSRC:%=$(BUILD)/%.o)
COMMON_INCS := common/include common/arch/$(ARCH)
COMMON_INCLUDES := $(addprefix -I,$(COMMON_INCS))

STAGE0_SRC := $(shell find stage0/arch/$(ARCH) -name '*.c' -or -name '*.S') \
              $(shell find stage0/common -name '*.c')
STAGE0_OBJS := $(STAGE0_SRC:%=$(BUILD)/%.o)
STAGE0_INCS := stage0/arch/$(ARCH)/ stage0/common/include/ $(COMMON_INCS)
STAGE0_INCLUDES := $(addprefix -I,$(STAGE0_INCS))

.PHONY: clean all

all: $(BUILD)/stage0.bin

clean:
	rm -r $(O)

$(filter %.c.o, $(COMMON_OBJS)): $(BUILD)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(COMMON_INCLUDES) $(CFLAGS) -c $< -o $@

$(filter %.S.o, $(STAGE0_OBJS)): $(BUILD)/%.S.o: %.S
	@mkdir -p $(dir $@)
	$(AS) $(STAGE0_INCLUDES) $(CFLAGS) $(ASFLAGS) -c $< -o $@

$(filter %.c.o, $(STAGE0_OBJS)): $(BUILD)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(AS) $(STAGE0_INCLUDES) $(CFLAGS) -c $< -o $@

$(BUILD)/stage0/arch/$(ARCH)/link.ld: stage0/arch/$(ARCH)/ld/link.ld
	@mkdir -p $(dir $@)
	$(CC) $(STAGE0_INCLUDES) $(CFLAGS)  -x c -E -o $@ $<

$(BUILD)/stage0.elf: $(BUILD)/stage0/arch/$(ARCH)/link.ld $(STAGE0_OBJS) $(COMMON_OBJS)
	$(LD) $(LDFLAGS) --script $(BUILD)/stage0/arch/$(ARCH)/link.ld -o $@ $(STAGE0_OBJS) $(COMMON_OBJS)

$(BUILD)/%.bin: $(BUILD)/%.elf
	@$(SIZE) $<
	$(OBJC) -O binary $< $@
