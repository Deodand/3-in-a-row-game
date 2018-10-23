#include <windows.h>
#include <WindowsX.h>
#include <d2d1.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <wrl\client.h>

#pragma comment(lib, "d2d1")


namespace Helpers
{
	bool ColorEqual(const D2D1_COLOR_F, const D2D1_COLOR_F);
	D2D1_COLOR_F GetRandColor();
}