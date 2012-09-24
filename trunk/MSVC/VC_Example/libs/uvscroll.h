
/*
    Mike.X.'s Ultra VScroll Lib 1.3
    -------------------------------------

    Name    : Ultra VScroll Lib
    Author  : Mike.X.
    Website : http://mikex.0fees.net
    Blog... : http://themikex.blogspot.com
    Creation : 20 Nov , 2011
    Last Updated : 23 Sept. , 2012

    Resource required :
    1.one xm tune ( ufmodlib required ) {optional}
    2.one bitmap image background skin  {optional}
    3.region file for background skin   {optional}
    NOTE : libs are required to be linked : ufmod , winmm , uvscroller
*/

//it should be called first than any-other uvlib function.
void UV_Init(HWND);

//params : width,height,font-color,bg-color,font-handle,font-size,text
//for fullscreen speify width and height both as 0
void UV_SetupScroller(int,int,COLORREF,COLORREF,HFONT,int,char*);

//params : Bitmap , Bitmap-Region , XM Tune
void UV_SetResources(int,int,int);
void UV_UseResources(bool,bool,bool); //isn't needed no more

//params : speed , centering , delay ,text align flag
void UV_Extra(int speed=25,int center=0,int delay=0,UINT ta = DT_LEFT);

//can window move ?
void UV_CanMove(bool);

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
