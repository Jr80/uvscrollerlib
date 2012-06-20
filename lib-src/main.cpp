
/*
    Mike.X.'s UVScroll Lib 1.2
    -------------------------------------------------------
    
    Name    : Mike.X.'s Ultra Vertical Text Scrolling Library 1.2
    Author  : Mike.X.
    Website : http://mikex.0fees.net
    Blog... : http://themikex.blogspot.com
    Creation : 20 Nov , 2011

    Resource required :
    1.one xm tune ( ufmodlib required ) {optional}
    2.one bitmap image background skin  {optional}
    3.region file for background skin   {optional}
    NOTE : libs are required to be linked : ufmod , winmm , uvscroll
    
*/

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <ufmod.h>
#include <mmsystem.h>
#include "resource.h"

void UV_Init(HWND hwnd)
{
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
    UV.IDB_ABOUT_SKIN = bitmap;
    UV.ID_RGN = rgn;
    UV.ID_XM = xm;
}

void UV_UseResources(bool bitmap,bool rgn,bool xm)
{
    UV.USE_BITMAP = bitmap;
    UV.USE_RGN = rgn;
    UV.PLAY_TUNE = xm;
}

void UV_Extra(int SPEED,int RIGHTSIDE,int DELAY)
{
    UV.WX = RIGHTSIDE;
    UV.DELAY = DELAY;
    UV.SPEED = SPEED;
}

void _UV_die(HWND hwnd)
{
    if(UV.Ccur)
    {
        SetCursor(UV.hCur);
    }

    if(UV.Fading)
    {
        __UV__FadeOut(hwnd,UV.fade_end);
    }

    if(UV.PLAY_TUNE == true)
    {
        uFMOD_StopSong();
    }

    EndDialog(hwnd,0);
}

//Main Dialog Procedure
BOOL CALLBACK UV_AboutDialogProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

    case WM_INITDIALOG:
    {
        if(UV.BW == 0 || UV.BH == 0)
        {
            MessageBox(NULL,"Please use UVSetupScroller(); to setup scroller\nSee uvscroll.h file\nMike.X.'s UVScroll Lib\nProgram is Exiting..\nhttp://themikex.blogspot.com","Error",MB_OK);
            ExitProcess(0);
        }

        //load  about skin
        if (UV.USE_BITMAP == true)
        {
            UV.hskin = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(UV.IDB_ABOUT_SKIN));
            HDC hdcScreen = GetDC(HWND_DESKTOP);
            UV.hdcskin = CreateCompatibleDC(hdcScreen);
            UV.ndcskin = SaveDC(UV.hdcskin);
            SelectObject(UV.hdcskin, UV.hskin);
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

        SetWindowPos(hwnd,0,0,0,UV.BW,UV.BH,SWP_NOZORDER);
        __UV__CenterWindow(hwnd,0,0);

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
        }
    }
    break;

    case WM_TIMER:
    {
        if(UV.Ccur)
        {
            SetCursor(UV.hCur);
        }

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

            if(UV.USE_BITMAP == true)
            {
                UV.ndcMem = SaveDC(hdcMem);
                //copy skin to hdcmem
                BitBlt(hdcMem, 0, 0, UV.BW, UV.BH, UV.hdcskin, 0, 0, SRCCOPY);
            }
            else
            {
                if(UV.BORDER)
                {
                    RECT brcrect = { 0, 0, UV.BW+UV.BORDER_SIZE, UV.BH+UV.BORDER_SIZE };
                    FillRect(hdcMem, &brcrect,(HBRUSH) CreateSolidBrush(UV.BORDER_COLOR));

                    if(UV.USE_RGN)
                    {
                        RECT rcrect = { UV.BORDER_SIZE, UV.BORDER_SIZE, UV.BW-(UV.BORDER_SIZE+UV.RBORDER) , UV.BH-UV.BORDER_SIZE };
                        FillRect(hdcMem, &rcrect,(HBRUSH) CreateSolidBrush(UV.BACK_COLOR));
                    }
                    else
                    {
                        RECT rcrect = { UV.BORDER_SIZE, UV.BORDER_SIZE, UV.BW-UV.BORDER_SIZE, UV.BH-UV.BORDER_SIZE };
                        FillRect(hdcMem, &rcrect,(HBRUSH) CreateSolidBrush(UV.BACK_COLOR));
                    }
                }
                else
                {
                    RECT rcrect = { 0, 0, UV.BW, UV.BH };
                    FillRect(hdcMem, &rcrect,(HBRUSH) CreateSolidBrush(UV.BACK_COLOR));
                }
            }

            // Draw text into double buffer
            __UV__DrawScrollText(hdcMem, UV.STEXT,UV.sfont,UV.FONT_COLOR,UV.WX,UV.WY, UV.BW, UV.BH);

            // Copy double buffer to screen
            BitBlt(ps.hdc, 0, 0, UV.BW, UV.BH, hdcMem, 0, 0, SRCCOPY);

            // Clean up
            if (UV.USE_BITMAP == false)
            {
                RestoreDC(hdcMem, UV.ndcMem);
            }

            DeleteObject(hbmMem);
            DeleteDC(hdcMem);
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
        if(UV.Ccur)
        {
            SetCursor(UV.hCur);
        }

        SetCapture( hwnd );
        GetWindowRect(hwnd, &UV.MainRect);
        //save current cursor coordinate
        GetCursorPos(&UV.point);
        ScreenToClient(hwnd, &UV.point);
        break;

    case WM_LBUTTONUP:
        if(UV.Ccur)
        {
            SetCursor(UV.hCur);
        }

        ReleaseCapture();
        break;

    case WM_MOUSEMOVE:
    {
        if(UV.Ccur)
        {
            SetCursor(UV.hCur);
        }

        GetCursorPos(&UV.curpoint);
        if(wParam==MK_LBUTTON)
        {
            MoveWindow(hwnd, UV.curpoint.x - UV.point.x, UV.curpoint.y - UV.point.y,
                       UV.MainRect.right - UV.MainRect.left, UV.MainRect.bottom - UV.MainRect.top,TRUE);
        }
    }
    break;

    case WM_MBUTTONDBLCLK:
    {
        if(UV.Ccur)
        {
            SetCursor(UV.hCur);
        }
    }
    break;

    case WM_LBUTTONDBLCLK:
    {
        if(UV.Ccur)
        {
            SetCursor(UV.hCur);
        }
    }
    break;

    case WM_RBUTTONDBLCLK:
    {
        if(UV.Ccur)
        {
            SetCursor(UV.hCur);
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
