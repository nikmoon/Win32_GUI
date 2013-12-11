/*
 * main.cpp
 *
 *  Created on: 05 дек. 2013 г.
 *      Author: Freddy
 */

#include <windows.h>
#include "CWindowClass.h"
#include "CWindow.h"

using namespace Win32_GUI_NMSP;


class CMainWindow : public CWindow
{
private:
	CWindow * pButton;

public:
	CMainWindow(HINSTANCE _hinst, LPCTSTR _wname) : CWindow(
		WS_OVERLAPPEDWINDOW,
		0,
		_wname,
		NULL, NULL,
		_hinst, NULL,
		100, 100, 800, 600)
	{
		pButton = new CChildWindow(WS_VISIBLE, 0, "Жамкни плиз", *this,
				101, _hinst, NULL, 10, 10, 150, 60, &CChildWindow::g_SysClassPool().g_ClassButton());
	};

	~CMainWindow()
	{
		delete pButton;
	};

	virtual LRESULT OnEvent_Destroy(UINT _msg, WPARAM _wp, LPARAM _lp)
	{
		PostQuitMessage(0);
		return 0;
	};
};



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int nCmdShow)
{
	CWindow * pMainWindow = new CMainWindow(hInstance, "Main Window");
	CWindow * pWindow2 = new CMainWindow(hInstance, "Other Window");


	pMainWindow->Show();
	pWindow2->Show();

	MSG msg;
	BOOL bRes;

	while ( (bRes = ::GetMessage(&msg, NULL, 0, 0)) )
	{
		if (bRes == -1)
		{
			throw ::GetLastError();
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	delete pWindow2->Destroy_IfExists();
	delete pMainWindow->Destroy_IfExists();

	return msg.wParam;
}
