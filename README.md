# funLoader

shellcode loader using antidbg techniques + direct syscalls + 1 byte xor for EDR / AV evasion. Will make updates to it as I go on, but don't expect it to stay undetected on runtime for very long. Will eventually comment code properly and write some mini-documentation. 

build as x64 only cuz syscalls are x64 (duh.), but you can change to x86 just replace x64 structs with x86. I got mine from syswhispers (as you can see from sysopen.*). 

have fun with this project and make some modifications, go learn how to beat different types of detections!
