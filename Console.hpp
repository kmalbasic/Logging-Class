#pragma once
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

using namespace std;

void create_console() // allocating a separate console window
{
    int console_handle;
    long std_handle;
    CONSOLE_SCREEN_BUFFER_INFO con_info;
    FILE* fp;

    AllocConsole(); // allocating console

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &con_info);
    con_info.dwSize.Y = 1500; // amount of lines our console window will show.
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), con_info.dwSize);

    // redirecting the input/output/errors to our separate window

    std_handle = (long)GetStdHandle(STD_OUTPUT_HANDLE); 
    console_handle = _open_osfhandle(std_handle, _O_TEXT);
    fp = _fdopen(console_handle, "w");

    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);
    std_handle = (long)GetStdHandle(STD_INPUT_HANDLE);
    console_handle = _open_osfhandle(std_handle, _O_TEXT);
    fp = _fdopen(console_handle, "r");
    *stdin = *fp;
    setvbuf(stdin, NULL, _IONBF, 0);


    std_handle = (long)GetStdHandle(STD_ERROR_HANDLE);
    console_handle = _open_osfhandle(std_handle, _O_TEXT);
    fp = _fdopen(console_handle, "w");

    *stderr = *fp;
    setvbuf(stderr, NULL, _IONBF, 0);

    // syncing the stdIO with our new console
    ios::sync_with_stdio();
}