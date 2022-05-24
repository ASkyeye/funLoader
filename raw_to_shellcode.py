#!/bin/python
import sys

shellcode = "\""
for bytes in open(sys.argv[1], "rb").read():
	shellcode += "\\x" + bytes.encode("hex")
shellcode += "\""
print shellcode
