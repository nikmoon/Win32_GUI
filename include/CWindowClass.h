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


/*		-----------------------------------------
 *		Оконный класс
 *		-----------------------------------------
 *
 *		Создает обертку для оконного класса WinAPI.
 *		Для классов, зарегистрированных системой, можно создавать сколько угодно экземпляров.
 *		Для собственных классов можно создавать лишь по одному классу на каждое имя класса.
 *
 *		В случае возникновения ошибок генерируется исключение int со значением равным ::GetLastError()
 */
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

	const WNDCLASSEX &g_ClassInfo() const { return m_ClassInfo; };
	UINT g_Style() const { return m_ClassInfo.style; };
	WNDPROC g_Proc() const { return m_ClassInfo.lpfnWndProc; };
	HINSTANCE g_hInst() const { return m_ClassInfo.hInstance; };
	HICON g_hIcon() const { return m_ClassInfo.hIcon; };
	HICON g_hIconSm() const { return m_ClassInfo.hIconSm; };
	HCURSOR g_hCursor() const { return m_ClassInfo.hCursor; };
	HBRUSH g_hBrush() const { return m_ClassInfo.hbrBackground; };
	LPCTSTR g_MenuName() const { return m_ClassInfo.lpszMenuName; };
	LPCTSTR g_ClassName() const { return m_ClassInfo.lpszClassName; };
};

} /* namespace Win32_GUI_NMSP */

#endif /* CWINDOWCLASS_H_ */
