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

	UINT g_ClassCode() const { return m_ClassCode; };

	static CSystemWindowClass * g_SysClass_Button() { return new CSystemWindowClass(ESCC_BUTTON); };
	static CSystemWindowClass * g_SysClass_ComboBox() { return new CSystemWindowClass(ESCC_COMBOBOX); };
	static CSystemWindowClass * g_SysClass_Edit() { return new CSystemWindowClass(ESCC_EDIT); };
	static CSystemWindowClass * g_SysClass_ListBox() { return new CSystemWindowClass(ESCC_LISTBOX); };
	static CSystemWindowClass * g_SysClass_ScrollBar() { return new CSystemWindowClass(ESCC_SCROLLBAR); };
	static CSystemWindowClass * g_SysClass_Static() { return new CSystemWindowClass(ESCC_STATIC); };

};



/*
 *		------------------------------------------------
 *		Оконный класс, определенный пользователем
 *		------------------------------------------------
 */
class CUserWindowClass : public CWindowClass
{
private:
	void RegisterWindowClass(UINT _clstyle, WNDPROC _wproc, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
			HCURSOR _hcur, HBRUSH _hbr, LPCTSTR _menuname, LPCTSTR _clname, HICON _hiconsm);

public:
	CUserWindowClass(UINT _clstyle, WNDPROC _wproc, int _clsext, int _wndext, HINSTANCE _hinst, HICON _hicon,
		HCURSOR _hcur, HBRUSH _hbr, LPCTSTR _menuname, LPCTSTR _clname, HICON _hiconsm);
	CUserWindowClass(HINSTANCE _hinst, LPCTSTR _clname);

	virtual ~CUserWindowClass();
};







} /* namespace Win32_GUI_NMSP */

#endif /* CWINDOWCLASS_H_ */
