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



//
//	����������� ��� ���������������� ������� �������
//
CUserWindowClass::CUserWindowClass(UINT _clstyle, WNDPROC _wproc, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
	HCURSOR _hcur, HBRUSH _hbr, LPCTSTR _menuname, LPCTSTR _clname, HICON _hiconsm)
{
	RegisterWindowClass(_clstyle, _wproc, _clsext, _wndext, _hinst, _hicon, _hcur, _hbr, _menuname, _clname, _hiconsm);
}


CUserWindowClass::CUserWindowClass(HINSTANCE _hinst, LPCTSTR _clname)
{
	RegisterWindowClass(
		CS_HREDRAW | CS_VREDRAW,
		NULL,
		0, 0,
		_hinst,
		::LoadIcon(NULL,IDI_APPLICATION),
		::LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_BACKGROUND+1),
		NULL,
		_clname,
		::LoadIcon(NULL, IDI_APPLICATION));
}


CUserWindowClass::~CUserWindowClass()
{
	// ������� �������� ����������� ������
	if (!::UnregisterClass(m_ClassInfo.lpszClassName,m_ClassInfo.hInstance))
	{
		throw ::GetLastError();		// ���������� ����������
	}
}

void
CUserWindowClass::RegisterWindowClass(UINT _clstyle, WNDPROC _wproc, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
		HCURSOR _hcur, HBRUSH _hbr, LPCTSTR _menuname, LPCTSTR _clname, HICON _hiconsm)
{
	// ���������� � ����������� �������� ������
	m_ClassInfo.cbSize = sizeof(WNDCLASSEX);
	m_ClassInfo.style = _clstyle;
	m_ClassInfo.lpfnWndProc = (_wproc == NULL) ? &::DefWindowProc : _wproc;
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

} /* namespace Win32_GUI_NMSP */
