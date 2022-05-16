# funLoader

releasing this because i need something for my resume lol. will be pushing more interesting stuff soon so you can follow me on here and keep an eye out.


this is a shellcode loader using anti-vm techniques (gonna add anti-debug soon, getting too many detections with current techniques) + direct syscalls + 1 byte xor for EDR / AV evasion. Will make updates to it as I go on, but don't expect it to stay undetected on runtime for very long. Will eventually comment code properly and write some mini-documentation. 

build as x64 only cuz syscalls are x64 (duh.), but you can change to x86 just replace x64 structs with x86. I got mine from syswhispers (as you can see from sysopen.*). 


dm me on twitter (@mookyfun) if you have questions or want to show me something cool that you learned.
have fun with this project and make some modifications, go learn how to beat different types of detections!
# antiscan.me result (scantime)



![scan result](https://antiscan.me/images/result/QJbJ350dTPiu.png)
