/*
 * CWindowClass.h
 *
 *  Created on: 05 дек. 2013 г.
 *      Author: Freddy
 */

#ifndef CWINDOWCLASS_H_
#define CWINDOWCLASS_H_

#include <windows.h>
#include <commctrl.h>
#include <vector>

namespace Win32_GUI_NMSP
{


//
//	Коды системных оконных классов
//
enum ESysClassCode
{
	ESCC_BUTTON		= 0,
	ESCC_COMBOBOX	= 1,
	ESCC_EDIT		= 2,
	ESCC_LISTBOX	= 3,
	ESCC_SCROLLBAR	= 4,
	ESCC_STATIC		= 5
};


//	Имена системных классов
extern LPCTSTR SysClassNames[];


//	Оконный класс
class CWindowClass
{
private:
	WNDCLASSEX m_ClassInfo;
	bool m_IsSystem;

public:
	CWindowClass(ESysClassCode _code);
	CWindowClass(UINT _clstyle, WNDPROC _wproc, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
		HCURSOR _hcur, HBRUSH _hbr, LPCTSTR _menuname, LPCTSTR _clname, HICON _hiconsm);
	virtual ~CWindowClass();
};

} /* namespace Win32_GUI_NMSP */

#endif /* CWINDOWCLASS_H_ */
