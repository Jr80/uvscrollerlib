
struct UVSCROLLER_STRUCT
{
    RECT MainRect;
    POINT point;
    POINT curpoint;
    HFONT sfont;
    HBITMAP hskin;
    int WY,lines;
    int vh;

    HDC hdcskin;
    INT ndcskin;
    INT ndcMem;

    int BW;
    int BH;

    bool PLAY_TUNE;
    bool USE_BITMAP;
    bool USE_RGN;

    int IDB_ABOUT_SKIN;
    int ID_RGN;
    int ID_XM;

    int FONT_SIZE;

    COLORREF FONT_COLOR;
    COLORREF BACK_COLOR;

    char *STEXT;

    int DELAY;
    int WX;
    int SPEED;

    bool BORDER;
    int BORDER_SIZE;
    int RBORDER;
    COLORREF BORDER_COLOR;

    bool Fading;
    int fade_start;
    int fade_end;
    int trans;

    bool Ccur;
    HCURSOR hCur;

    bool Parent_hwnd;
    HWND hParenthwnd;
};

struct UVSCROLLER_STRUCT UV;

/// Functions /// /// Functions /// /// Functions /// /// Functions /// /// Functions ///
/// Functions /// /// Functions /// /// Functions /// /// Functions /// /// Functions ///
/// Functions /// /// Functions /// /// Functions /// /// Functions /// /// Functions ///

void UV_SetupScroller(
    int WINWIDTH,           //window/bitmap width
    int WINHEIGHT,          //window/bitmap height
    COLORREF _FONT_COLOR,   //font color
    COLORREF _BACK_COLOR,   //back color if bitmap is not used it'll be displayed
    HFONT hfont,            //font handle
    int FSIZE,              //font size
    char *_THETEXT)         //text that will gonna roll oh yeah ...
{
    UV.BW = WINWIDTH;
    UV.BH = WINHEIGHT;
    UV.FONT_COLOR = _FONT_COLOR;
    UV.BACK_COLOR = _BACK_COLOR;
    UV.sfont = hfont;
    UV.FONT_SIZE = FSIZE;
    UV.STEXT = _THETEXT;

    UV.WY = UV.BH+10;
    UV.vh = UV.FONT_SIZE + 10;
}

VOID __UV__DrawScrollText(HDC hdcDest, LPTSTR lpszText, HFONT hFont, COLORREF crColor,
                          INT nLeft, INT nTop, INT nRight, INT nBottom)
{
    RECT rcText = { nLeft, nTop, nRight, nBottom };
    SelectObject(hdcDest, hFont);
    SetBkColor(hdcDest,UV.BACK_COLOR);
    SetTextColor(hdcDest, crColor);
    SetBkMode(hdcDest, TRANSPARENT);
    DrawText(hdcDest, lpszText, strlen(lpszText), &rcText, DT_CENTER | DT_VCENTER);
}


// BCX generated
void __UV__CenterWindow (HWND hwnd, HWND Xhwnd, HWND Yhwnd)
{
    if(UV.Parent_hwnd)
    {
        Xhwnd = UV.hParenthwnd;
    }

    RECT rect, rectP;
    int  x, y, width, height;
    int  screenwidth, screenheight;
    if(Xhwnd==0)
    {
        RECT  DesktopArea;
        RECT  rc;
        SystemParametersInfo(SPI_GETWORKAREA,0,&DesktopArea,0);
        GetWindowRect(hwnd,&rc);
        SetWindowPos(hwnd,HWND_TOP,
                     ((DesktopArea.right-DesktopArea.left)-(rc.right-rc.left))/2+
                     DesktopArea.left,((DesktopArea.bottom-DesktopArea.top)-
                                       (rc.bottom-rc.top))/2 + DesktopArea.top,0,0,SWP_NOSIZE);
        return;
    }
    GetWindowRect (hwnd,&rect);
    GetWindowRect (Xhwnd,&rectP);
    width = rect.right-rect.left;
    x = ((rectP.right-rectP.left)-width)/2 + rectP.left;
    if(Yhwnd==NULL)
    {
        height = rect.bottom-rect.top;
        y = ((rectP.bottom-rectP.top)-height)/2 + rectP.top;
    }
    else
    {
        GetWindowRect(Yhwnd,&rectP);
        height = rect.bottom-rect.top;
        y = ((rectP.bottom-rectP.top)-height)/2+rectP.top;
    }

    screenwidth = GetSystemMetrics(SM_CXSCREEN);
    screenheight = GetSystemMetrics(SM_CYSCREEN);
    if ((x<0)) x=0;
    if ((y<0)) y=0;
    if ((x+width>screenwidth))   x = screenwidth-width;
    if ((y+height>screenheight)) y = screenheight-height;
    MoveWindow (hwnd, x, y, width, height, FALSE);
}


/* Fade In And Fade Out Window */
void __UV__FadeIn(HWND hwnd,int SPEED)
{
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    int nAlpha = 0;

    while(nAlpha < 0xD5)
    {
        SetLayeredWindowAttributes(hwnd, 0,nAlpha, LWA_ALPHA);
        Sleep(SPEED);
        nAlpha += 5;
        UpdateWindow(hwnd);
        ShowWindow(hwnd, SW_SHOW);
    }
}

void __UV__FadeOut(HWND hwnd,int SPEED)
{
    SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    int nAlpha = 0xD5;

    while(nAlpha > 0)
    {
        SetLayeredWindowAttributes(hwnd, 0,nAlpha, LWA_ALPHA);
        Sleep(SPEED);
        nAlpha -= 10;
    }
}

/* SetTransparency*/
void _UV_SetTransparency(HWND hWnd,int trans)
{
    typedef DWORD (WINAPI *pLayer)(HWND, DWORD, BYTE, DWORD);
    pLayer SetLayer;
    LONG wAttr;
    SetLayer = (pLayer)GetProcAddress(LoadLibrary("user32.dll"), "SetLayeredWindowAttributes");
    wAttr  = GetWindowLong(hWnd, GWL_EXSTYLE);
    wAttr |= 0x80000;	//WS_EX_LAYERED
    SetWindowLong(hWnd, GWL_EXSTYLE, wAttr);
    SetLayer(hWnd, 0, trans, 2);
}

void __UV__SetRegion(HWND hWnd,int RGNID)
{
    HINSTANCE hInst = GetModuleHandle(NULL);
    HRSRC	hRsrc;
    DWORD	size;
    HRGN	hRgn;
    LPVOID	pRsrc;

    hRsrc = FindResource(hInst, MAKEINTRESOURCE(RGNID), "RGN");
    pRsrc = LockResource(LoadResource(hInst, hRsrc));
    size  = SizeofResource(hInst, hRsrc);
    hRgn = ExtCreateRegion(NULL, size, (const RGNDATA*)pRsrc);
    SetWindowRgn(hWnd, hRgn, TRUE);
}

int __UV__CountLines(char *str)
{
    int l=0,i=0;

    for(i=0; str[i]!='\0'; i++)
    {
        if(str[i] == '\n')
        {
            l++;
        }
    }

    return l;
}
