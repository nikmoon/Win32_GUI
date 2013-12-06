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

LRESULT CALLBACK MyWndProc(HWND _hwnd, UINT _msg, WPARAM _wp, LPARAM _lp)
{
	LRESULT res;
	switch (_msg)
	{
		case WM_DESTROY:
			::PostQuitMessage(0);
			res = ::DefWindowProc(_hwnd, _msg, _wp, _lp);
			break;
		default:
			res = ::DefWindowProc(_hwnd, _msg, _wp, _lp);
			break;
	}
	return res;
}


class CMainWindowClass : public CWindowClass
{
public:
	CMainWindowClass(HINSTANCE _hinst);
};

CMainWindowClass::CMainWindowClass(HINSTANCE _hinst) : CWindowClass(
		CS_HREDRAW|CS_VREDRAW,
		&MyWndProc,
		0, 0,
		_hinst,
		::LoadIcon(NULL, IDI_APPLICATION),
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_BACKGROUND+1),
		NULL,
		"MainWindowClass",
		::LoadIcon(NULL, IDI_APPLICATION))
{

}


class CMainWindow : public CWindow
{
private:
	CWindowClass *m_pClassButton;
public:
	CMainWindow(CWindowClass &_class, HINSTANCE _hinst);
	~CMainWindow();
};


CMainWindow::CMainWindow(CWindowClass &_class, HINSTANCE _hinst) : CWindow(
		_class,
		WS_OVERLAPPEDWINDOW,
		0,
		"Main Window",
		NULL,
		NULL,
		_hinst,
		NULL,
		100, 100, 800, 600)
{
	m_pClassButton = new CWindowClass(ESCC_BUTTON);

	AddChildWindow(*m_pClassButton, WS_CHILD|WS_VISIBLE, 0, "Жамкни епт!", NULL, NULL, 10, 10, 100, 30);
	AddChildWindow(*m_pClassButton, WS_CHILD|WS_VISIBLE, 0, "Жамкни епт!", NULL, NULL, 10, 40, 100, 30);
}

CMainWindow::~CMainWindow()
{
	// уничтожаем все дочерние окна
	ClearChildList();

	// удаляем созданные оконные классы
	delete m_pClassButton;
}


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
