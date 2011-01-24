
#define ERRORMSG            // activa printado mensajes de error
#define LOADING             // activa pintado "wait while loading..."
#define AUTOCENTER          // activa centrado de la ventana


#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <string.h>
#include <stdio.h>
#include "../../intro.h"
#include "../../sys/msys.h"
//#include "../mys_malloc.h"
//#include "../mys_font.h"

//----------------------------------------------------------------------------

typedef struct
{
    //---------------
    Display     *hDisplay;
    GLXContext  hRC;
    Window      hWnd;
    //---------------
    int         full;
    //---------------
    char        wndclass[11];	// window class and title :)
    //---------------
	MSYS_EVENTINFO   events;
}WININFO;

static int doubleBufferVisual[]  =
{
        GLX_RGBA,           // Needs to support OpenGL
        GLX_DEPTH_SIZE, 24, // Needs to support a 16 bit depth buffer
        GLX_DOUBLEBUFFER,   // Needs to support double-buffering
        None                // end of list
};


static WININFO wininfo = {  0,0,0,
							1,	// full
							{'r','g','b','a','_','i','n','t','r','o',0}
                            };

#ifdef LOADING
static char fnt_wait[6]    = "arial";
static char msg_wait[22]   = "wait while loading...";
#endif
#ifdef ERRORMSG
static char msg_error[207] = "intro_init()!\n\n"\
                             "  no memory?\n"\
                             "  no music?\n"\
                             "  no GL_ARB_multitexture?\n"\
                             "  no GL_ARB_vertex_program?\n"\
                             "  no GL_ARB_fragment_program?\n"\
							 "  no GL_EXT_texture_lod_bias?\n"\
                             "  no GL_EXT_texture3D?\n"\
                             "  no GL_SGIS_generate_mipmap?";

static char tlt_error[6]   = "error";
#endif




//----------------------------------------------------------------------------

// n = [0..224]
void loadbar( int  n )
{
/*
    WININFO *info = &wininfo;

    int     xo = 16*(XRES)>>8;
    int     y1 = (240*YRES)>>8;
    int     yo = y1-8;

    // draw background
    SelectObject( wininfo.hDC, CreateSolidBrush(0x0045302c) );
    Rectangle( wininfo.hDC, 0, 0, XRES, YRES );

    // draw text
    SetBkMode( wininfo.hDC, TRANSPARENT );
    SetTextColor( wininfo.hDC, 0x00ffffff );
    SelectObject( wininfo.hDC, CreateFont( 44,0,0,0,0,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,fnt_wait) );
    TextOut( wininfo.hDC, (XRES-318)>>1, (YRES-38)>>1, msg_wait, 21 );
    //DrawText( info->hDC, msg_wait, 21, &rec, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

    // draw bar
    SelectObject( wininfo.hDC, CreateSolidBrush(0x00705030) );
    Rectangle( wininfo.hDC, xo, yo, (240*XRES)>>8, y1 );
    SelectObject( wininfo.hDC, CreateSolidBrush(0x00f0d0b0) );
    Rectangle( wininfo.hDC, xo, yo, ((16+n)*XRES)>>8, y1 );
    */
    //iqiq TODO
}

MSYS_EVENTINFO *getEvents( void )
{
	return &wininfo.events;
}

int getKeyPress( int key )
{
	int res;

	res = wininfo.events.keyb.state[key];
	wininfo.events.keyb.state[key] = 0;
	return res;
}

static void queryState( void )
{
	Window root_return;
	Window child_return;
	int root_x_return;
	int root_y_return;
	int win_x_return;
	int win_y_return;
	unsigned int mask_return;

	bool result = XQueryPointer(wininfo.hDisplay, wininfo.hWnd, &root_return, &child_return,  &root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);

	if (result)
	{
		wininfo.events.mouse.ox = wininfo.events.mouse.x;
		wininfo.events.mouse.oy = wininfo.events.mouse.y;
		wininfo.events.mouse.x = win_x_return;
		wininfo.events.mouse.y = win_y_return;
		wininfo.events.mouse.dx =  wininfo.events.mouse.x - wininfo.events.mouse.ox;
		wininfo.events.mouse.dy =  wininfo.events.mouse.y - wininfo.events.mouse.oy;

		wininfo.events.mouse.obuttons[0] = wininfo.events.mouse.buttons[0];
		wininfo.events.mouse.obuttons[1] = wininfo.events.mouse.buttons[1];
		wininfo.events.mouse.buttons[0] = (mask_return & 256)?1:0;
		wininfo.events.mouse.buttons[2] = (mask_return & 512)?1:0;
		wininfo.events.mouse.buttons[1] = (mask_return & 1024)?1:0;

		wininfo.events.mouse.dbuttons[0] = wininfo.events.mouse.buttons[0] - wininfo.events.mouse.obuttons[0];
		wininfo.events.mouse.dbuttons[1] = wininfo.events.mouse.buttons[1] - wininfo.events.mouse.obuttons[1];
	}

	/*
	POINT   p;
	int     i;
	wininfo.events.keyb.state[KEY_LEFT]     = GetAsyncKeyState( VK_LEFT );
	wininfo.events.keyb.state[KEY_RIGHT]    = GetAsyncKeyState( VK_RIGHT );
	wininfo.events.keyb.state[KEY_UP]       = GetAsyncKeyState( VK_UP );
	wininfo.events.keyb.state[KEY_DOWN]     = GetAsyncKeyState( VK_DOWN );
	wininfo.events.keyb.state[KEY_PGUP]     = GetAsyncKeyState( VK_PRIOR );
	wininfo.events.keyb.state[KEY_PGDOWN]   = GetAsyncKeyState( VK_NEXT );
	wininfo.events.keyb.state[KEY_SPACE]    = GetAsyncKeyState( VK_SPACE );
	wininfo.events.keyb.state[KEY_RSHIFT]   = GetAsyncKeyState( VK_RSHIFT );
	wininfo.events.keyb.state[KEY_RCONTROL] = GetAsyncKeyState( VK_RCONTROL );
	wininfo.events.keyb.state[KEY_LSHIFT]   = GetAsyncKeyState( VK_LSHIFT );
	wininfo.events.keyb.state[KEY_LCONTROL] = GetAsyncKeyState( VK_LCONTROL );
	wininfo.events.keyb.state[KEY_1]        = GetAsyncKeyState( '1' );
	wininfo.events.keyb.state[KEY_2]        = GetAsyncKeyState( '2' );
	wininfo.events.keyb.state[KEY_3]        = GetAsyncKeyState( '3' );
	wininfo.events.keyb.state[KEY_4]        = GetAsyncKeyState( '4' );
	wininfo.events.keyb.state[KEY_5]        = GetAsyncKeyState( '5' );
	wininfo.events.keyb.state[KEY_6]        = GetAsyncKeyState( '6' );
	wininfo.events.keyb.state[KEY_7]        = GetAsyncKeyState( '7' );
	wininfo.events.keyb.state[KEY_8]        = GetAsyncKeyState( '8' );
	wininfo.events.keyb.state[KEY_9]        = GetAsyncKeyState( '9' );
	wininfo.events.keyb.state[KEY_0]        = GetAsyncKeyState( '0' );
	for( i=KEY_A; i<=KEY_Z; i++ )
		wininfo.events.keyb.state[i] = GetAsyncKeyState( 'A'+i-KEY_A );

	//-------
	GetCursorPos( &p );



	wininfo.events.mouse.obuttons[0] = wininfo.events.mouse.buttons[0];
	wininfo.events.mouse.obuttons[1] = wininfo.events.mouse.buttons[1];
	wininfo.events.mouse.buttons[0] = GetAsyncKeyState(VK_LBUTTON);
	wininfo.events.mouse.buttons[1] = GetAsyncKeyState(VK_RBUTTON);
	wininfo.events.mouse.buttons[2] = GetAsyncKeyState(VK_MBUTTON);

	wininfo.events.mouse.dbuttons[0] = wininfo.events.mouse.buttons[0] - wininfo.events.mouse.obuttons[0];
	wininfo.events.mouse.dbuttons[1] = wininfo.events.mouse.buttons[1] - wininfo.events.mouse.obuttons[1];
*/
}

//----------------------------------------------------------------------------

static void window_end( WININFO *info )
{
    XDestroyWindow( info->hDisplay, info->hWnd );
    XCloseDisplay( info->hDisplay );
}

static int window_init( WININFO *info )
{
    XVisualInfo *visualInfo;
    int         errorBase;
    int         eventBase;

    info->hDisplay = XOpenDisplay( NULL );
    if( !info->hDisplay )
        return( 0 );

    // Make sure OpenGL's GLX extension supported
    if( !glXQueryExtension( info->hDisplay, &errorBase, &eventBase ) )
        return( 0 );

    // Try for the double-bufferd visual first
    visualInfo = glXChooseVisual( info->hDisplay, DefaultScreen(info->hDisplay), doubleBufferVisual );
    if( visualInfo == NULL )
        return( 0 );

    // Create an OpenGL rendering context
    info->hRC = glXCreateContext( info->hDisplay, visualInfo, NULL, GL_TRUE );
    if( info->hRC == NULL )
        return( 0 );

    // Create an X colormap since we're probably not using the default visual
    Colormap colorMap;
    colorMap = XCreateColormap( info->hDisplay, RootWindow(info->hDisplay, visualInfo->screen), 
                                visualInfo->visual, AllocNone );

    XSetWindowAttributes winAttr;
    winAttr.colormap     = colorMap;
    winAttr.border_pixel = 0;
    winAttr.event_mask   = ExposureMask           |
                           VisibilityChangeMask   |
                           KeyPressMask           |
                           KeyReleaseMask         |
                           ButtonPressMask        |
                           ButtonReleaseMask      |
                           PointerMotionMask      |
                           StructureNotifyMask    |
                           SubstructureNotifyMask |
                           FocusChangeMask;

    // Create an X window with the selected visual
    info->hWnd = XCreateWindow( info->hDisplay, RootWindow(info->hDisplay, visualInfo->screen), 
                              0, 0, XRES, YRES, 0, visualInfo->depth, InputOutput, 
				visualInfo->visual, CWBorderPixel | CWColormap | CWEventMask,
                              &winAttr );

    if( !info->hWnd )
        return( 0 );
	
    //XSetStandardProperties( info->hDisplay, info->hWnd, info->wndclass, info->wndclass, None, argv, argc, NULL );
    char *argv[] = { "hola", 0 };
    XSetStandardProperties( info->hDisplay, info->hWnd, info->wndclass,
    info->wndclass, None, argv, 1, NULL );

    glXMakeCurrent( info->hDisplay, info->hWnd, info->hRC );


    XMapWindow( info->hDisplay, info->hWnd );


    return( 1 );
}


//----------------------------------------------------------------------------

//void entrypoint( void )
int main( void )
{
    XEvent      event;
    int         done=0;
    WININFO     *info = &wininfo;


	if( !msys_init(0) )
	{
		#ifdef ERRORMSG
		printf( "msys_init()!" );
		#endif
		return( 1 );
	}

/*
    if( MessageBox( 0, "fullscreen?", info->wndclass, MB_YESNO|MB_ICONQUESTION)==IDYES )
        info->full++;
*/
	if( !window_init(info) )
	{
		window_end( info );
		#ifdef ERRORMSG
			printf( "window_init()!" );
		#endif
		return( 2 );
	}

    //FONT_Init( (long)info->hDisplay );

	if( !intro_init( XRES, YRES, 0, 0 ) )
	{
		window_end( info );
		#ifdef ERRORMSG
		printf( msg_error );
		#endif
		return( 3 );
	}

	while( !done )
	{
		queryState();
		done = intro_do();

		wininfo.events.keyb.state[KEY_LEFT]     = 0;
		wininfo.events.keyb.state[KEY_RIGHT]    = 0;
		wininfo.events.keyb.state[KEY_UP]       = 0;
		wininfo.events.keyb.state[KEY_DOWN]     = 0;
		wininfo.events.keyb.state[KEY_PGUP]     = 0;

		while( XPending(info->hDisplay) )
		{
			XNextEvent( info->hDisplay, &event );
			switch( event.type )
			{
			case KeyPress:
				switch( XKeycodeToKeysym( info->hDisplay, event.xkey.keycode, 0 ) )
				{
				case XK_Up:
					wininfo.events.keyb.state[KEY_UP] = 1;break;
				case XK_Down:
					wininfo.events.keyb.state[KEY_DOWN] = 1;break;
				case XK_Left:
					wininfo.events.keyb.state[KEY_LEFT] = 1;break;
				case XK_Right:
					wininfo.events.keyb.state[KEY_RIGHT] = 1;break;
				case XK_Escape:
					done = 1;break;
				}break;
			case DestroyNotify:
				done = 1;break;
			}
		}
		glXSwapBuffers( info->hDisplay, info->hWnd );
	}

    intro_end();

    window_end( info );

	msys_end();

    return( 0 );
}


#include <stdlib.h>
extern "C" {
int __cxa_pure_virtual()
{
	return 0;
}
// commented out when using default libs
/*
void _start( void )
{
	main();
	exit(0);
}
*/
}
