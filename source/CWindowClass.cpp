/*
 * CWindowClass.cpp
 *
 *  Created on: 05 ���. 2013 �.
 *      Author: Freddy
 */

#include "CWindowClass.h"

namespace Win32_GUI_NMSP
{


//
//	����� ��������� ������� �������
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
//	����������� ��� ��������� ������� �������
//
CSystemWindowClass::CSystemWindowClass(UINT _esccode)
{
	// �������� ���������� ���
	if (_esccode >= ESCC_SYSCLASSCOUNT)
	{
		throw -1;
	}

	// ��������� �������� ���������� � ��������� ������� ������
	if (!::GetClassInfoEx(NULL, SysClassNames[_esccode], &m_ClassInfo))
	{
		throw ::GetLastError();		// ���������� ����������
	}
	m_ClassCode = _esccode;
}


CSystemWindowClassPool::CSystemWindowClassPool()
{
	// �������� ������ ��� ������ ��������� �������
	m_SysClassPool = new CSystemWindowClass*[ESCC_SYSCLASSCOUNT];

	// ������� ���������� ������� ���������� ������
	for(int i = 0; i < ESCC_SYSCLASSCOUNT; i++)
	{
		m_SysClassPool[i] = new CSystemWindowClass(i);
	}
}


CSystemWindowClassPool::~CSystemWindowClassPool()
{
	// ������� ���������� ��������� �������
	for(int i = 0; i < ESCC_SYSCLASSCOUNT; i++)
	{
		delete m_SysClassPool[i];
	}

	// ����������� ������ �������
	delete [] m_SysClassPool;
}


//
//	����������� ��� ���������������� ������� �������
//
CUserWindowClass::CUserWindowClass(UINT _clstyle, WNDPROC _wproc, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
	HCURSOR _hcur, HBRUSH _hbr, LPCTSTR _menuname, LPCTSTR _clname, HICON _hiconsm)
{
	// ���������� � ����������� �������� ������
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

	// ������� ���������������� �����
	if (!::RegisterClassEx(&m_ClassInfo))
	{
		throw ::GetLastError();		// ���������� ����������
	}
}


CUserWindowClass::~CUserWindowClass()
{
	// ������� �������� ����������� ������
	if (!::UnregisterClass(m_ClassInfo.lpszClassName,m_ClassInfo.hInstance))
	{
		throw ::GetLastError();		// ���������� ����������
	}
}

} /* namespace Win32_GUI_NMSP */
