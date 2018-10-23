#include "helpers.h"


bool Helpers::ColorEqual(const D2D1_COLOR_F A, const D2D1_COLOR_F B)
{
	if (A.r != B.r) return 0;
	if (A.g != B.g) return 0;
	if (A.b != B.b) return 0;
	if (A.a != B.a) return 0;
	return 1;
}

D2D1_COLOR_F Helpers::GetRandColor()
{
	D2D1_COLOR_F color;
	int rndVal = std::rand() % 6 + 1;
	switch (rndVal)
	{
	case 1:
		color = D2D1::ColorF(1.0f, 0, 0);
		break;
	case 2:
		color = D2D1::ColorF(0, 1.0f, 0);
		break;
	case 3:
		color = D2D1::ColorF(0, 0, 1.0f);
		break;
	case 4:
		color = D2D1::ColorF(1.0f, 1.0, 0);
		break;
	case 5:
		color = D2D1::ColorF(0, 1.0f, 1.0f);
		break;
	case 6:
		color = D2D1::ColorF(1.0f, 0, 1.0f);
		break;
	default:
		color = D2D1::ColorF(0, 0, 0);
	}
	return color;
}
