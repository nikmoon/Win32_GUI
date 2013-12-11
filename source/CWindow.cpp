/*
 * CWindow.cpp
 *
 *  Created on: 06 ���. 2013 �.
 *      Author: karpachevnk
 */

#include "CWindow.h"

namespace Win32_GUI_NMSP
{

CWindow::CWindow(DWORD _style, DWORD _exstyle, LPCTSTR _wname, HWND _owner, HMENU _menu, HINSTANCE _hinst, LPVOID _pdata,
		int _x, int _y, int _cx, int _cy, const CWindowClass * _class)
{
	// ���� ������� ����� �� �����
	if (_class == NULL)
	{
		// ����� ������������ ��������� ������� �����
		_class = g_DefWindowClass(_hinst);
	}

	// ������� ������� ����
	m_hWnd = ::CreateWindowEx(
		_exstyle,
		_class->g_ClassName(),
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
	m_pWndClass = _class;
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

	// ���� ����� ��������� ������� �����
	if (m_pWndClass == sm_pDefWindowClass)
	{
		// ����������� ������� ����������� ������ � ��������� ������� �������
		sm_DefClassWndCount++;
	}
}


CWindow::~CWindow()
{
	// ���� ���� WinAPI �� ����������
	if (::IsWindow(m_hWnd))
	{
		::MessageBox(m_hWnd, "Call Destroy() before deleting CWindow object", "��������", MB_OK);
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

	// ���� ��������� ���� ��������� ������� �����
	if (m_pWndClass == sm_pDefWindowClass)
	{
		// ��������� ������� �����������
		sm_DefClassWndCount--;

		// ���� ����� ���� ������ ���
		if (sm_DefClassWndCount == 0)
		{
			// ������� ��������� ������� �����
			delete sm_pDefWindowClass;
			sm_pDefWindowClass = NULL;
		}
	}
}


// ������������ �� ��������� ������� �����
CUserWindowClass *
CWindow::sm_pDefWindowClass = NULL;


// ������� ����������� ���� � ��������� ������� �������
UINT
CWindow::sm_DefClassWndCount = 0;


// ���������� ��������� ������� �����
const CUserWindowClass *
CWindow::g_DefWindowClass(HINSTANCE _hinst)
{
	// ���� ����� ��� �� ���������������
	if (sm_pDefWindowClass == NULL)
	{
		// ������������ � ������� ���������
		sm_pDefWindowClass = CreateWndClass(
				CS_HREDRAW|CS_VREDRAW,
				0, 0,
				_hinst,
				::LoadIcon(NULL, IDI_APPLICATION),
				::LoadCursor(NULL, IDC_ARROW),
				(HBRUSH)(COLOR_BACKGROUND+1),
				NULL,
				"DefWindowClassName",
				::LoadIcon(NULL, IDI_APPLICATION));
	}

	return sm_pDefWindowClass;
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


// �������� �������� ������ � ��������� ������� ����������
CUserWindowClass *
CWindow::CreateWndClass(UINT _clstyle, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
		HCURSOR _hcur, HBRUSH _hbr, LPCTSTR _menuname, LPCTSTR _clname, HICON _hiconsm)
{
	return new CUserWindowClass(
			_clstyle,
			&BasicWndProc,
			_clsext,
			_wndext,
			_hinst,
			_hicon,
			_hcur,
			_hbr,
			_menuname,
			_clname,
			_hiconsm);
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
	return (CWindow*)::SetWindowLongPtr(_hwnd, GWLP_USERDATA, (LONG_PTR)_pwin);
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
	CWindow * wchild;

	switch (_msg)
	{
		case WM_COMMAND:
			if (_lp != 0)	// ��������� �� ���������� ����
			{
				wchild = GetWindowPtr((HWND)_lp);
				if (!wchild)
				{
					throw -1;
				}
				ev_result = OnEvent_ChildCommand(*wchild, _msg, _wp, _lp);
			}
			else
			{
				ev_result = OnEvent_Default(_msg, _wp, _lp);
			}
			break;
		case WM_NOTIFY:
			ev_result = OnEvent_ChildNotify(_msg, _wp, _lp);
			break;
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

LRESULT
CWindow::OnEvent_ChildCommand(CWindow &_child, UINT _msg, WPARAM _wp, LPARAM _lp)
{
	return OnEvent_Default(_msg, _wp, _lp);
}

LRESULT
CWindow::OnEvent_ChildNotify(UINT _msg, WPARAM _wp, LPARAM _lp)
{
	return OnEvent_Default(_msg, _wp, _lp);
}




// ��� ��������� ������� �������
CSystemWindowClassPool *
CChildWindow::sm_pSysClassPool = NULL;

// ������� ����������� CChildWindow � �����������
UINT
CChildWindow::sm_ChildWndCount = 0;


CChildWindow::CChildWindow(DWORD _style, DWORD _exstyle, LPCTSTR _wname, const CWindow &_owner, UINT _id, HINSTANCE _hinst,
		LPVOID _pdata, int _x, int _y, int _cx, int _cy, const CWindowClass * _class)
		: CWindow((_style | WS_CHILD) & (~WS_POPUP), _exstyle, _wname, _owner.g_hWnd(), (HMENU)_id, _hinst, _pdata, _x, _y, _cx, _cy, _class)
{
	// ����������� ������� �����������
	sm_ChildWndCount++;
}


CChildWindow::~CChildWindow()
{
	// ��������� ������� �����������
	sm_ChildWndCount--;

	// ���� ����������� ������ �� ��������
	if (sm_ChildWndCount == 0)
	{
		// ������� ��� ��������� ������� �������
		delete sm_pSysClassPool;
		sm_pSysClassPool = NULL;
	}
}


const CSystemWindowClassPool &
CChildWindow::g_SysClassPool()
{
	// ���� ��� ��������� ������� ������� ��� �� ������
	if (sm_pSysClassPool == NULL)
	{
		sm_pSysClassPool = new CSystemWindowClassPool();
	}

	return *sm_pSysClassPool;
}

} /* namespace Win32_GUI_NMSP */
