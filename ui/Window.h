#pragma once

#ifdef _WINDOWS

#include <windows.h>
#include <windowsx.h>
#include <string>

namespace base
{
	class Window
	{
	public:
		typedef void (*KeyPressCallback)( int key );

		Window();
		HWND                                       getHandle();
		virtual void                                    show(); // shows the window

		virtual void                                   paint();
		virtual void                                 destroy(); // closes and destroys the window
		virtual void         setCaption( std::string caption );
		virtual void          setSize( int width, int height );
		void  setKeyPressCallback( KeyPressCallback keyPress );

	//protected:
		HWND                                            m_hwnd; // window handle
		int                                            m_width;
		int                                           m_height;
		std::string                                  m_caption; // caption text of the window


		//callbacks
		KeyPressCallback                            m_keypress;
	};

	// this function will return the classid of the default windows class
	std::string getRegisteredWndClass();

	// will return the winappinstance
	HINSTANCE getWinAppInstance();
}
#endif

#ifdef linux
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace base
{
	typedef Window HWND;

	class Window
	{
	public:
		Window();
		virtual ~Window();
		HWND                                       getHandle();
		void                                            show(); // shows the window

		virtual void                                   paint();
		virtual void                                 destroy(); // closes and destroys the window
	protected:
		HWND                                            m_hwnd; // window handle
	};
}
#endif
