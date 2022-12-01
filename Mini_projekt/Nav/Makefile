# PIC32 device number
DEVICE		= 32MX340F512H

# UART settings for programmer
TTYDEV		?=/dev/ttyUSB0
TTYBAUD		?=115200

# Name of the project
PROGNAME	= outfile

# Includes
INCLUDE = -I/Users/saradanielsson/IS1500HT22/miniprojekt-dtek/Mini_projekt/pic32/compiler/pic32-tools/pic32mx/include -I/Users/saradanielsson/IS1500HT22/miniprojekt-dtek/Mini_projekt/pic32/compiler/pic32-tools/pic32mx/include/machine -I/Users/saradanielsson/IS1500HT22/miniprojekt-dtek/Mini_projekt/pic32/cores/pic32 -I/Users/saradanielsson/IS1500HT22/miniprojekt-dtek/Mini_projekt/pic32/variants/uC32

# Linkscript
LINKSCRIPT	:= p$(shell echo "$(DEVICE)" | tr '[:upper:]' '[:lower:]').ld

# Compiler and linker flags
CFLAGS		+=  -ffreestanding -march=mips32r2 -msoft-float -Wa,-msoft-float $(INCLUDE)
ASFLAGS		+= -msoft-float
LDFLAGS		+= -T $(LINKSCRIPT)


# Filenames
ELFFILE		= $(PROGNAME).elf
HEXFILE		= $(PROGNAME).hex

# Find all source files automatically
CFILES          = $(wildcard *.c )
CXXFILES          = $(wildcard *.cpp )
ASFILES         = $(wildcard *.S)
SYMSFILES	= $(wildcard *.syms)

# Object file names
OBJFILES        = $(CFILES:.c=.c.o)
OBJFILES        +=$(CXXFILES:.cpp=.cpp.o)
OBJFILES        +=$(ASFILES:.S=.S.o)
OBJFILES	+=$(SYMSFILES:.syms=.syms.o)

# Hidden directory for dependency files
DEPDIR = .deps
df = $(DEPDIR)/$(*F)

.PHONY: all clean install envcheck
.SUFFIXES:

all: $(HEXFILE)

clean:
	$(RM) $(HEXFILE) $(ELFFILE) $(OBJFILES)
	$(RM) -R $(DEPDIR)

envcheck:
	@echo "$(TARGET)" | grep mcb32 > /dev/null || (\
		echo ""; \
		echo " **************************************************************"; \
		echo " * Make sure you have sourced the cross compiling environment *"; \
		echo " * Do this by issuing:                                        *"; \
		echo " * . /path/to/crosscompiler/environment                       *"; \
		echo " **************************************************************"; \
		echo ""; \
		exit 1)

install: envcheck
	$(TARGET)avrdude -v -p $(shell echo "$(DEVICE)" | tr '[:lower:]' '[:upper:]') -c stk500v2 -P "$(TTYDEV)" -b $(TTYBAUD) -U "flash:w:$(HEXFILE)"

$(ELFFILE): $(OBJFILES) envcheck
	$(CXX) $(CFLAGS) -o $@ $(OBJFILES) $(LDFLAGS)

$(HEXFILE): $(ELFFILE) envcheck
	$(TARGET)bin2hex -a $(ELFFILE)

$(DEPDIR):
	@mkdir -p $@

# Compile C files
%.c.o: %.c envcheck | $(DEPDIR)
	$(CXX) $(CFLAGS) -c -MD -o $@ $<
	@cp $*.c.d $(df).c.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.c.d >> $(df).c.P; $(RM) $*.c.d

# Compile CPP files
%.cpp.o: %.cpp envcheck | $(DEPDIR)
	$(CXX) $(CFLAGS) -c -MD -o $@ $<
	@cp $*.cpp.d $(df).cpp.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.cpp.d >> $(df).cpp.P; $(RM) $*.cpp.d

# Compile ASM files with C pre-processor directives
%.S.o: %.S envcheck | $(DEPDIR)
	$(CXX) $(CFLAGS) $(ASFLAGS) -c -MD -o $@ $<
	@cp $*.S.d $(df).S.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.S.d >> $(df).S.P; $(RM) $*.S.d

# Link symbol lists to object files
%.syms.o: %.syms
	$(LD) -o $@ -r --just-symbols=$<

# Check dependencies
-include $(CXXFILES:%.cpp=$(DEPDIR)/%.cpp.P)
-include $(CFILES:%.c=$(DEPDIR)/%.c.P)
-include $(ASFILES:%.S=$(DEPDIR)/%.S.P)
