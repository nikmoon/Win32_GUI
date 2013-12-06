/*
 * CWindow.cpp
 *
 *  Created on: 06 ���. 2013 �.
 *      Author: karpachevnk
 */

#include "CWindow.h"

namespace Win32_GUI_NMSP
{

CWindow::CWindow(CWindowClass &_class, DWORD _style, DWORD _exstyle, LPCTSTR _wname, HWND _owner, HMENU _menu, HINSTANCE _hinst,
		LPVOID _pdata, int _x, int _y, int _cx, int _cy)
{
	// ������� ������� ����
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

	// ���� �� ������� ������� ����
	if (!m_hWnd)
	{
		throw ::GetLastError();		// ���������� ����������
	}

	// �������������� ���� ����������
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
	// ���� ���� �� ���� ����������
	if (::IsWindow(m_hWnd))
	{
		// �������� ���������� ����
		if (!::DestroyWindow(m_hWnd))
		{
			throw ::GetLastError();		// ���������� ����������
		}
	}
}


const CWindow &
CWindow::AddChildWindow(CWindowClass &_class, DWORD _style, DWORD _exstyle, LPCTSTR _wname, HMENU _menu, LPVOID _pdata,
		int _x, int _y, int _cx, int _cy)
{
	// ������� ����
	CWindow *pChild = new CWindow(_class, _style, _exstyle, _wname, g_hWnd(), _menu, g_hInstance(), _pdata, _x, _y, _cx, _cy);

	// ��������� ��������� ���� � ������ �������� ����
	m_ChildWindows.push_back(pChild);

	return *pChild;
}


void
CWindow::RemoveChildWindow(CWindow &_window)
{
	delete &_window;
	m_ChildWindows.remove(&_window);
}


void
CWindow::ClearChildList()
{
	// ���������� ��� �������� ����
	while (!m_ChildWindows.empty())
	{
		delete m_ChildWindows.back();
		m_ChildWindows.pop_back();
	}
}


} /* namespace Win32_GUI_NMSP */
