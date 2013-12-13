/*
 * CWindow.cpp
 *
 *  Created on: 06 дек. 2013 г.
 *      Author: karpachevnk
 */

#include "CWindow.h"

namespace Win32_GUI_NMSP
{

CWindow::CWindow(const CWindowClass &_class, DWORD _style, DWORD _exstyle, LPCTSTR _wname, HWND _owner, HMENU _menu,
		HINSTANCE _hinst, LPVOID _pdata, int _x, int _y, int _cx, int _cy)
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

	// настройка системы обработки событий
	InitEventSystem(_class);

	// инициализируем поля экземпляра
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
		::MessageBox(m_hWnd, "Call Destroy() before deleting CWindow object", "Внимание", MB_OK);
		throw "Call Destroy() before deleting CWindow object";	// генерируем исключение
	}
}



void
CWindow::InitEventSystem(const CWindowClass &_class)
{
	// выполняем связывание окна и объекта окна
	SetWindowPtr(m_hWnd, this);

	// если в оконном классе задана пользовательская оконная процедура
	if (_class.g_Proc() != EventSystemWndProc)
	{
		// подставляем оконную процедуру, реализующую обработку событий
		SetWindowProc(m_hWnd, EventSystemWndProc);

		// в качестве дефолтной будем использовать пользовательскую оконную процедуру
		m_DefWindowProc = _class.g_Proc();
	}
	else
	{
		// в качестве дефолтной будем использовать системную дефолтную оконную процедуру
		m_DefWindowProc = &::DefWindowProc;
	}
}


void
CWindow::DoneEventSystem()
{
	// если был сделан сабклассинг
	if (m_pWndClass->g_Proc() != EventSystemWndProc)
	{
		// возвращаем прежний адрес оконной процедуры
		SetWindowProc(m_hWnd, m_pWndClass->g_Proc());
	}

	// отменяем связывание окна и объекта окна
	SetWindowPtr(m_hWnd, NULL);
}



LRESULT CALLBACK
CWindow::EventSystemWndProc(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp)
{
	// получим адрес объекта
	CWindow * pWindow = CWindow::GetWindowPtr(_hwnd);

	LRESULT ev_result;

	// если объект уже создан
	if (pWindow)
	{
		// вызываем обработчик событий объекта
		ev_result = pWindow->OnEvent(_msg, _wp, _lp);

		// если окно уничтожено
		if (_msg == WM_NCDESTROY)
		{
			// отключаем систему обработки событий
			pWindow->DoneEventSystem();
		}
	}
	else
	{
		// вызываем дефолтный системный обработчик
		ev_result = ::DefWindowProc(_hwnd, _msg, _wp, _lp);
	}

	return ev_result;
}


WNDPROC
CWindow::SetWindowProc(HWND _hwnd, WNDPROC _wproc)
{
	return (WNDPROC)::SetWindowLongPtr(_hwnd, GWLP_WNDPROC, (LONG_PTR)_wproc);
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
			if (_lp != 0)	// сообщение от дочернего (WS_CHILD) окна
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
	return m_DefWindowProc(m_hWnd, _msg, _wp, _lp);
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


int
CWindow::GetActionCode(UINT _msg, WPARAM _wp, LPARAM _lp)
{
	return -1;
}

} /* namespace Win32_GUI_NMSP */
