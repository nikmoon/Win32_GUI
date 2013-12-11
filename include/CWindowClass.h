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

namespace Win32_GUI_NMSP
{


//
//	Коды системных оконных классов
//
#define		ESCC_BUTTON			0
#define		ESCC_COMBOBOX		(ESCC_BUTTON + 1)
#define		ESCC_EDIT			(ESCC_COMBOBOX + 1)
#define		ESCC_LISTBOX		(ESCC_EDIT + 1)
#define		ESCC_SCROLLBAR		(ESCC_LISTBOX + 1)
#define		ESCC_STATIC			(ESCC_SCROLLBAR + 1)
#define		ESCC_SYSCLASSCOUNT	(ESCC_STATIC + 1)

//	Имена системных классов
extern LPCTSTR SysClassNames[];


/*		--------------------------------------------
 *		Базовый класс для оконных классов WinAPI
 *		--------------------------------------------
 *
 *		Должен наследоваться.
 */
class CWindowClass
{
protected:
	WNDCLASSEX m_ClassInfo;

	CWindowClass() {};

public:
	virtual ~CWindowClass() {};

public:
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


/*
 *		-------------------------------------
 *		Системный оконный класс
 *		-------------------------------------
 */
class CSystemWindowClass : public CWindowClass
{
private:
	UINT m_ClassCode;

public:
	CSystemWindowClass(UINT _esccode);
	virtual ~CSystemWindowClass() {};
};


/*
 * 		-------------------------------------
 *		Пул системных оконных классов
 *		-------------------------------------
*/
class CSystemWindowClassPool
{
private:
	CSystemWindowClass ** m_SysClassPool;

	const CSystemWindowClass &GetSysClass(UINT _code) const { return *(m_SysClassPool[_code]); };

public:
	CSystemWindowClassPool();
	virtual ~CSystemWindowClassPool();

	const CSystemWindowClass &g_ClassButton() const { return GetSysClass(ESCC_BUTTON); };
	const CSystemWindowClass &g_ClassCombobox() const { return GetSysClass(ESCC_COMBOBOX); };
	const CSystemWindowClass &g_ClassEdit() const { return GetSysClass(ESCC_EDIT); };
	const CSystemWindowClass &g_ClassListbox() const { return GetSysClass(ESCC_LISTBOX); };
	const CSystemWindowClass &g_ClassScrollbar() const { return GetSysClass(ESCC_SCROLLBAR); };
	const CSystemWindowClass &g_ClassStatic() const { return GetSysClass(ESCC_STATIC); };
};



/*
 *		------------------------------------------------
 *		Оконный класс, определенный пользователем
 *		------------------------------------------------
 */
class CUserWindowClass : public CWindowClass
{
public:
	CUserWindowClass(UINT _clstyle, WNDPROC _wproc, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
		HCURSOR _hcur, HBRUSH _hbr, LPCTSTR _menuname, LPCTSTR _clname, HICON _hiconsm);
	virtual ~CUserWindowClass();
};







} /* namespace Win32_GUI_NMSP */

#endif /* CWINDOWCLASS_H_ */
