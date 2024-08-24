:: start validator in terminal
@echo off
cd C:\github\GB-Dumper\code-pc\
set /p gameTitle="Input game title with extention [.gb/.gbc/.gba]: "
call xgbfix.exe -v -d %gameTitle%
PAUSE
