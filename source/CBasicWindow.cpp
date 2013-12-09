/*
 * CBasicWindow.cpp
 *
 *  Created on: 09 ���. 2013 �.
 *      Author: karpachevnk
 */

#include "CBasicWindow.h"

namespace Win32_GUI_NMSP
{


CBasicWindow::CBasicWindow(const CWindowClass &_class, DWORD _style, DWORD _exstyle, LPCTSTR _wname, HWND _owner, HMENU _menu,
	HINSTANCE _hinst, LPVOID _pdata, int _x, int _y, int _cx, int _cy)
	: CWindow(_class, _style, _exstyle, _wname, _owner, _menu, _hinst, _pdata, _x, _y, _cx, _cy)
{
	// ��������� ���������� ���� � ������� ����
	SetWindowPtr(g_hWnd(), this);

	// ���� ������ ������� ���������, �������� �� ���������
	m_DefWindowProc = GetWindowProc(g_hWnd());
	if (m_DefWindowProc != BasicWndProc)
	{
		// ��������� �����������
		SetWindowProc(g_hWnd(), BasicWndProc);
	}
	else
	{
		// ����������� �� ������, ��������� ����� ���������� ��������� ������� ���������
		m_DefWindowProc = ::DefWindowProc;
	}
}


CBasicWindow::~CBasicWindow()
{
	// ��� ������������� ������ �������������
	if (m_DefWindowProc != BasicWndProc)
	{
		SetWindowProc(g_hWnd(), m_DefWindowProc);
	}

	// �������� ���������� ���� � ������� ����
	SetWindowPtr(g_hWnd(), NULL);
}



LRESULT
CBasicWindow::OnEvent(UINT _msg, WPARAM _wp, LPARAM _lp)
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
CBasicWindow::OnEvent_Default(UINT _msg, WPARAM _wp, LPARAM _lp)
{
	return m_DefWindowProc(g_hWnd(), _msg, _wp, _lp);
}


LRESULT
CBasicWindow::OnEvent_Destroy(UINT _msg, WPARAM _wp, LPARAM _lp)
{
	return OnEvent_Default(_msg, _wp, _lp);
}


LRESULT CALLBACK
CBasicWindow::BasicWndProc(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp)
{
	// ������� ����� �������
	CBasicWindow * pWindow = CBasicWindow::GetWindowPtr(_hwnd);

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
CBasicWindow::SetWindowProc(HWND _hwnd, WNDPROC _wproc)
{
	return (WNDPROC)::SetWindowLongPtr(_hwnd, GWLP_WNDPROC, (LONG_PTR)BasicWndProc);
}


WNDPROC
CBasicWindow::GetWindowProc(HWND _hwnd)
{
	return (WNDPROC)::GetWindowLongPtr(_hwnd, GWLP_WNDPROC);
}


CBasicWindow *
CBasicWindow::SetWindowPtr(HWND _hwnd, CBasicWindow * _pwin)
{
	return (CBasicWindow*)::SetWindowLongPtr(_hwnd, GWLP_USERDATA, (LONG_PTR)_pwin);
}


CBasicWindow *
CBasicWindow::GetWindowPtr(HWND _hwnd)
{
	return (CBasicWindow*)::GetWindowLongPtr(_hwnd, GWLP_USERDATA);
}


} /* namespace Win32_GUI_NMSP */
