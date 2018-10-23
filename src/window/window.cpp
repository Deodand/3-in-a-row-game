#include "window.h"


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
				j.ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
				j.color = Helpers::GetRandColor();
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
	pRenderTarget.Reset();
	pBrush.Reset();
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
				pRenderTarget->CreateSolidColorBrush(j.color, &pBrush);
				pRenderTarget->FillEllipse(j.ellipse, pBrush.Get());
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

void MainWindow::BeginDraw()
{
	return pRenderTarget->BeginDraw();
}

HRESULT MainWindow::EndDraw()
{
	return pRenderTarget->EndDraw();
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
	int jPos = xPos / (ellipses[0][0].ellipse.radiusX * 2);
	int iPos = yPos / (ellipses[0][0].ellipse.radiusX * 2);
	ptMouse.first = iPos;
	ptMouse.second = jPos;
}

void MainWindow::LButtonUpPressed(LPARAM lParam)
{
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	int jPos = xPos / (ellipses[0][0].ellipse.radiusX * 2);
	int iPos = yPos / (ellipses[0][0].ellipse.radiusX * 2);

	if (abs(iPos - ptMouse.first) + abs(jPos - ptMouse.second) == 1)
	{
		D2D1_COLOR_F tempColor = ellipses[ptMouse.first][ptMouse.second].color;
		ellipses[ptMouse.first][ptMouse.second].color = ellipses[iPos][jPos].color;
		ellipses[iPos][jPos].color = tempColor;

		BeginDraw();

		pRenderTarget->CreateSolidColorBrush(ellipses[iPos][jPos].color, &pBrush);
		pRenderTarget->FillEllipse(&ellipses[iPos][jPos].ellipse, pBrush.Get());

		pRenderTarget->CreateSolidColorBrush(ellipses[ptMouse.first][ptMouse.second].color, &pBrush);
		pRenderTarget->FillEllipse(&ellipses[ptMouse.first][ptMouse.second].ellipse, pBrush.Get());

		EndDraw();
		while (CheckThreeInARow()) {}
	}
}

bool MainWindow::CheckThreeInARow()
{
	D2D1_COLOR_F color;
	for (int i = 0; i < ellipses.size(); ++i)
	{
		for (int j = 2; j < ellipses[0].size(); ++j)
		{
			if (Helpers::ColorEqual(ellipses[i][j - 2].color, ellipses[i][j - 1].color) &&
				Helpers::ColorEqual(ellipses[i][j - 2].color, ellipses[i][j].color))
			{
				BeginDraw();

				for (int k = 0; k < 3; ++k)
				{
					for (int i2 = i; i2 > 0; --i2)
					{
						color = ellipses[i2 - 1][j - k].color;
						pRenderTarget->CreateSolidColorBrush(color, &pBrush);
						pRenderTarget->FillEllipse(&ellipses[i2][j - k].ellipse, pBrush.Get());
						ellipses[i2][j - k].color = color;
					}
					color = Helpers::GetRandColor();
					pRenderTarget->CreateSolidColorBrush(color, &pBrush);
					pRenderTarget->FillEllipse(&ellipses[0][j - k].ellipse, pBrush.Get());
					ellipses[0][j - k].color = color;
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
			if (Helpers::ColorEqual(ellipses[i - 2][j].color, ellipses[i - 1][j].color) &&
				Helpers::ColorEqual(ellipses[i - 2][j].color, ellipses[i][j].color))
			{
				BeginDraw();

				for (int i2 = i; i2 > 2; --i2)
				{
					color = ellipses[i2 - 3][j].color;
					pRenderTarget->CreateSolidColorBrush(color, &pBrush);
					pRenderTarget->FillEllipse(&ellipses[i2][j].ellipse, pBrush.Get());
					ellipses[i2][j].color = color;
				}

				for (int k = 0; k < 3; ++k)
				{
					color = Helpers::GetRandColor();
					pRenderTarget->CreateSolidColorBrush(color, &pBrush);
					pRenderTarget->FillEllipse(&ellipses[k][j].ellipse, pBrush.Get());
					ellipses[k][j].color = color;
				}

				EndDraw();
				return 1;
			}
		}
	}

	return 0;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, pFactory.GetAddressOf())))
		{
			return -1;  // Fail CreateWindowEx.
		}
		return 0;

	case WM_DESTROY:
		DiscardGraphicsResources();
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