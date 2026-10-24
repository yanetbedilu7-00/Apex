#include <windows.h>
#include <stdlib.h>

int main() {
    // Triggers the Windows Camera app (Requires user permission)
    system("start microsoft.windows.camera:");
    
    // Wait 3 seconds for the camera to initialize
    Sleep(3000);
    
    // Take a screenshot as proof it launched
    system("powershell.exe -WindowStyle Hidden -Command \"Add-Type -AssemblyName System.Windows.Forms; Add-Type -AssemblyName System.Drawing; $b = New-Object Drawing.Bitmap([Windows.Forms.Screen]::PrimaryScreen.Bounds.Width, [Windows.Forms.Screen]::PrimaryScreen.Bounds.Height); $g = [Drawing.Graphics]::FromImage($b); $g.CopyFromScreen(0,0,0,0,$b.Size); $b.Save(\\\"$env:USERPROFILE\\\\Desktop\\\\camera_proof.png\\\")\"");
    
    return 0;
}
