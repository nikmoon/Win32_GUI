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
//	����������� ��� ��������� �������
//
CWindowClass::CWindowClass(ESysClassCode _code)
{
	// ��������� �������� ���������� � ��������� ������� ������
	if (!::GetClassInfoEx(NULL, SysClassNames[_code], &m_ClassInfo))
	{
		throw ::GetLastError();		// ���������� ����������
	}

	// ����� ��������������� ��������
	m_IsSystem = true;
}


//
//	�������� �����������
//
CWindowClass::CWindowClass(UINT _clstyle, WNDPROC _wproc, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
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

	// ����� ��������������� �������������, �� ��������
	m_IsSystem = false;
}


CWindowClass::~CWindowClass()
{
	// ���� ������� ����� �� �������� ���������
	if (!m_IsSystem)
	{
		// ������� �������� ����������� ������
		if (!::UnregisterClass(m_ClassInfo.lpszClassName,m_ClassInfo.hInstance))
		{
			throw ::GetLastError();		// ���������� ����������
		}
	}
}


} /* namespace Win32_GUI_NMSP */
