#pragma once


namespace base
{
#ifdef _WINDOWS
	class Window
	{
	public:
		Window() : m_hwnd(NULL)
		{
		}
		HWND getHandle()
		{
			return m_hwnd;
		}
	protected:
		HWND                                            m_hwnd; // window handle
	};
#endif
}