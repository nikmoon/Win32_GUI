/*
 * CWindow.cpp
 *
 *  Created on: 06 дек. 2013 г.
 *      Author: karpachevnk
 */

#include "CWindow.h"

namespace Win32_GUI_NMSP
{

CWindow::CWindow(const CWindowClass &_class, DWORD _style, DWORD _exstyle, LPCTSTR _wname, HWND _owner, HMENU _menu, HINSTANCE _hinst,
		LPVOID _pdata, int _x, int _y, int _cx, int _cy)
{
	// пробуем создать окно
	m_hWnd = ::CreateWindowEx(
		_exstyle,
		_class.g_ClassName(),
		_wname,
		_style,
		_x, _y, _cx, _cy,
		_owner,
		_menu,
		_hinst,
		_pdata
	);

	// если не удалось создать окно
	if (!m_hWnd)
	{
		throw ::GetLastError();		// генерируем исключение
	}

	// инициализируем пол€ экземпл€ра
	m_pWndClass = &_class;
	m_Owner = _owner;
	m_Style = _style;
	m_ExStyle = _exstyle;
	m_WndName = _wname;
	m_hMenu = _menu;
	m_hInstance = _hinst;
	m_pData = _pdata;
}



CWindow::~CWindow()
{
	// если окно WinAPI не уничтожено
	if (::IsWindow(m_hWnd))
	{
		throw "Call Destroy() before deleting CWindow object";	// генерируем исключение
	}
}


void
CWindow::Destroy()
{
	// окно должно существовать
	::DestroyWindow(m_hWnd);
}

} /* namespace Win32_GUI_NMSP */
