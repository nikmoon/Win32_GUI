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


class CButtonWindow : public CWindow
{
private:
	int m_OnClick_Code;

public:
	CButtonWindow(HINSTANCE _hinst, LPCTSTR _wname, CWindow &_owner, int _x, int _y, int _cx, int _cy);
	virtual ~CButtonWindow();

	void SetActionCode_OnClick(int _code) { m_OnClick_Code = _code; };
	virtual int GetActionCode(UINT _msg, WPARAM _wp, LPARAM _lp);
};


CButtonWindow::CButtonWindow(HINSTANCE _hinst, LPCTSTR _wname, CWindow &_owner, int _x, int _y, int _cx, int _cy)
	: CWindow (
		*CSystemWindowClass::g_SysClass_Button(),
		WS_CHILD | WS_VISIBLE,
		0,
		_wname,
		_owner.g_hWnd(),
		NULL,
		_hinst,
		NULL,
		_x, _y, _cx, _cy)
{
	m_OnClick_Code = -1;
}


CButtonWindow::~CButtonWindow()
{
	delete &g_WndClass();
}


int
CButtonWindow::GetActionCode(UINT _msg, WPARAM _wp, LPARAM _lp)
{
	switch (HIWORD(_wp))
	{
		case BN_CLICKED:
			return m_OnClick_Code;
		default:
			return -1;
	}
}


class CMainWindow : public CWindow
{
private:
	CButtonWindow * pButton1;

public:
	CMainWindow(HINSTANCE _hinst, LPCTSTR _wname, const CWindowClass &_class) : CWindow(
		_class,
		WS_OVERLAPPEDWINDOW,
		0,
		_wname,
		NULL, NULL,
		_hinst, NULL,
		100, 100, 800, 600)
	{
		pButton1 = new CButtonWindow(_hinst, "Кнопка1", *this, 10, 10, 100, 35);
		pButton1->SetActionCode_OnClick(0);
	};

	~CMainWindow()
	{
		delete pButton1;
	};

	virtual LRESULT OnEvent_Destroy(UINT _msg, WPARAM _wp, LPARAM _lp)
	{
		PostQuitMessage(0);
		return 0;
	};


	virtual LRESULT OnEvent_ChildCommand(CWindow &_child, UINT _msg, WPARAM _wp, LPARAM _lp)
	{
		switch (_child.GetActionCode(_msg, _wp, _lp))
		{
			case 0:
				::MessageBox(g_hWnd(), "Нажата кнопочка", "Внимание", MB_OK);
				break;
			default:
				break;
		}

		return CWindow::OnEvent_ChildCommand(_child, _msg, _wp, _lp);
	}
};



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR CmdLine, int nCmdShow)
{
	CWindowClass * pClass = new CUserWindowClass(hInstance, "MainWindowClass");
	CWindow * pMainWindow = new CMainWindow(hInstance, "Main Window", *pClass);
	CWindow * pWindow2 = new CMainWindow(hInstance, "Other Window", *pClass);


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
	delete pClass;

	return msg.wParam;
}
