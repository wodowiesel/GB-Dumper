# Command-line client
CMDLINE = GBCartRead_v1.8_Rev1.2_C_Interface.exe

# By default, build the firmware and command-line client
all: $(CMDLINE)

# One-liner to compile the command-line client
$(CMDLINE): GBCartRead_v1.8_Rev1.2_C_Interface.c
	gcc -O2 -Wall -lm -v -save-temps -g3 -ggdb3 -pg GBCartRead_v1.8_Rev1.2_C_Interface.c rs232.c -o GBCartRead_v1.8_Rev1.2_C_Interface.exe

# Housekeeping if you want it
clean:
	$(RM) *.exe
