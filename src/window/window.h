#include <windows.h>
#include <WindowsX.h>
#include <d2d1.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <wrl\client.h>

#include "helpers.h"

#pragma comment(lib, "d2d1")



template <class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE *pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	BaseWindow() : m_hwnd(NULL) { }

	BOOL Create(LPCSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0, int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT, int nWidth = 400, int nHeight = 425, HWND hWndParent = 0, HMENU hMenu = 0
	)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);

		m_hwnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
		);

		return (m_hwnd ? TRUE : FALSE);
	}

	HWND Window() const { return m_hwnd; }

protected:

	virtual LPCSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};


struct ellipseStruct
{
	D2D1_ELLIPSE ellipse;
	D2D1_COLOR_F color;
};

class MainWindow : public BaseWindow<MainWindow>
{
	Microsoft::WRL::ComPtr<ID2D1Factory>          pFactory;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>  pBrush;
	std::vector<std::vector<ellipseStruct> > ellipses;
	std::pair<int, int>    ptMouse;


	void    CalculateLayout();
	HRESULT CreateGraphicsResources();
	void    DiscardGraphicsResources();
	void    BeginDraw();
	HRESULT EndDraw();
	void    OnPaint();
	void    Resize();
	void    LButtonDownPressed(LPARAM);
	void    LButtonUpPressed(LPARAM);
	bool    CheckThreeInARow();


public:

	MainWindow() : pFactory(), pRenderTarget(), pBrush(), ptMouse(0, 0)
	{
	}

	LPCSTR  ClassName() const { return "Circle Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};