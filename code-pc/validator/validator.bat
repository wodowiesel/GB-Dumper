:: start validator in terminal
@echo off
cd C:\github\GB-Dumper\code-pc\
set /p gameTitle="Input game title with extention [.gb/.gbc]: "
call xgbfix.exe -v -p -d %gameTitle%
::motorgbfix v1.0.0 (part of ASMotor 0.1.0)
::Usage: xgbfix [flags] gametitle[.gb]
::Options: Flags
::        -h              This text
::        -d              Debug: Don't change image
::        -p              Pad image to valid size
::                        Pads to 32/64/128/256/512kB as appropriate
::        -t<name>  Change cartridge title field (16 characters)
::        -v              Validate header
::                        Corrects: - Nintendo Character Area (0x0104)
::                                  - ROM type (0x0147)
::                                  - ROM size (0x0148)
::                                  - Checksums (0x014D-0x014F)
PAUSE