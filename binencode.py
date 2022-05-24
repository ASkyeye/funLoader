#!/bin/python
# https://github.com/mookyfun
import sys

key_list = [0xA7, 0xC2, 0xC6]

def xor(data, key):
	return bytes(bytearray(((data[i] ^ key for i in range(0,len(data))))))
 
with open(sys.argv[1], "rb") as scFile:
    shellcodeBytes = bytearray(scFile.read())
cShellcode = shellcodeBytes

for each_key in key_list:
    cShellcode = xor(cShellcode, each_key)

with open(sys.argv[2], "wb") as scFile:
    scFile.write(cShellcode)
