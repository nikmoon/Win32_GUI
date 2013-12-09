/*
 * CWindow.h
 *
 *  Created on: 06 дек. 2013 г.
 *      Author: karpachevnk
 */

#ifndef CWINDOW_H_
#define CWINDOW_H_

#include "CWindowClass.h"


namespace Win32_GUI_NMSP
{

class CWindow
{
private:
	const CWindowClass *m_pWndClass;
	HWND m_hWnd;
	HWND m_Owner;
	DWORD m_Style;
	DWORD m_ExStyle;
	LPCTSTR m_WndName;
	HMENU m_hMenu;
	HINSTANCE m_hInstance;
	LPVOID m_pData;

public:
	CWindow(const CWindowClass &_class, DWORD _style, DWORD _exstyle, LPCTSTR _wname, HWND _owner, HMENU _menu, HINSTANCE _hinst,
			LPVOID _pdata, int _x, int _y, int _cx, int _cy);
	virtual ~CWindow();

	const CWindowClass &g_WndClass() const { return *m_pWndClass; };
	HWND g_hWnd() const { return m_hWnd; };
	HWND g_hOwner() const { return m_Owner; };
	DWORD g_Style() const { return m_Style; };
	DWORD g_ExStyle() const { return m_ExStyle; };
	LPCTSTR g_WndName() const { return m_WndName; };
	HMENU g_hMenu() const { return m_hMenu; };
	HINSTANCE g_hInstance() const { return m_hInstance; };
	const LPVOID g_pData() const { return m_pData; };

	virtual void Show() { ::ShowWindow(m_hWnd, SW_SHOW); };
	virtual void Hide() { ::ShowWindow(m_hWnd, SW_HIDE); };
	virtual void Destroy();

};

} /* namespace Win32_GUI_NMSP */

#endif /* CWINDOW_H_ */
