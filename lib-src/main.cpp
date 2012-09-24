
/*
    Mike.X. Ultra VScroll Lib 1.3
    ------------------------------

    Name    : Ultra VScroll Lib
    Author  : Mike.X.
    Website : http://mikex.0fees.net
    Craetion : 20 Nov , 2011
    Last Updated : 23 Sept. , 2012

    Resource required :
    1.one xm tune ( ufmodlib required ) {optional}
    2.one bitmap image background skin  {optional}
    3.region file for background skin   {optional}
*/

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <ufmod.h>
#include <mmsystem.h>
#include "lol.h"

void UV_Init(HWND hwnd)
{
    //dont know which works
    RtlZeroMemory(&UV,sizeof(UVSCROLLER_STRUCT));
    RtlZeroMemory(&UV,sizeof(UV));
    UV = {};

    if(hwnd)
    {
        UV.Parent_hwnd = true;
        UV.hParenthwnd = hwnd;
    }

    UV.SPEED = 25;
    UV.BORDER = false;
    UV.Fading=true;
    UV.fade_start=15;
    UV.fade_end=15;
    UV.trans=230;
    UV.Ccur=false;
    UV.TEXT_ALIGNMENT = DT_LEFT;
    UV.FULL_SCREEN = false;
    UV.CAN_MOVE = true;

    UV.hdcskin = NULL;
    UV.ndcMem = 0;
    UV.ndcskin = 0;
    UV.hskin = NULL;
    UV.hCur = NULL;
    UV.Ccur = false;
    UV.MainRect = {0};
    UV.point = {0};
}

void UV_SetCursor(HCURSOR hcur)
{
    UV.Ccur = true;
    UV.hCur = hcur;
}

void UV_Border(int bs,COLORREF cf,int rb)
{
    UV.BORDER = true;
    UV.BORDER_SIZE = bs;
    UV.BORDER_COLOR = cf;
    UV.RBORDER = rb;
}

void UV_Fading(bool woot,int trans,int start,int end)
{
    if(woot)
    {
        UV.Fading = true;
        UV.fade_start = start;
        UV.fade_end = end;
    }
    else
    {
        UV.trans = trans;
        UV.Fading = false;
    }
}

HFONT UV_CreateFont(char *fx,int szx,int bold)
{
    HFONT hfx;
    HDC hdc;
    long lfHeight;
    hdc = GetDC(NULL);
    lfHeight = -MulDiv(szx, GetDeviceCaps(hdc, LOGPIXELSY), 72);
    ReleaseDC(NULL, hdc);
    hfx = CreateFont(lfHeight, 0, 0, 0, bold, 0, 0, 0, 0, 0, 0, 0, 0, fx);
    return hfx;
}

void UV_SetResources(int bitmap,int rgn,int xm)
{
    if(bitmap != NULL)
    {
        UV.IDB_ABOUT_SKIN = bitmap;
        UV.USE_BITMAP = true;
    }

    if(rgn != NULL)
    {
        UV.ID_RGN = rgn;
        UV.USE_RGN = true;
    }

    if(xm != NULL)
    {
        UV.ID_XM = xm;
        UV.PLAY_TUNE = true;
    }
}

void UV_CanMove(bool x)
{
    UV.CAN_MOVE = x;
}

void UV_UseResources(bool bitmap,bool rgn,bool xm)
{
    UV.USE_BITMAP = bitmap;
    UV.USE_RGN = rgn;
    UV.PLAY_TUNE = xm;
}

void UV_Extra(int SPEED,int RIGHTSIDE,int DELAY,UINT talign)
{
    UV.WX = RIGHTSIDE;
    UV.DELAY = DELAY;
    UV.SPEED = SPEED;
    UV.TEXT_ALIGNMENT = talign;
}

void _UV_die(HWND hwnd)
{
    if(UV.Ccur)
    {
        HCURSOR hc = LoadCursor(NULL, IDC_ARROW);
        SetClassLong(hwnd, GCL_HCURSOR, (DWORD)hc);
        DeleteObject(UV.hCur);
    }

    if(UV.USE_BITMAP)
    {
        DeleteObject(UV.hskin);
    }

    if(UV.Fading)
    {
        __UV__FadeOut(hwnd,UV.fade_end);
    }

    if(UV.PLAY_TUNE == true)
    {
        uFMOD_StopSong();
    }

    //dont know which works
    RtlZeroMemory(&UV,sizeof(UVSCROLLER_STRUCT));
    RtlZeroMemory(&UV,sizeof(UV));
    UV = {};

    EndDialog(hwnd,0);
}

//Main Dialog Procedure
BOOL CALLBACK UV_AboutDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_INITDIALOG:
    {
        char sign[] = "mayur chauhan aka mike.x.";

        //load  about skin
        if (UV.USE_BITMAP == true)
        {
            UV.hskin = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(UV.IDB_ABOUT_SKIN));
            HDC hdcScreen = GetDC(HWND_DESKTOP);
            UV.hdcskin = CreateCompatibleDC(hdcScreen);
            UV.ndcskin = SaveDC(UV.hdcskin);
            SelectObject(UV.hdcskin, UV.hskin);
        }
        else
        {
            HDC hdcScreen = GetDC(HWND_DESKTOP);
            UV.hdcskin = CreateCompatibleDC(hdcScreen);
            HBITMAP hbmx = CreateCompatibleBitmap(hdcScreen, UV.BW, UV.BH);
            UV.ndcskin = SaveDC(UV.hdcskin);
            SelectObject(UV.hdcskin, hbmx);

            if(UV.BORDER)
            {
                RECT brcrect = { 0, 0, UV.BW+UV.BORDER_SIZE, UV.BH+UV.BORDER_SIZE };
                FillRect(UV.hdcskin, &brcrect,(HBRUSH) CreateSolidBrush(UV.BORDER_COLOR));

                if(UV.USE_RGN)
                {
                    RECT rcrect = { UV.BORDER_SIZE, UV.BORDER_SIZE, UV.BW-(UV.BORDER_SIZE+UV.RBORDER) , UV.BH-UV.BORDER_SIZE };
                    FillRect(UV.hdcskin, &rcrect,(HBRUSH) CreateSolidBrush(UV.BACK_COLOR));
                }
                else
                {
                    RECT rcrect = { UV.BORDER_SIZE, UV.BORDER_SIZE, UV.BW-UV.BORDER_SIZE, UV.BH-UV.BORDER_SIZE };
                    FillRect(UV.hdcskin, &rcrect,(HBRUSH) CreateSolidBrush(UV.BACK_COLOR));
                }
            }
        }

        if(UV.PLAY_TUNE == true)
        {
            uFMOD_PlaySong((char*)UV.ID_XM, 0, XM_RESOURCE);
        }

        if(UV.USE_RGN == true)
        {
            __UV__SetRegion(hwnd,UV.ID_RGN);
        }

        SetWindowText(hwnd,"About");
        UV.lines = __UV__CountLines(UV.STEXT);

        if( UV.FULL_SCREEN )
        {
            SetWindowPos(hwnd,HWND_TOPMOST ,0,0,UV.BW,UV.BH,SWP_FRAMECHANGED);
        }
        else
        {
            SetWindowPos(hwnd,0,0,0,UV.BW,UV.BH,SWP_NOZORDER);
            __UV__CenterWindow(hwnd,0,0);
        }

        if(UV.Fading)
        {
            __UV__FadeIn(hwnd,UV.fade_start);
        }
        else
        {
            _UV_SetTransparency(hwnd,UV.trans);
        }

        SetTimer(hwnd,1,UV.SPEED,NULL);

        if(UV.Ccur)
        {
            SetCursor(UV.hCur);
            SetClassLong(hwnd, GCL_HCURSOR, (DWORD)UV.hCur);
        }
        else
        {
            HCURSOR hc = LoadCursor(NULL, IDC_ARROW);
            SetClassLong(hwnd, GCL_HCURSOR, (DWORD)hc);
        }
    }
    break;

    case WM_SETCURSOR:
    {
        if(UV.Ccur)
        {
            SetCursor(UV.hCur);
            SetClassLong(hwnd, GCL_HCURSOR, (DWORD)UV.hCur);
        }
    }
    break;

    case WM_TIMER:
    {
        if(UV.WY == -(UV.lines+UV.DELAY)*UV.vh)
        {
            UV.WY = UV.BH + 10;
        }
        else
        {
            UV.WY -= 1;
        }

        InvalidateRect(hwnd, NULL, FALSE);
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        if(BeginPaint(hwnd, &ps))
        {
            HDC hdcMem = CreateCompatibleDC(ps.hdc);
            HBITMAP hbmMem = CreateCompatibleBitmap(ps.hdc, UV.BW, UV.BH);
            SelectObject(hdcMem,hbmMem);
            UV.ndcMem = SaveDC(hdcMem);

            //copy bg to double buffer
            BitBlt(hdcMem, 0, 0, UV.BW, UV.BH, UV.hdcskin, 0, 0, SRCCOPY);

            // Draw text into double buffer
            __UV__DrawScrollText(hdcMem, UV.STEXT,UV.sfont,UV.FONT_COLOR,UV.WX,UV.WY, UV.BW, UV.BH);

            // Copy double buffer to screen
            BitBlt(ps.hdc, 0, 0, UV.BW, UV.BH, hdcMem, 0, 0, SRCCOPY);

            RestoreDC(hdcMem, UV.ndcMem);
            DeleteObject(hbmMem);
            DeleteDC(hdcMem);
            DeleteDC(ps.hdc);
            EndPaint(hwnd, &ps);
        }
    }
    break;

    case WM_KEYDOWN:
    {
        _UV_die(hwnd);
    }
    break;

    case WM_LBUTTONDOWN:
        SetCapture( hwnd );
        GetWindowRect(hwnd, &UV.MainRect);
        //save current cursor coordinate
        GetCursorPos(&UV.point);
        ScreenToClient(hwnd, &UV.point);
        break;

    case WM_LBUTTONUP:
        ReleaseCapture();
        break;

    case WM_MOUSEMOVE:
    {
        GetCursorPos(&UV.curpoint);
        if(wParam==MK_LBUTTON)
        {
            if(UV.CAN_MOVE)
            {
                MoveWindow(hwnd, UV.curpoint.x - UV.point.x, UV.curpoint.y - UV.point.y,
                           UV.MainRect.right - UV.MainRect.left, UV.MainRect.bottom - UV.MainRect.top,TRUE);
            }
        }
    }
    break;

    case WM_MBUTTONDOWN:
    {
        _UV_die(hwnd);
    }
    break;

    case WM_RBUTTONDOWN:
    {
        _UV_die(hwnd);
    }
    break;

    case WM_CLOSE:
    {
        _UV_die(hwnd);
    }
    break;

    }
    return 0;
}
