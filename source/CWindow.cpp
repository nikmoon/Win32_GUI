/*
 * CWindow.cpp
 *
 *  Created on: 06 дек. 2013 г.
 *      Author: karpachevnk
 */

#include "CWindow.h"

namespace Win32_GUI_NMSP
{

CWindow::CWindow(DWORD _style, DWORD _exstyle, LPCTSTR _wname, HWND _owner, HMENU _menu, HINSTANCE _hinst, LPVOID _pdata,
		int _x, int _y, int _cx, int _cy, const CWindowClass * _class)
{
	// если оконный класс не задан
	if (_class == NULL)
	{
		// будем использовать дефолтный оконный класс
		_class = g_DefWindowClass(_hinst);
	}

	// пробуем создать окно
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

	// если не удалось создать окно
	if (!m_hWnd)
	{
		throw ::GetLastError();		// генерируем исключение
	}

	// инициализируем поля экземпляра
	m_pWndClass = _class;
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

	// если задан дефолтный оконный класс
	if (m_pWndClass == sm_pDefWindowClass)
	{
		// увеличиваем счетчик экземпляров класса с дефолтным оконным классом
		sm_DefClassWndCount++;
	}
}


CWindow::~CWindow()
{
	// если окно WinAPI не уничтожено
	if (::IsWindow(m_hWnd))
	{
		::MessageBox(m_hWnd, "Call Destroy() before deleting CWindow object", "Внимание", MB_OK);
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

	// если экземпляр имел дефолтный оконный класс
	if (m_pWndClass == sm_pDefWindowClass)
	{
		// уменьшаем счетчик экземпляров
		sm_DefClassWndCount--;

		// если таких окон больше нет
		if (sm_DefClassWndCount == 0)
		{
			// удаляем дефолтный оконный класс
			delete sm_pDefWindowClass;
			sm_pDefWindowClass = NULL;
		}
	}
}


// Используемый по умолчанию оконный класс
CUserWindowClass *
CWindow::sm_pDefWindowClass = NULL;


// Счетчик экземпляров окон с дефолтным оконным классом
UINT
CWindow::sm_DefClassWndCount = 0;


// Возвращаем дефолтный оконный класс
const CUserWindowClass *
CWindow::g_DefWindowClass(HINSTANCE _hinst)
{
	// если класс еще не зарегистрирован
	if (sm_pDefWindowClass == NULL)
	{
		// регистрируем и создаем экземпляр
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


// Создание оконного класса с дефолтной оконной процедурой
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
			if (_lp != 0)	// сообщение от владеемого окна
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




// Пул системных оконных классов
CSystemWindowClassPool *
CChildWindow::sm_pSysClassPool = NULL;

// Счетчик экземпляров CChildWindow и наследуемых
UINT
CChildWindow::sm_ChildWndCount = 0;


CChildWindow::CChildWindow(DWORD _style, DWORD _exstyle, LPCTSTR _wname, const CWindow &_owner, UINT _id, HINSTANCE _hinst,
		LPVOID _pdata, int _x, int _y, int _cx, int _cy, const CWindowClass * _class)
		: CWindow((_style | WS_CHILD) & (~WS_POPUP), _exstyle, _wname, _owner.g_hWnd(), (HMENU)_id, _hinst, _pdata, _x, _y, _cx, _cy, _class)
{
	// увеличиваем счетчик экземпляров
	sm_ChildWndCount++;
}


CChildWindow::~CChildWindow()
{
	// уменьшаем счетчик экземпляров
	sm_ChildWndCount--;

	// если экземпляров больше не осталось
	if (sm_ChildWndCount == 0)
	{
		// удаляем пул системных оконных классов
		delete sm_pSysClassPool;
		sm_pSysClassPool = NULL;
	}
}


const CSystemWindowClassPool &
CChildWindow::g_SysClassPool()
{
	// если пул системных оконных классов еще не создан
	if (sm_pSysClassPool == NULL)
	{
		sm_pSysClassPool = new CSystemWindowClassPool();
	}

	return *sm_pSysClassPool;
}

} /* namespace Win32_GUI_NMSP */
