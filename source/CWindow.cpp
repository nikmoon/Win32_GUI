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

	// инициализируем поля экземпляра
	m_pWndClass = &_class;
	m_Owner = _owner;
	m_Style = _style;
	m_ExStyle = _exstyle;
	m_WndName = _wname;
	m_hMenu = _menu;
	m_hInstance = _hinst;
	m_pData = _pdata;

	// выполняем связывание окна и объекта окна
	SetWindowPtr(m_hWnd, this);

	// получим адрес текущей оконной процедуры
	m_DefWindowProc = GetWindowProc(m_hWnd);

	// если текущая оконная процедура не является базовой
	if (m_DefWindowProc != BasicWndProc)
	{
		// выполняем сабклассинг, чтобы перехватывать все события
		SetWindowProc(m_hWnd, BasicWndProc);
	}
	else
	{
		// добавляем в конец цепочки дефолтную системную оконную процедуру
		m_DefWindowProc = ::DefWindowProc;
	}

}



CWindow::~CWindow()
{
	// если окно WinAPI не уничтожено
	if (::IsWindow(m_hWnd))
	{
		throw "Call Destroy() before deleting CWindow object";	// генерируем исключение
	}

	// если последняя в цепочке оконная процедура не является дефолтной системной
	// значит был выполнен сабклассинг
	if (m_DefWindowProc != ::DefWindowProc)
	{
		// отменяем сабклассинг
		SetWindowProc(m_hWnd, m_DefWindowProc);
	}

	// отменяем связывание окна и объекта окна
	SetWindowPtr(m_hWnd, NULL);
}


LRESULT CALLBACK
CWindow::BasicWndProc(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp)
{
	// получим адрес объекта
	CWindow * pWindow = CWindow::GetWindowPtr(_hwnd);

	LRESULT ev_result;

	// если объект уже создан
	if (pWindow)
	{
		// вызываем обработчик событий объекта
		ev_result = pWindow->OnEvent(_msg, _wp, _lp);
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
	// окно должно существовать
	::DestroyWindow(m_hWnd);
}

} /* namespace Win32_GUI_NMSP */
