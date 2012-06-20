
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

//it should be called first than any-other uvlib function.
void UV_Init(HWND);

//params : width,height,font-color,bg-color,font-handle,font-size,text
void UV_SetupScroller(int,int,COLORREF,COLORREF,HFONT,int,char*);

//params : Bitmap , Bitmap-Region , XM Tune
void UV_SetResources(int,int,int);
void UV_UseResources(bool,bool,bool);

//params : speed , centering , delay
void UV_Extra(int speed=25,int center=0,int delay=0);

//this function helps to create font, you can use your own way to create font
//parameters are .: font-name,font-size,font-boldness
HFONT UV_CreateFont(char *,int,int );

//on/off fading. + set transparency in off mode
void UV_Fading(bool,int trans=210,int start=15,int end=20);

//to create border, param : size and color [hint use RGB();]
//if border is used with rgn [Example 4] than right side border may have problem
//in that case increase or decrese third parameter.
void UV_Border(int,COLORREF,int rb=0);

// set custome cursor
void UV_SetCursor(HCURSOR);

//uv about dialog proc.
BOOL CALLBACK UV_AboutDialogProc(HWND , UINT , WPARAM , LPARAM );
