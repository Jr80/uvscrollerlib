
/*
    Mike.X. Ultra VScroll Lib 1.3
    ----------------------------------

    Name    : Ultra VScroll Lib
    Author  : Mike.X.
    Website : http://mikex.0fees.net
    Blog... : http://themikex.blogspot.com
    Email : mayurcmn@gmail.com

    Creation : 20 Nov , 2011
    Last Updated : 23 Sept. , 2012

    Resource required :
    1.one xm tune ( ufmodlib required ) {optional}
    2.one bitmap image background skin  {optional}
    3.region file for background skin   {optional}
    NOTE : libs are required to be linked : ufmod , winmm , uvscroller

    =--=--=--=--=--=
    Example Program
    =--=--=--=--=--=
*/

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "libs/UVScroller.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "libs/ufmod.lib")

#include <windows.h>
#include <mmsystem.h>
#include "libs/ufmod.h"
#include "libs/uvscroll.h"
#include "resource.h"

char TEXT[] = TEXT("About\n--------\nUVScroller Lib v1.3\n\nUltra Vertical\nScroller Lib v1.3\n\nA small library to\ndo what you're \nseeing :p\n\nRightClick To Exit\nThanks !");

HICON hicon;
HFONT sfont;
HCURSOR hcur;
HWND ht;

BOOL CALLBACK YourDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_INITDIALOG:
    {
        //load cursor
        hcur = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CUR));
    }
    break;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {

        case IDC_BUTTON1:

            //init uv scroller lib
            UV_Init(hwnd);

            //create comic sans ms font
            sfont = UV_CreateFont("comic sans ms",10,700);

            //width,height,font-color,bg-color,font-handle,font-size,text
            UV_SetupScroller(200,300,RGB(0,0,150),RGB(255,255,200),sfont,10,TEXT);

            //resources
            UV_SetResources(NULL,NULL,ID_XM2);

            //some extras
            UV_Extra(15,0,0,DT_CENTER);
            UV_Border(3,RGB(10,10,10));

            DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG1),hwnd,(DLGPROC)UV_AboutDialogProc,0);
            break;

        case IDC_BUTTON2:

            UV_Init(hwnd);

            //create new font
            sfont = UV_CreateFont("verdana",12,700);

            //width,height,font-color,bg-color,font-handle,font-size,text
            UV_SetupScroller(300,388,RGB(15,15,253),NULL,sfont,12,TEXT);

            //resources
            UV_SetResources(IDB_ABOUT_SKIN,ID_RGN,ID_XM1);

            UV_Extra(20,20,10,DT_CENTER);

            DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG1),hwnd,(DLGPROC)UV_AboutDialogProc,0);
            break;

        case IDC_BUTTON3:

            UV_Init(hwnd);

            //create new font
            sfont = UV_CreateFont("arial",12,700);

            //width,height,font-color,bg-color,font-handle,font-size,text
            UV_SetupScroller(236,338,RGB(0,200,0),NULL,sfont,12,TEXT);

            //resources
            UV_SetResources(IDB_ABOUT_SKIN2,NULL,ID_XM3);

            //extra
            UV_Fading(false,200);
            UV_Extra(10,0,0,DT_CENTER);

            DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG1),hwnd,(DLGPROC)UV_AboutDialogProc,0);
            break;

        case IDC_BUTTON4:

            UV_Init(hwnd);

            //create new font
            sfont = UV_CreateFont("arial",10,700);

            //width,height,font-color,bg-color,font-handle,font-size,text
            UV_SetupScroller(163,212,RGB(150,150,150),RGB(0,0,0),sfont,10,TEXT);

            //resources
            UV_SetResources(NULL,ID_RGN3,ID_XM4);

            //extra
            UV_Border(5,RGB(150,150,150),3);
            UV_Extra(1,0,-2,DT_CENTER);

            DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG1),hwnd,(DLGPROC)UV_AboutDialogProc,0);
            break;


        case IDC_BUTTON6:

            UV_Init(hwnd);

            //create new font
            sfont = UV_CreateFont("arial",10,700);

            //width,height,font-color,bg-color,font-handle,font-size,text
            UV_SetupScroller(0,0,RGB(150,150,150),RGB(0,0,0),sfont,10,TEXT);

            //resources
            UV_SetResources(NULL,NULL,ID_XM3);

            //extra
            UV_SetCursor(hcur);
            UV_CanMove(false);
            UV_Extra(5,0,-2,DT_CENTER);

            DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG1),hwnd,(DLGPROC)UV_AboutDialogProc,0);
            break;
        }
    }
    break;

    case WM_CLOSE:
        EndDialog(hwnd,0);
        break;
    }

    return 0;
}

// Win Main
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    hicon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON));
    DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG2),NULL,(DLGPROC)YourDialogProc,0);
    return 0;
}
