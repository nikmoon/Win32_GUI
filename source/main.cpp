/*
 * main.cpp
 *
 *  Created on: 05 дек. 2013 г.
 *      Author: Freddy
 */

#include <windows.h>
#include "CWindowClass.h"
#include "CWindow.h"
#include "CBasicWindow.h"

using namespace Win32_GUI_NMSP;


class CMainWindowClass : public CWindowClass
{
public:
	CMainWindowClass(HINSTANCE _hinst) : CWindowClass
	(
		CS_HREDRAW|CS_VREDRAW,
		&CBasicWindow::BasicWndProc,
		0, 0,
		_hinst,
		::LoadIcon(NULL, IDI_APPLICATION),
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_BACKGROUND+1),
		NULL,
		"MainWindowClass",
		::LoadIcon(NULL, IDI_APPLICATION)
	) {};
};


class CMainWindow : public CBasicWindow
{

public:
	CMainWindow(CWindowClass &_class, HINSTANCE _hinst) : CBasicWindow
	(
		_class,
		WS_OVERLAPPEDWINDOW,
		0,
		"Main Window",
		NULL,
		NULL,
		_hinst,
		NULL,
		100, 100, 800, 600
	) {};

	~CMainWindow() {};

	virtual LRESULT OnEvent_Destroy(UINT _msg, WPARAM _wp, LPARAM _lp)
	{
		PostQuitMessage(0);
		return 0;
	};
};



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int nCmdShow)
{
	CWindowClass * pMainWindowClass = new CMainWindowClass(hInstance);
	CWindow * pMainWindow = new CMainWindow(*pMainWindowClass, hInstance);


	pMainWindow->Show();

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

	delete pMainWindow;
	delete pMainWindowClass;

	return msg.wParam;
}
