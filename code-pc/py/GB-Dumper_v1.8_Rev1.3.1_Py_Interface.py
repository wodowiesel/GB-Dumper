# GBCartRead - Arduino Interface Version: 1.8
# orig. Author: Alex from insideGadgets (http://www.insidegadgets.com)
# Created: 18/03/2011, Last Modified: 21/03/2016
# GB-Dumperv 1.8 Rev1.3.2 by wodowiesel
# Optimized: 21/10/2024

import os
import sys
import time
import string
import serial
import atexit
import signal

#sys.stdout.write('\nGB-Dumperv1.8 Rev1.3.2 by wodowiesel\n')
#sys.stdout.write('###################################\n')
print('\nGB-Dumper v1.8 Rev1.3.2 by wodowiesel\n')
print('###################################\n')
sys.stdout.flush()

# Change COM to the port the Arduino is on.
# You can lower the baud rate of 400kBit if you have issues connecting to the Arduino or the ROM has checksum errors
port = 'COM3'
baudrate = 19200 #9600 #57600
print('Serial connection on: '+port+' with baudrate', baudrate)
#16 lines = 1Kb and every 1Kb a # is written in before output
time.sleep(1)
waitInput = 1
userInput = '0'
data = bytes(b'')

try:
    ser = serial.Serial(port, baudrate, timeout=1) # /dev/ttyACM0 (old) or ttyS0 (newer via usb) for linux-based systems
except:
    print('\nSerial error')
    exit()

while (waitInput == 1):
    #sys.stdout.write('\nSelect an option below\n0. Read Header\n1. Dump ROM\n2. Save RAM\n3. Write RAM\n4. Exit\n')
    print('\nSelect an option below\n0. Read Header\n1. Dump ROM\n2. Save RAM\n3. Write RAM\n4. Exit\n')
    #sys.stdout.write('>')
    print('> ')
    sys.stdout.flush()
    userInput = input()

    if (userInput == '0'):
        ser.write('HEADER'.encode()) 
        #standard is utf_8 if nothig is specified or use option encoding="ascii", same for decode
        #https://docs.python.org/3/library/codecs.html#standard-encodings
        #ser.write(b'0') #not sure if needed? probably not

        gameTitle = ascii(ser.readline())
        gameTitle = gameTitle[2:(len(gameTitle)-5)]
       #sys.stdout.write('\ngametitle: ')
        print('\nGametitle: ')
        print (gameTitle)
        if (gameTitle != None):
            print (gameTitle)
        else:
            print ('not found or none, using "unknown"\n')
            gameTitle = 'unknown'

        cartridgeType = ascii(ser.readline())
        cartridgeType = int(cartridgeType[2:(len(cartridgeType)-5)])
        print ('\nCartridge type: '+cartridgeType+'\n')
        #sys.stdout.write('MBC type: ')
        print('MBC type: ')
        if (cartridgeType == 0):
            print ('ROM ONLY\n')
        elif (cartridgeType == 1):
            print ('MBC1\n')
        elif (cartridgeType == 2):
            print ('MBC1+RAM\n')
        elif (cartridgeType == 3):
            print ('MBC1+RAM+BATTERY\n')
        elif (cartridgeType == 5):
            print ('MBC2\n')
        elif (cartridgeType == 6):
            print ('MBC2+BATTERY\n')
        elif (cartridgeType == 8):
            print ('ROM+RAM\n')
        elif (cartridgeType == 9):
            print ('ROM+RAM+BATTERY\n')
        elif (cartridgeType == 11):
            print ('MMM01\n')
        elif (cartridgeType == 12):
            print ('MMM01+RAM\n')
        elif (cartridgeType == 13):
            print ('MMM01+RAM+BATTERY\n')
        elif (cartridgeType == 15):
            print ('MBC3+TIMER+BATTERY\n')
        elif (cartridgeType == 16):
            print ('MBC3+TIMER+RAM+BATTERY\n')
        elif (cartridgeType == 17):
            print ('MBC3\n')
        elif (cartridgeType == 18):
            print ('MBC3+RAM\n')
        elif (cartridgeType == 19):
            print ('MBC3+RAM+BATTERY\n')
        elif (cartridgeType == 21):
            print ('MBC4\n')
        elif (cartridgeType == 22):
            print ('MBC4+RAM')
        elif (cartridgeType == 23):
            print ('MBC4+RAM+BATTERY\n')
        elif (cartridgeType == 25):
            print ('MBC5\n')
        elif (cartridgeType == 26):
            print ('MBC5+RAM\n')
        elif (cartridgeType == 27):
            print ('MBC5+RAM+BATTERY\n')
        elif (cartridgeType == 28):
            print ('MBC5+RUMBLE\n')
        elif (cartridgeType == 29):
            print ('MBC5+RUMBLE+RAM\n')
        elif (cartridgeType == 30):
            print ('MBC5+RUMBLE+RAM+BATTERY\n')
        elif (cartridgeType == 99):
            print ('WISDOM TREE MAPPER\n')
        elif (cartridgeType == 252):
            print ('Gameboy Camera\n')
        else:
            print('not found or unknown\n')

        romSize = ascii(ser.readline())
        romSize = int(romSize[2:(len(romSize)-5)])
        print ('ROM type: '+romSize+'\n')
        #sys.stdout.write('ROM size: ')
        print('ROM size: ')
        if (romSize == 0):
            print ('32 KByte (no ROM banking)\n')
        elif (romSize == 1):
            print ('64 KByte (4 banks)\n')
        elif (romSize == 2):
            print ('128 KByte (8 banks)\n')
        elif (romSize == 3):
            print ('256 KByte (16 banks)\n')
        elif (romSize == 4):
            print ('512 KByte (32 banks)\n')
        elif (romSize == 5 and (cartridgeType == 1 or cartridgeType == 2 or cartridgeType == 3)):
            print ('1 MByte (63 banks)\n')
        elif (romSize == 5):
            print ('1 MByte (64 banks)\n')
        elif (romSize == 6 and (cartridgeType == 1 or cartridgeType == 2 or cartridgeType == 3)):
            print ('2 MByte (125 banks)\n')
        elif (romSize == 6):
            print ('2 MByte (128 banks)\n')
        elif (romSize == 7):
            print ('4 MByte (256 banks)\n')
        elif (romSize == 82):
            print ('1.1 MByte (72 banks)\n')
        elif (romSize == 83):
            print ('1.2 MByte (80 banks)\n')
        elif (romSize == 84):
            print ('1.5 MByte (96 banks)\n')
        else:
            print('not found or unknown\n')

        ramSize = ascii(ser.readline())
        ramSize = int(ramSize[2:(len(ramSize)-5)])
        print ('RAM type: '+ramSize+'\n')
        #sys.stdout.write('RAM size: ')
        print('RAM size: ')
        if (ramSize == 0 and cartridgeType == 6):
            print ('512 bytes (nibbles)\n')
        elif (ramSize == 0):
            print ('0 None\n')
        elif (ramSize == 1):
            print ('2 KBytes\n')
        elif (ramSize == 2):
            print ('8 KBytes\n')
        elif (ramSize == 3):
            print ('32 KBytes (4 banks of 8 Kbytes)\n')
        elif (ramSize == 4):
            print ('128 KBytes (16 banks of 8 Kbytes)\n')
        else:
            print('not found or unknown\n')

        #sys.stdout.write('Logo Check: ')
        print('Logo Check: ')
        logoCheck = ascii(ser.readline())
        logoCheck = int(logoCheck[2:(len(logoCheck)-5)])
        #print (logoCheck)
        if (logoCheck == 1):
            print ('1 OK\n')
        elif (logoCheck == 0):
            print ('0 Failed\n')
        else:
            print('not found or unknown\n')

    elif (userInput == '1'):
        #sys.stdout.write('\nDumping ROM to ' + gameTitle + '.gb')
        print('\nDumping ROM (game) to ' + gameTitle + '.gb\n')
        readBytes = 0
        inRead = 1
        Kbytesread = 0
        ser.write('READROM'.encode())
        f = open(gameTitle+'.gb', 'wb')
        while 1:
            if inRead == 1:
                line = ser.read(64) # note sure if 64 for gba
                print(line.hex())
                if len(line) == 0:
                    break
                readBytes += 64
                f.write(line)
            if readBytes % 1024 == 0 and readBytes != 0:
                #sys.stdout.write('#')
                print('#')
                sys.stdout.flush()
            if readBytes % 32768 == 0 and readBytes != 0:
                Kbytesread = Kbytesread + 1
                Kbytesprint = Kbytesread * 32
                #sys.stdout.write('%sK' % Kbytesprint)
                print('%sK' % Kbytesprint)
                sys.stdout.flush()
        #maybe put  flush back here?
        f.close()
        #sys.stdout.write('\nFinished\n')
        print('\nFinished\n')
        sys.stdout.flush()

    elif (userInput == '2'):
       #sys.stdout.write('\nDumping RAM to ' + gameTitle + '.sav\n')
        print('\nDumping RAM (save) to ' + gameTitle + '.sav\n')
        readBytes = 0
        inRead = 1
        Kbytesread = 0
        ser.write('READRAM'.encode())
        f = open(gameTitle+'.sav', 'wb')
        while 1:
            if inRead == 1:
                line = ser.read(64)
                print(line.hex())
                if len(line) == 0:
                    break
                readBytes += 64
                f.write(line)
            if readBytes % 256 == 0 and readBytes != 0:
                #sys.stdout.write('#')
                print('#')
                sys.stdout.flush()
            if readBytes % 1024 == 0 and readBytes != 0:
                Kbytesread = Kbytesread + 1
                #sys.stdout.write('%sK' % Kbytesread)
                print('%sK' % Kbytesread)
                sys.stdout.flush()
        f.close()
        #sys.stdout.write('\nFinished\n')
        print('\nFinished\n')
        sys.stdout.flush()

    elif (userInput == '3'):
        #sys.stdout.write('\nGoing to write to RAM from ' + gameTitle + '.sav\n')
        #sys.stdout.write('*** This will erase the save game from your Gameboy Cartridge ***\n')
        #sys.stdout.write('Press y to continue or any other key to abort\n')

        print('\nGoing to write to RAM (save) from ' + gameTitle + '.sav\n')
        print('*** This will erase the save game from your Gameboy Cartridge ***\n')
        print('Press y to continue or any other key to abort\n')
        userInput2 = input()

        if (userInput2 == 'y'):
            #sys.stdout.write('\nWriting to RAM from ' + gameTitle + '.sav')
            print('\nWriting to RAM from ' + gameTitle + '.sav\n')
            fileExists = 1
            doExit = 0
            printHash = 0
            Kbyteswrite = 0
            try:
                f = open(gameTitle+'.sav', 'rb')
            except IOError:
                #sys.stdout.write('\nNo save file found, aborted\n')
                print('\nNo save file found, aborted\n')
                fileExists = 0
            if (fileExists == 1):
                ser.write('WRITERAM'.encode())
                time.sleep(1); # Wait for Arduino to setup
                while 1:
                    if printHash % 4 == 0 and printHash != 0: # 256 / 64 = 4
                        #sys.stdout.write('#')
                        print('#')
                        sys.stdout.flush()
                    if printHash % 16 == 0 and printHash != 0:
                        Kbyteswrite = Kbyteswrite + 1
                        #sys.stdout.write('%sK' % Kbyteswrite)
                        print('%sK' % Kbyteswrite)
                        sys.stdout.flush()
                    printHash += 1

                    line1 = f.read(64) # Read 64bytes of save file
                    if not line1:
                        break
                    ser.write(line1)
                    time.sleep (0.005); # Wait 5ms for Arduino to process the 64 bytes

            f.close()
            #sys.stdout.write('\nFinished\n')
            print('\nFinished\n')
            sys.stdout.flush()

        else:
            #sys.stdout.write('\nAborted! binary decode....\n')
            print('\nAborted! binary decode....\n')
            data += byte
            dc = data.decode()
            print(dc)
            sys.stdout.flush()

    elif (userInput == '4'):
        waitInput = 0

    else:
        print('\nOption not recognised, please try again\n')
        #ys.stdout.write('\nOption not recognised, please try again\n')

print('\nSerial closed \n')
ser.close()
print('\nExiting.... \n')
exit()

#EOF
