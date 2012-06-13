#ifdef _WINDOWS
#include "GLWindow.h"
#include "GLExtensions.h"
#include "GLTemporaryContext.h"

#include <gltools/gl.h>
#include <string>
#include <iostream>


namespace base
{

	// when using this constructor the window will be created when show is called first time
	GLWindow::GLWindow() : Window(), m_init(0), m_numSamples(-1), m_sampleBuffers(false)
	{
		setSize( 800, 600 );
		setCaption( "app" );
	}

	GLWindow::GLWindow( int width, int height, std::string caption, InitCallback init ) : Window(), m_numSamples(-1), m_sampleBuffers(false), m_init(init)
	{
		setSize( width, height );
		setCaption( caption );
		createWindow();
	}

	// shows the window (creates the window when shown the first time)
	void GLWindow::show()
	{
		if(!m_hwnd)
			createWindow();
		Window::show();
	}

	// closes and destroys the window
	void GLWindow::destroy()
	{
		// shutdown callback
		if( m_shutdown )
			m_shutdown();
		Window::destroy();
	}

	void GLWindow::createWindow()
	{
		// intitialisierung
		m_hdc           =            NULL;
		m_hrc           =            NULL;
		m_bpp           =              32;
		m_fsbpp         =              32;
		m_fullscreen    =           false;
		m_zdepth        =              24; // http://www.gamedev.net/topic/601212-wglchoosepixelformatarb-problem/

		m_pixelformat =     0;

		m_windowRect.left       =      100;
		m_windowRect.top        =      100;
		m_windowRect.right      =  m_windowRect.left + m_width;
		m_windowRect.bottom     = m_windowRect.top + m_height;
		
		m_fullscreenRect.left   =        0;
		m_fullscreenRect.top    =        0;
		m_fullscreenRect.right  =  m_width;
		m_fullscreenRect.bottom = m_height;

		// Save The Current Display State
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &this->m_DMsaved); 

		m_hinstance          = GetModuleHandle(NULL);

		m_wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		m_wc.lpfnWndProc	= (WNDPROC) Application::windowHandler;
		m_wc.cbClsExtra		= 0;
		m_wc.cbWndExtra		= 0;
		m_wc.hInstance		= m_hinstance;
		m_wc.hIcon			= NULL;
		m_wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
		m_wc.hbrBackground	= NULL;
		m_wc.lpszMenuName	= NULL;
		m_wc.lpszClassName	= "mf window class";

		if( !RegisterClass( &m_wc ) )
			return;

		m_dwExstyle	        = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		m_dwstyle		        = WS_OVERLAPPEDWINDOW;

		/*
		if ( _fullscreen == true )
		{
			dwstyle = WS_POPUP;
		}
		else
		{
			dwstyle = WS_OVERLAPPEDWINDOW;
		}
*/

		AdjustWindowRectEx( &m_windowRect , m_dwstyle , FALSE , m_dwExstyle );


		m_hwnd = CreateWindowEx(m_dwExstyle, getRegisteredWndClass().c_str(), m_caption.c_str() ,
								m_dwstyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
								m_windowRect.left, m_windowRect.top, 			 
								m_windowRect.right  - m_windowRect.left,	
								m_windowRect.bottom - m_windowRect.top, 
								NULL, NULL, m_hinstance, NULL);
		int x = GetLastError();
		if( m_hwnd == NULL) return;

		m_hdc = GetDC( m_hwnd );


		//set pixelformat
		PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),		// Size Of This Pixel Format Descriptor
			1,									// Version Number
			PFD_DRAW_TO_WINDOW |				// Format Must Support Window
			PFD_SUPPORT_OPENGL |				// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,					// Must Support Double Buffering
			PFD_TYPE_RGBA,						// Request An RGBA Format
			m_bpp,								// Select Our Color Depth
			0, 0, 0, 0, 0, 0,					// Color Bits Ignored
			0,									// No Alpha Buffer
			0,									// Shift Bit Ignored
			0,									// No Accumulation Buffer
			0, 0, 0, 0,							// Accumulation Bits Ignored
			m_zdepth,							// 16Bit Z-Buffer (Depth Buffer)  
			0,									// No Stencil Buffer
			0,									// No Auxiliary Buffer
			PFD_MAIN_PLANE,						// Main Drawing Layer
			0,									// Reserved
			0, 0, 0								// Layer Masks Ignored
		};

		m_hdc = GetDC(m_hwnd);

		m_pixelformat = choosePixelFormat( m_hdc , &pfd );

		SetPixelFormat( m_hdc , m_pixelformat , &pfd );

		m_hrc = wglCreateContext( m_hdc );
		wglMakeCurrent( m_hdc , m_hrc );

/*
		if( _fullscreen )
		{
			setFullScreen( _width , _height , bpp );
		}

		return true;
*/
		Application::registerWindow( this );

		// now that we (hopefully) have valid gl...
		if( m_init )
			m_init();
	}

	typedef bool (APIENTRY *PFNWGLGETPIXELFORMATATTRIBIVARB)(HDC hdc,
                                                         int iPixelFormat,
                                                         int iLayerPlane,
                                                         unsigned int nAttributes,
                                                         const int *piAttributes,
                                                         int *piValues);
	typedef bool (APIENTRY *PFNWGLCHOOSEPIXELFORMATARB)(HDC hdc,
                                                    const int *piAttribList,
                                                    const float *pfAttribFList,
                                                    unsigned int nMaxFormats,
                                                    int *piFormats,
                                                    unsigned int *nNumFormats);

	int GLWindow::choosePixelFormat( HDC pdc, PIXELFORMATDESCRIPTOR *dummyPfd )
	{
		// we want to use wglChoosePixelFormatARB which is a replacement for the windows function ChoosePixelFormat
		// it allows a dynamic list of ints for specifying the pixelformat. In addition it allows us to specify
		// this like samplebuffers etc.
		// the problem however is, that we need to be in a working opengl context in order to use wglGetProcAddress and
		// wglChoosePixelFormatARB. Otherwise these functions wont work. This is a henn/egg problem because we want to choose
		// the pixelformat in order to create the context. the solution is to create a temporary opengl context
		GLTemporaryContext tempGlContext;

		PFNWGLCHOOSEPIXELFORMATARB wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARB) wglGetProcAddress("wglChoosePixelFormatARB");
		int chosenPfi = 0;
		if (wglChoosePixelFormatARB)
		{
			int pixelFormat = 0;
			unsigned int numFormats = 0;
			int iAttributes[40];
			int i = 0;
			iAttributes[i++] = WGL_ACCELERATION_ARB;
			iAttributes[i++] = WGL_FULL_ACCELERATION_ARB;
			iAttributes[i++] = WGL_SUPPORT_OPENGL_ARB;
			iAttributes[i++] = TRUE;
			iAttributes[i++] = WGL_DRAW_TO_WINDOW_ARB;
			iAttributes[i++] = TRUE;
			iAttributes[i++] = WGL_COLOR_BITS_ARB;
			iAttributes[i++] = 24;
			iAttributes[i++] = WGL_DOUBLE_BUFFER_ARB;
			iAttributes[i++] = TRUE;
			iAttributes[i++] = WGL_DEPTH_BITS_ARB;
			iAttributes[i++] = m_zdepth;

			iAttributes[i++] = WGL_PIXEL_TYPE_ARB;
			iAttributes[i++] = WGL_TYPE_RGBA_ARB;

			// ignoring color bits
			//if (d->glFormat.redBufferSize() != -1) {
			//	iAttributes[i++] = WGL_RED_BITS_ARB;
			//	iAttributes[i++] = d->glFormat.redBufferSize();
			//}
			//if (d->glFormat.greenBufferSize() != -1) {
			//	iAttributes[i++] = WGL_GREEN_BITS_ARB;
			//	iAttributes[i++] = d->glFormat.greenBufferSize();
			//}
			//if (d->glFormat.blueBufferSize() != -1) {
			//	iAttributes[i++] = WGL_BLUE_BITS_ARB;
			//	iAttributes[i++] = d->glFormat.blueBufferSize();
			//}

			// no alpha buffer
			//iAttributes[i++] = WGL_ALPHA_BITS_ARB;
			//iAttributes[i++] = d->glFormat.alphaBufferSize() == -1 ? 8 : d->glFormat.alphaBufferSize();

			// no accumulation bits
			//iAttributes[i++] = WGL_ACCUM_BITS_ARB;
			//	iAttributes[i++] = d->glFormat.accumBufferSize() == -1 ? 16 : d->glFormat.accumBufferSize();

			if(m_stencilBuffer)
			{
				iAttributes[i++] = WGL_STENCIL_BITS_ARB;
				iAttributes[i++] = 8;
			}

			// no idea what this is about
			//iAttributes[i++] = WGL_NUMBER_OVERLAYS_ARB;
			//iAttributes[i++] = 1;

			int si = 0;
			bool trySampleBuffers = (GLExtensions::glExtensions() & GLExtensions::SampleBuffers) != 0;
			if (trySampleBuffers && this->m_sampleBuffers)
			{
				iAttributes[i++] = WGL_SAMPLE_BUFFERS_ARB;
				iAttributes[i++] = TRUE;
				iAttributes[i++] = WGL_SAMPLES_ARB;
				si = i;
				iAttributes[i++] = this->m_numSamples == -1 ? 4 : this->m_numSamples;
			}

			// finish
			iAttributes[i] = 0;

			wglChoosePixelFormatARB(pdc, iAttributes, 0, 1, &pixelFormat, &numFormats);

			//do {
				//valid = wglChoosePixelFormatARB(pdc, iAttributes.constData(), 0, 1, &pixelFormat, &numFormats);
				//if (trySampleBuffers  && (!valid || numFormats < 1) && d->glFormat.sampleBuffers())
				//	iAttributes[si] /= 2; // try different no. samples - we aim for the best one
				//else
				//	break;
			//} while ((!valid || numFormats < 1) && iAttributes[si] > 1);

			chosenPfi = pixelFormat;
		}

		if(!chosenPfi)
		{
			// fallback (call the windows function)
			chosenPfi = ChoosePixelFormat( m_hdc , dummyPfd );
		}

		return chosenPfi;
	}


	//
	//
	//
	void GLWindow::paint()
	{
		// make current
		// TODO

		// paintGL
		paintGL();

		// swap buffers
		SwapBuffers( m_hdc );

		// update dirty flag
		Window::paint();
	}

	void GLWindow::paintGL()
	{
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLWindow::setCaption( std::string caption )
	{
		Window::setCaption( caption );
		//TODO: set caption when window already exists
	}

	void GLWindow::setSize( int width, int height )
	{
		Window::setSize( width, height );
		// tODO: set size when window already exists
	}

	// specifies whether a glcontext with samplebuffer support is to be created
	void GLWindow::setSampleBuffers( bool enabled )
	{
		m_sampleBuffers = enabled;
		// TODO: handle the case when the context already has been created
	}

	// specifies the number of samples we want
	void GLWindow::setSamples( int numSamples )
	{
		m_numSamples = numSamples;
		// TODO: handle the case when the context already has been created
	}

	// specifies whether a glcontext with stencil support is to be created
	void GLWindow::setStencilBuffer( bool enabled )
	{
		m_stencilBuffer = enabled;
	}

	void GLWindow::setInitCallback( InitCallback init )
	{
		m_init = init;
	}

	void GLWindow::setShutdownCallback( ShutdownCallback shutdown )
	{
		m_shutdown = shutdown;
	}


	/*
	destroy:
	    if( info->hRC )
    {
        wglMakeCurrent( 0, 0 );
        wglDeleteContext( info->hRC );
    }

    if( info->hDC  ) ReleaseDC( info->hWnd, info->hDC );
    if( info->hWnd ) DestroyWindow( info->hWnd );

    UnregisterClass( wndclass, info->hInstance );

    if( info->full )
    {
        ChangeDisplaySettings( 0, 0 );
		while( ShowCursor( 1 )<0 ); // show cursor
    }

	/*

	void GLWindow::show( void )
	{
		ShowWindow( mhwnd , SW_SHOW );
		UpdateWindow( mhwnd );
		SetForegroundWindow( mhwnd );
		SetFocus( mhwnd );
	}
		int GLWindow::getWidth( void )
	{
		RECT windowRect;
		GetClientRect( mhwnd, &windowRect );

		//To access members
		return windowRect.right - windowRect.left;
	}

	int GLWindow::getHeight( void )
	{
		RECT windowRect;
		GetClientRect( mhwnd, &windowRect );

		//To access members
		return windowRect.bottom - windowRect.top;
	}*/
}
#endif








#ifdef linux

#include "GLWindow.h"
#include <string>


namespace base
{
	GLWindow::GLWindow( int width, int height, std::string caption ) : Window()
	{
		XVisualInfo *visualInfo;
		int         errorBase;
		int         eventBase;
		Display *display = Application::getDisplay();

		// Make sure OpenGL's GLX extension supported
		if( !glXQueryExtension( display, &errorBase, &eventBase ) )
			return;

		// Try for the double-bufferd visual first
		int doubleBufferVisual[]  =
		{
				GLX_RGBA,           // Needs to support OpenGL
				GLX_DEPTH_SIZE, 24, // Needs to support a 16 bit depth buffer
				GLX_DOUBLEBUFFER,   // Needs to support double-buffering
				X11_None            // end of list
		};

		visualInfo = glXChooseVisual( display, DefaultScreen(display), doubleBufferVisual );
		if( visualInfo == NULL )
			return;

		// Create an OpenGL rendering context
		m_hrc = glXCreateContext( display, visualInfo, NULL, GL_TRUE );
		if( m_hrc == NULL )
			return;

		// Create an X colormap since we're probably not using the default visual
		Colormap colorMap;
		colorMap = XCreateColormap( display, RootWindow(display, visualInfo->screen),
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
		m_hwnd = XCreateWindow( display, RootWindow(display, visualInfo->screen),
								  0, 0, width, height, 0, visualInfo->depth, InputOutput,
					visualInfo->visual, CWBorderPixel | CWColormap | CWEventMask,
								  &winAttr );

		if( !m_hwnd )
			return;

		char *argv[] = { "hola", 0 };
		XSetStandardProperties( display, m_hwnd, caption.c_str(), 0, X11_None, argv, 1, NULL );

		glXMakeCurrent( display, m_hwnd, m_hrc );

		Application::registerWindow( this );
	}

	GLWindow::~GLWindow()
	{
		Display *display = Application::getDisplay();

		// close connection to xserver
		//XUnmapWindow(display, m_hwnd);

		//Application::unregisterWindow( this );
	}


	//
	//
	//
	void GLWindow::paint()
	{
		// make current
		// TODO

		// paintGL
		paintGL();

		// swap buffers
		glXSwapBuffers( Application::getDisplay(), m_hwnd );

		// update dirty flag
		Window::paint();
	}

	void GLWindow::paintGL()
	{
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}

#endif











/*
// add the opengl library
#pragma message("     _Adding library: opengl32.lib" ) 
#pragma comment( lib, "opengl32.lib" )


namespace win32
{

	GLWindow::GLWindow()
	{
		// intitialisierung
		mhwnd       =  NULL;
		mhDC        =  NULL;
		mhRC        =  NULL;
		bpp         =    32;
		fsbpp       =    32;
		fullscreen  = false;
		zdepth      =    32;
		pixelformat =     0;

		WindowRect.left       =   0;
		WindowRect.top        =   0;
		WindowRect.right      = 640;
		WindowRect.bottom     = 480;
		
		FullscreenRect.left   =   0;
		FullscreenRect.top    =   0;
		FullscreenRect.right  = 640;
		FullscreenRect.bottom = 480;

		// Save The Current Display State
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &this->DMsaved); 

	}
	GLWindow::~GLWindow()
	{
		if( this->mhwnd )
		{
			if( this->fullscreen == true )
				this->restoreScreen();

			if( mhRC )
			{
				wglMakeCurrent( NULL , NULL );
				wglDeleteContext( mhRC );
				mhRC  =  NULL;
			}
			
			if( mhDC )
			{
				ReleaseDC( mhwnd , mhDC );
				mhDC  =  NULL;
			}

			DestroyWindow( this->mhwnd );

			UnregisterClass( L"mf window class" , mhinstance );
		}
	}






	bool GLWindow::createGLWindow( std::string  _caption, int _width, int _height,
								  int _startx, int _starty, int _bpp, bool _fullscreen,
									WNDPROC WindowProcedure, HINSTANCE _hInstance )
	{
		WindowRect.left		= _startx;
		WindowRect.top		= _starty;
		WindowRect.right	= _width  + _startx;
		WindowRect.bottom	= _height + _starty;

		bpp                 = _bpp;

		// convert std::wstring to std::string
		caption             = std::wstring( _caption.begin(), _caption.end() );


		mhinstance          = _hInstance;

		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc		= (WNDPROC) WindowProcedure;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hInstance		= mhinstance;
		wc.hIcon			= NULL;
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground	= NULL;
		wc.lpszMenuName		= NULL;
		wc.lpszClassName	= L"mf window class";


		if( !RegisterClass( &wc ) )
			return false;

		dwExstyle	        = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwstyle		        = WS_OVERLAPPEDWINDOW;

		if ( _fullscreen == true )
		{
			dwstyle = WS_POPUP;
		}
		else
		{
			dwstyle = WS_OVERLAPPEDWINDOW;
		}

		AdjustWindowRectEx( &WindowRect , dwstyle , FALSE , dwExstyle );


		mhwnd = CreateWindowEx(	dwExstyle, L"mf window class" , caption.c_str() ,
								dwstyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
								WindowRect.left, WindowRect.top, 			 
								WindowRect.right  - WindowRect.left,	
								WindowRect.bottom - WindowRect.top, 
								NULL, NULL, mhinstance, NULL);
		int x = GetLastError();
		if( mhwnd == NULL) return false;

		mhDC = GetDC( mhwnd );

		if ( _SetPixelFormat( mhwnd ) )
		{
			mhRC = wglCreateContext( mhDC );
			wglMakeCurrent( mhDC , mhRC );
		} else
		{
			//DestroyWindow( mhwnd );
		}

		if( _fullscreen )
		{
			setFullScreen( _width , _height , bpp );
		}

		return true;
	}


	bool GLWindow::_SetPixelFormat( HWND hWnd )
	{
		PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),		// Size Of This Pixel Format Descriptor
			1,									// Version Number
			PFD_DRAW_TO_WINDOW |				// Format Must Support Window
			PFD_SUPPORT_OPENGL |				// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,					// Must Support Double Buffering
			PFD_TYPE_RGBA,						// Request An RGBA Format
			bpp,								// Select Our Color Depth
			0, 0, 0, 0, 0, 0,					// Color Bits Ignored
			0,									// No Alpha Buffer
			0,									// Shift Bit Ignored
			0,									// No Accumulation Buffer
			0, 0, 0, 0,							// Accumulation Bits Ignored
			zdepth,								// 16Bit Z-Buffer (Depth Buffer)  
			0,									// No Stencil Buffer
			0,									// No Auxiliary Buffer
			PFD_MAIN_PLANE,						// Main Drawing Layer
			0,									// Reserved
			0, 0, 0								// Layer Masks Ignored
		};


		mhDC=GetDC(hWnd);

		pixelformat = ChoosePixelFormat( mhDC , &pfd );

		SetPixelFormat( mhDC , pixelformat , &pfd );

		return true;
	}



	bool GLWindow::setFullScreen( int width, int height, int _bpp )
	{
		// already fullscreen? -> go back
		if( fullscreen )
			return true;

		dwExstyle=WS_EX_APPWINDOW;
		dwstyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		// Save The Current Display State
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &this->DMsaved);

		DEVMODE dmScreenSettings;

		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared 

		dmScreenSettings.dmSize             = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth        = width; 
		dmScreenSettings.dmPelsHeight       = height;
		dmScreenSettings.dmBitsPerPel       = bpp;
		dmScreenSettings.dmFields           = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;
		dmScreenSettings.dmDisplayFrequency = 85;  // we assume that there are at least 85 hz

		if ( ChangeDisplaySettings( &dmScreenSettings , CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
		{
			restoreScreen();
			// somethings gone wrong
			return false;
		}

		fullscreen = true;

		// adjust the window
		FullscreenRect.left   =      0;
		FullscreenRect.top    =      0;
		FullscreenRect.right  =  width;
		FullscreenRect.bottom = height;

		SetWindowLong( mhwnd , GWL_STYLE   , dwstyle   );
		SetWindowLong( mhwnd , GWL_EXSTYLE , dwExstyle );

		SetWindowPos(  mhwnd , HWND_TOP , 0 , 0 , width , height , SWP_SHOWWINDOW );

		AdjustWindowRectEx( &FullscreenRect , dwstyle , FALSE , dwExstyle );

		SetForegroundWindow( mhwnd );

		fsbpp = _bpp;

		// hide the cursor
		//ShowCursor( FALSE );

		return true;
	}

	void GLWindow::restoreScreen( bool show_cursor )
	{	
		dwExstyle	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwstyle		= WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		AdjustWindowRectEx(&WindowRect, dwstyle, FALSE, dwExstyle);

		SetWindowLong( mhwnd , GWL_STYLE   , dwstyle   );
		SetWindowLong( mhwnd , GWL_EXSTYLE , dwExstyle );

		SetWindowPos( mhwnd, HWND_TOP, WindowRect.left, WindowRect.top, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, SWP_SHOWWINDOW );

		if( show_cursor )
			ShowCursor( TRUE );

		if( fullscreen )
		{
			// If The Shortcut Doesn't Work
			if (!ChangeDisplaySettings(NULL,CDS_TEST))
			{
				// Do It Anyway (To Get The Values Out Of The Registry)
				ChangeDisplaySettings(NULL,CDS_RESET);
				// Change Resolution To The Saved Settings
				ChangeDisplaySettings(&this->DMsaved,CDS_RESET);
			}else
			{
				ChangeDisplaySettings(NULL,CDS_RESET);
			}

			fullscreen = false;
		}
	}

	bool GLWindow::toggleFullscreen( void )
	{
		if( this->fullscreen == true )
			this->restoreScreen();
		else
			this->setFullScreen( this->FullscreenRect.right , this->FullscreenRect.bottom , this->bpp );
		return true;
	}


	void GLWindow::show( void )
	{
		ShowWindow( mhwnd , SW_SHOW );
		UpdateWindow( mhwnd );
		SetForegroundWindow( mhwnd );
		SetFocus( mhwnd );
	}

	HWND GLWindow::getHandle()
	{
		return mhwnd;
	}


	int GLWindow::getWidth( void )
	{
		RECT windowRect;
		GetClientRect( mhwnd, &windowRect );

		//To access members
		return windowRect.right - windowRect.left;
	}

	int GLWindow::getHeight( void )
	{
		RECT windowRect;
		GetClientRect( mhwnd, &windowRect );

		//To access members
		return windowRect.bottom - windowRect.top;
	}


} // namespace dk
*/
