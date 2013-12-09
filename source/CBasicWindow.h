/*
 * CBasicWindow.h
 *
 *  Created on: 09 дек. 2013 г.
 *      Author: karpachevnk
 */

#ifndef CBASICWINDOW_H_
#define CBASICWINDOW_H_

#include "CWindowClass.h"
#include "CWindow.h"

namespace Win32_GUI_NMSP
{

class CBasicWindow : public CWindow
{
public:
	CBasicWindow(const CWindowClass &_class, DWORD _style, DWORD _exstyle, LPCTSTR _wname, HWND _owner, HMENU _menu,
			HINSTANCE _hinst, LPVOID _pdata, int _x, int _y, int _cx, int _cy);
	CBasicWindow();
	virtual ~CBasicWindow();

	virtual LRESULT OnEvent(UINT _msg, WPARAM _wp, LPARAM _lp);
	virtual LRESULT OnEvent_Default(UINT _msg, WPARAM _wp, LPARAM _lp);

	virtual LRESULT OnEvent_Destroy(UINT _msg, WPARAM _wp, LPARAM _lp);

	static LRESULT CALLBACK BasicWndProc(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp);
	static WNDPROC SetWindowProc(HWND _hwnd, WNDPROC _wproc);
	static WNDPROC GetWindowProc(HWND _hwnd);
	static CBasicWindow * SetWindowPtr(HWND _hwnd, CBasicWindow * _pwin);
	static CBasicWindow * GetWindowPtr(HWND _hwnd);

private:
	WNDPROC m_DefWindowProc;
};

} /* namespace Win32_GUI_NMSP */

#endif /* CBASICWINDOW_H_ */
