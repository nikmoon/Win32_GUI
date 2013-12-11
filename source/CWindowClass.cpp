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
//	Конструктор для системных оконных классов
//
CSystemWindowClass::CSystemWindowClass(UINT _esccode)
{
	// проверим переданный код
	if (_esccode >= ESCC_SYSCLASSCOUNT)
	{
		throw -1;
	}

	// попробуем получить информацию о системном оконном классе
	if (!::GetClassInfoEx(NULL, SysClassNames[_esccode], &m_ClassInfo))
	{
		throw ::GetLastError();		// генерируем исключение
	}
	m_ClassCode = _esccode;
}


CSystemWindowClassPool::CSystemWindowClassPool()
{
	// выделяем память под массив системных классов
	m_SysClassPool = new CSystemWindowClass*[ESCC_SYSCLASSCOUNT];

	// создаем экземпляры каждого системного класса
	for(int i = 0; i < ESCC_SYSCLASSCOUNT; i++)
	{
		m_SysClassPool[i] = new CSystemWindowClass(i);
	}
}


CSystemWindowClassPool::~CSystemWindowClassPool()
{
	// удаляем экземпляры системных классов
	for(int i = 0; i < ESCC_SYSCLASSCOUNT; i++)
	{
		delete m_SysClassPool[i];
	}

	// освобождаем память массива
	delete [] m_SysClassPool;
}


//
//	Конструктор для пользовательских оконных классов
//
CUserWindowClass::CUserWindowClass(UINT _clstyle, WNDPROC _wproc, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
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
}


CUserWindowClass::~CUserWindowClass()
{
	// пробуем отменить регистрацию класса
	if (!::UnregisterClass(m_ClassInfo.lpszClassName,m_ClassInfo.hInstance))
	{
		throw ::GetLastError();		// генерируем исключение
	}
}

} /* namespace Win32_GUI_NMSP */
