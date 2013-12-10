/*
 * CWindow.cpp
 *
 *  Created on: 06 ���. 2013 �.
 *      Author: karpachevnk
 */

#include "CWindow.h"

namespace Win32_GUI_NMSP
{

CWindow::CWindow(const CWindowClass &_class, DWORD _style, DWORD _exstyle, LPCTSTR _wname, HWND _owner, HMENU _menu, HINSTANCE _hinst,
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

	// ��������� ���������� ���� � ������� ����
	SetWindowPtr(m_hWnd, this);

	// ������� ����� ������� ������� ���������
	m_DefWindowProc = GetWindowProc(m_hWnd);

	// ���� ������� ������� ��������� �� �������� �������
	if (m_DefWindowProc != BasicWndProc)
	{
		// ��������� �����������, ����� ������������� ��� �������
		SetWindowProc(m_hWnd, BasicWndProc);
	}
	else
	{
		// ��������� � ����� ������� ��������� ��������� ������� ���������
		m_DefWindowProc = ::DefWindowProc;
	}

}



CWindow::~CWindow()
{
	// ���� ���� WinAPI �� ����������
	if (::IsWindow(m_hWnd))
	{
		throw "Call Destroy() before deleting CWindow object";	// ���������� ����������
	}

	// ���� ��������� � ������� ������� ��������� �� �������� ��������� ���������
	// ������ ��� �������� �����������
	if (m_DefWindowProc != ::DefWindowProc)
	{
		// �������� �����������
		SetWindowProc(m_hWnd, m_DefWindowProc);
	}

	// �������� ���������� ���� � ������� ����
	SetWindowPtr(m_hWnd, NULL);
}


LRESULT CALLBACK
CWindow::BasicWndProc(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp)
{
	// ������� ����� �������
	CWindow * pWindow = CWindow::GetWindowPtr(_hwnd);

	LRESULT ev_result;

	// ���� ������ ��� ������
	if (pWindow)
	{
		// �������� ���������� ������� �������
		ev_result = pWindow->OnEvent(_msg, _wp, _lp);
	}
	else
	{
		// �������� ��������� ��������� ����������
		ev_result = ::DefWindowProc(_hwnd, _msg, _wp, _lp);
	}

	return ev_result;
}

WNDPROC
CWindow::SetWindowProc(HWND _hwnd, WNDPROC _wproc)
{
	return (WNDPROC)::SetWindowLongPtr(_hwnd, GWLP_WNDPROC, (LONG_PTR)BasicWndProc);
}


WNDPROC
CWindow::GetWindowProc(HWND _hwnd)
{
	return (WNDPROC)::GetWindowLongPtr(_hwnd, GWLP_WNDPROC);
}

CWindow *
CWindow::SetWindowPtr(HWND _hwnd, CWindow * _pwin)
{
	return (CBasicWindow*)::SetWindowLongPtr(_hwnd, GWLP_USERDATA, (LONG_PTR)_pwin);
}


CWindow *
CWindow::GetWindowPtr(HWND _hwnd)
{
	return (CWindow*)::GetWindowLongPtr(_hwnd, GWLP_USERDATA);
}

LRESULT
CWindow::OnEvent(UINT _msg, WPARAM _wp, LPARAM _lp)
{
	LRESULT ev_result;

	switch (_msg)
	{
		case WM_DESTROY:
			ev_result = OnEvent_Destroy(_msg, _wp, _lp);
			break;
		default:
			ev_result = OnEvent_Default(_msg, _wp, _lp);
			break;
	}

	return ev_result;
}


LRESULT
CWindow::OnEvent_Default(UINT _msg, WPARAM _wp, LPARAM _lp)
{
	return m_DefWindowProc(g_hWnd(), _msg, _wp, _lp);
}


LRESULT
CWindow::OnEvent_Destroy(UINT _msg, WPARAM _wp, LPARAM _lp)
{
	return OnEvent_Default(_msg, _wp, _lp);
}


void
CWindow::Destroy()
{
	// ���� ������ ������������
	::DestroyWindow(m_hWnd);
}

} /* namespace Win32_GUI_NMSP */
