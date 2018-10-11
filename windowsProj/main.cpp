#include <windows.h>
#include <WindowsX.h>
#include <d2d1.h>
#include <vector>
#include <cmath>
#include <ctime>

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

	BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0, int x = CW_USEDEFAULT,
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

	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};

template <class T> void SafeRelease(T **ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = nullptr;
	}
}

class MainWindow : public BaseWindow<MainWindow>
{
	ID2D1Factory          *pFactory;
	ID2D1HwndRenderTarget *pRenderTarget;
	ID2D1SolidColorBrush  *pBrush;
	std::vector<std::vector<std::pair<D2D1_ELLIPSE, D2D1_COLOR_F> > > ellipses;
	std::pair<int, int>    ptMouse;
	
	void BeginDraw();
	HRESULT EndDraw();

	void    CalculateLayout();
	HRESULT CreateGraphicsResources();
	void    DiscardGraphicsResources();
	void    OnPaint();
	void    Resize();
	void    LButtonDownPressed(LPARAM);
	void    LButtonUpPressed(LPARAM);
	D2D1_COLOR_F GetRandColor();
	bool    CheckThreeInARow();


public:

	MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL), ptMouse(0,0)
	{
	}

	PCWSTR  ClassName() const { return L"Circle Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


bool operator == (const D2D1_COLOR_F A, const D2D1_COLOR_F B)
{
	if (A.r != B.r) return 0;
	if (A.g != B.g) return 0;
	if (A.b != B.b) return 0;
	if (A.a != B.a) return 0;
	return 1;
}

void MainWindow::BeginDraw()
{
	return pRenderTarget->BeginDraw();
}

HRESULT MainWindow::EndDraw()
{
	return pRenderTarget->EndDraw();
}

// Recalculate drawing layout when the size of the window changes.

void MainWindow::CalculateLayout()
{
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		const float radius = size.width / 20;
		float x = radius;
		float y = radius;
		ellipses.resize(10);
		
		for (auto &i : ellipses)
		{
			i.resize(10);
			for (auto &j : i)
			{
				j.first = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
				j.second = GetRandColor();
				x += radius * 2;
			}
			x = radius;
			y += radius * 2;
		}
		while (CheckThreeInARow()) {}
	}
}

HRESULT MainWindow::CreateGraphicsResources()
{
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		const D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&pRenderTarget);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}
		}
	}
	return hr;
}

void MainWindow::DiscardGraphicsResources()
{
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
}

void MainWindow::OnPaint()
{
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		::BeginPaint(m_hwnd, &ps);

		BeginDraw();

		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

		for (auto const &i : ellipses)
		{
			for (auto const &j : i)
			{
				pRenderTarget->CreateSolidColorBrush(j.second, &pBrush);
				pRenderTarget->FillEllipse(j.first, pBrush);
			}
		}

		hr = EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}
}

void MainWindow::Resize()
{
	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		CalculateLayout();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}

void MainWindow::LButtonDownPressed(LPARAM lParam)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	int jPos = xPos / (ellipses[0][0].first.radiusX * 2);
	int iPos = yPos / (ellipses[0][0].first.radiusX * 2);
	ptMouse.first = iPos;
	ptMouse.second = jPos;
}

void MainWindow::LButtonUpPressed(LPARAM lParam)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	int jPos = xPos / (ellipses[0][0].first.radiusX * 2);
	int iPos = yPos / (ellipses[0][0].first.radiusX * 2);

	if (abs(iPos - ptMouse.first) + abs(jPos - ptMouse.second) == 1)
	{
		D2D1_COLOR_F tempColor = ellipses[ptMouse.first][ptMouse.second].second;
		ellipses[ptMouse.first][ptMouse.second].second = ellipses[iPos][jPos].second;
		ellipses[iPos][jPos].second = tempColor;

		BeginDraw();

		pRenderTarget->CreateSolidColorBrush(ellipses[iPos][jPos].second, &pBrush);
		pRenderTarget->FillEllipse(&ellipses[iPos][jPos].first, pBrush);

		pRenderTarget->CreateSolidColorBrush(ellipses[ptMouse.first][ptMouse.second].second, &pBrush);
		pRenderTarget->FillEllipse(&ellipses[ptMouse.first][ptMouse.second].first, pBrush);

		EndDraw();
		while (CheckThreeInARow()) {}
	}
}

D2D1_COLOR_F MainWindow::GetRandColor()
{
	int rndVal = std::rand() % 6 + 1;
	if (rndVal == 1)
		return D2D1::ColorF(1.0f, 0, 0);
	else if (rndVal == 2)
		return D2D1::ColorF(0, 1.0f, 0);
	else if (rndVal == 3)
		return D2D1::ColorF(0, 0, 1.0f);
	else if (rndVal == 4)
		return D2D1::ColorF(1.0f, 1.0, 0);
	else if (rndVal == 5)
		return D2D1::ColorF(0, 1.0f, 1.0f);
	else if (rndVal == 6)
		return D2D1::ColorF(1.0f, 0, 1.0f);
}

bool MainWindow::CheckThreeInARow()
{
	D2D1_COLOR_F color;
	for (int i = 0; i < ellipses.size(); ++i)
	{
		for (int j = 2; j < ellipses[0].size(); ++j)
		{
			if (ellipses[i][j - 2].second == ellipses[i][j - 1].second &&
				ellipses[i][j - 2].second == ellipses[i][j].second)
			{
				BeginDraw();

				for (int k = 0; k < 3; ++k)
				{
					for (int i2 = i; i2 > 0; --i2)
					{
						color = ellipses[i2-1][j-k].second;
						pRenderTarget->CreateSolidColorBrush(color, &pBrush);
						pRenderTarget->FillEllipse(&ellipses[i2][j - k].first, pBrush);
						ellipses[i2][j - k].second = color;
					}
					color = GetRandColor();
					pRenderTarget->CreateSolidColorBrush(color, &pBrush);
					pRenderTarget->FillEllipse(&ellipses[0][j - k].first, pBrush);
					ellipses[0][j - k].second = color;
				}

				EndDraw();
				return 1;
			}
		}
	}

	for (int i = 2; i < ellipses.size(); ++i)
	{
		for (int j = 0; j < ellipses[0].size(); ++j)
		{
			if (ellipses[i-2][j].second == ellipses[i-1][j].second &&
				ellipses[i-2][j].second == ellipses[i][j].second)
			{
				BeginDraw();

				for (int i2 = i; i2 > 2; --i2)
				{
					color = ellipses[i2 - 3][j].second;
					pRenderTarget->CreateSolidColorBrush(color, &pBrush);
					pRenderTarget->FillEllipse(&ellipses[i2][j].first, pBrush);
					ellipses[i2][j].second = color;
				}

				for (int k = 0; k < 3; ++k)
				{
					color = GetRandColor();
					pRenderTarget->CreateSolidColorBrush(color, &pBrush);
					pRenderTarget->FillEllipse(&ellipses[k][j].first, pBrush);
					ellipses[k][j].second = color;
				}

				EndDraw();
				return 1;
			}
		}
	}

	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	std::srand(unsigned(std::time(0)));
	MainWindow win;

	if (!win.Create(L"Circle", WS_OVERLAPPEDWINDOW))
	{
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
		{
			return -1;  // Fail CreateWindowEx.
		}
		return 0;

	case WM_DESTROY:
		DiscardGraphicsResources();
		SafeRelease(&pFactory);
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;

	case WM_SIZE:
		Resize();
		return 0;

	case WM_LBUTTONDOWN:
		LButtonDownPressed(lParam);
		return 0;
	case WM_LBUTTONUP:
		LButtonUpPressed(lParam);
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}