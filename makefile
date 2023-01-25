CC          = powerpc-eabi-gcc
AS			= powerpc-eabi-gcc -x assembler-with-cpp
LD          = $(CC)
OBJCOPY     = powerpc-eabi-objcopy
CFILES      = *.c
SFILES		= *.s
SRCDIR		= src
OBJDIR 		= obj
BINDIR		= bin
VC_VERSIONS	= NACJ NACE NARJ NARE NAEJ NAEE
NAME		= homeboy
RESDESC		= res.json

ADDRESS     = 0x90000800
ALL_CFLAGS      = -c -Iinclude -mcpu=750 -meabi -mhard-float -G 0 -O3 -ffunction-sections -fdata-sections $(CFLAGS)
ALL_CPPFLAGS	= $(CPPFLAGS)
ALL_LDFLAGS     = -T build.ld -G 0 -nostartfiles -specs=nosys.specs -Wl,--section-start,.init=$(ADDRESS) $(LDFLAGS)
ALL_OBJCOPYFLAGS	= -S -O binary --set-section-flags .bss=alloc,load,contents $(OBJCOPYFLAGS)

HOMEBOY			= $(foreach v,$(VC_VERSIONS),hb-$(v))

HB-NACJ		= $(COBJ-hb-NACJ) $(ELF-hb-NACJ)
HB-NACE		= $(COBJ-hb-NACE) $(ELF-hb-NACE)
HB-NARJ		= $(COBJ-hb-NARJ) $(ELF-hb-NARJ)
HB-NARE		= $(COBJ-hb-NARE) $(ELF-hb-NARE)
HB-NAEJ		= $(COBJ-hb-NAEJ) $(ELF-hb-NAEJ)
HB-NAEE		= $(COBJ-hb-NAEE) $(ELF-hb-NAEE)

all			: $(HOMEBOY)
clean       :
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY		: all clean

define bin_template
SRCDIR-$(1)      = src
OBJDIR-$(1)      = obj/$(1)
BINDIR-$(1)      = bin/$(1)
CSRC-$(1)       := $$(foreach s,$$(CFILES),$$(wildcard $$(SRCDIR-$(1))/$$(s)))
SSRC-$(1)		:= $$(foreach s,$$(SFILES),$$(wildcard $$(SRCDIR-$(1))/$$(s)))
COBJ-$(1)        = $$(patsubst $$(SRCDIR-$(1))/%,$$(OBJDIR-$(1))/%.o,$$(CSRC-$(1)))
SOBJ-$(1)		 = $$(patsubst $$(SRCDIR-$(1))/%,$$(OBJDIR-$(1))/%.o,$$(SSRC-$(1)))
ELF-$(1)         = $$(BINDIR-$(1))/$(2).elf
BIN-$(1)         = $$(BINDIR-$(1))/$(2).bin
OUTDIR-$(1)      = $$(OBJDIR-$(1)) $$(BINDIR-$(1))
BUILD-$(1)		 = $(1)
CLEAN-$(1)		 = clean-$(1)
$$(ELF-$(1))		 : LDFLAGS += -Wl,--defsym,init=$$(ADDRESS)
$$(BUILD-$(1))       : $$(BIN-$(1))
$$(CLEAN-$(1))       :
	rm -rf $$(OUTDIR-$(1))

$$(COBJ-$(1))     : $$(OBJDIR-$(1))/%.o: $$(SRCDIR-$(1))/% | $$(OBJDIR-$(1))
	$(CC) $$(ALL_CPPFLAGS) $$(ALL_CFLAGS) $$< -o $$@
$$(SOBJ-$(1))		: $$(OBJDIR-$(1))/%.o: $$(SRCDIR-$(1))/% | $$(OBJDIR-$(1))
	$(AS) -c -mregnames $$(ALL_CPPFLAGS) $$< -o $$@
$$(RESOBJ-$(1))		: $$(OBJDIR-$(1))/%.o: $$(RESDIR-$(1))/% | $$(OBJDIR-$(1))
	$(GRC) $$< -d $(RESDESC) -o $$@
$$(ELF-$(1))      : $$(COBJ-$(1)) $$(SOBJ-$(1)) | $$(BINDIR-$(1))
	$(LD) $$(ALL_LDFLAGS) $$^ -o $$@
$$(BIN-$(1))      : $$(ELF-$(1)) | $$(BINDIR-$(1))
	$(OBJCOPY) $$(ALL_OBJCOPYFLAGS) $$< $$@
$$(OUTDIR-$(1))   :
	mkdir -p $$@
endef

$(foreach v,$(VC_VERSIONS),$(eval $(call bin_template,hb-$(v),homeboy)))

$(HB-NACJ)  	: ALL_CPPFLAGS	+=	-DVC_VERSION=NACJ
$(HB-NACE)  	: ALL_CPPFLAGS	+=	-DVC_VERSION=NACE
$(HB-NARJ)  	: ALL_CPPFLAGS	+=	-DVC_VERSION=NARJ
$(HB-NARE)  	: ALL_CPPFLAGS	+=	-DVC_VERSION=NARE
$(HB-NAEJ)  	: ALL_CPPFLAGS	+=	-DVC_VERSION=NAEJ
$(HB-NAEE)  	: ALL_CPPFLAGS	+=	-DVC_VERSION=NAEE
