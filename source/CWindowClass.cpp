/*
 * CWindowClass.cpp
 *
 *  Created on: 05 дек. 2013 г.
 *      Author: Freddy
 */

#include "CWindowClass.h"

namespace Win32_GUI_NMSP
{


//
//	Имена системных оконных классов
//
LPCTSTR SysClassNames[] =
{
	WC_BUTTON,
	WC_COMBOBOX,
	WC_EDIT,
	WC_LISTBOX,
	WC_SCROLLBAR,
	WC_STATIC
};



//
//	Конструктор для системных классов
//
CWindowClass::CWindowClass(ESysClassCode _code)
{
	// попробуем получить информацию о системном оконном классе
	if (!::GetClassInfoEx(NULL, SysClassNames[_code], &m_ClassInfo))
	{
		throw ::GetLastError();		// генерируем исключение
	}

	// класс зарегистрирован системой
	m_IsSystem = true;
}


//
//	Основной конструктор
//
CWindowClass::CWindowClass(UINT _clstyle, WNDPROC _wproc, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
	HCURSOR _hcur, HBRUSH _hbr, LPCTSTR _menuname, LPCTSTR _clname, HICON _hiconsm)
{
	// подготовка к регистрации оконного класса
	m_ClassInfo.cbSize = sizeof(WNDCLASSEX);
	m_ClassInfo.style = _clstyle;
	m_ClassInfo.lpfnWndProc = _wproc;
	m_ClassInfo.cbClsExtra = _clsext;
	m_ClassInfo.cbWndExtra = _wndext;
	m_ClassInfo.hInstance = _hinst;
	m_ClassInfo.hIcon = _hicon;
	m_ClassInfo.hCursor = _hcur;
	m_ClassInfo.hbrBackground = _hbr;
	m_ClassInfo.lpszMenuName = _menuname;
	m_ClassInfo.lpszClassName = _clname;
	m_ClassInfo.hIconSm = _hiconsm;

	// пробуем зарегистрировать класс
	if (!::RegisterClassEx(&m_ClassInfo))
	{
		throw ::GetLastError();		// генерируем исключение
	}

	// класс зарегистрирован пользователем, не системой
	m_IsSystem = false;
}


CWindowClass::~CWindowClass()
{
	// если оконный класс не является системным
	if (!m_IsSystem)
	{
		// пробуем отменить регистрацию класса
		if (!::UnregisterClass(m_ClassInfo.lpszClassName,m_ClassInfo.hInstance))
		{
			throw ::GetLastError();		// генерируем исключение
		}
	}
}


} /* namespace Win32_GUI_NMSP */
