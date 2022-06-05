#include "BitmapPractice.h"

void BitmapPractice::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	mspBackBuffer = std::make_unique<UINT8[]>(
			BITMAP_WIDTH * BITMAP_HEIGHT * BITMAP_BYTECOUNT
		);

	HRESULT hr = mspRenderTarget->CreateBitmap(
		D2D1::SizeU(BITMAP_WIDTH, BITMAP_HEIGHT),
		D2D1::BitmapProperties( D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE) ),
		mspFrameBitmap.GetAddressOf()
	);

	ThrowIfFailed(hr);
}

void BitmapPractice::Render()
{
	mspRenderTarget->BeginDraw();
	mspRenderTarget->Clear(D2D1::ColorF(0.0f, 0.2f, 0.4f, 1.0f));

	ClearBuffer(D2D1::ColorF(D2D1::ColorF::Black));

	//DrawPixelToBuffer(10, 10, D2D1::ColorF::White);
	FillRectToBuffer(0, 0, 100, 100, D2D1::ColorF::Green);
	FillRectToBuffer(50, 50, 100, 100, D2D1::ColorF(1, 0, 0, 0.5f));
	DrawCircleToBuffer(50, 300, 50, D2D1::ColorF::Green);
	DrawLineToBuffer(300, 300, 0, 400, D2D1::ColorF::Green);

	PresentBuffer();

	mspRenderTarget->DrawBitmap(mspFrameBitmap.Get());

	mspRenderTarget->EndDraw();
}

void BitmapPractice::PresentBuffer()
{
	mspFrameBitmap->CopyFromMemory(
		nullptr,
		&mspBackBuffer[0],
		BITMAP_WIDTH * BITMAP_BYTECOUNT
	);
}

void BitmapPractice::DrawPixelToBuffer(int x, int y, D2D1::ColorF color)
{
	int pitch = BITMAP_WIDTH * BITMAP_BYTECOUNT;
	int index = x * BITMAP_BYTECOUNT + y * pitch;

	float inverse = 1.0f - color.a;

	UINT8 r = static_cast<UINT8>(color.r * 255);
	UINT8 g = static_cast<UINT8>(color.g * 255);
	UINT8 b = static_cast<UINT8>(color.b * 255);
	UINT8 a = static_cast<UINT8>(color.a * 255);

	mspBackBuffer[index]		= static_cast<UINT8>(mspBackBuffer[index] * inverse + r * color.a);
	mspBackBuffer[index + 1]	= static_cast<UINT8>(mspBackBuffer[index + 1] * inverse + g * color.a);
	mspBackBuffer[index + 2]	= static_cast<UINT8>(mspBackBuffer[index + 2] * inverse + b * color.a);
	mspBackBuffer[index + 3]	= 255;
}

void BitmapPractice::ClearBuffer(D2D1::ColorF color)
{
	//for (int x = 0; x < BITMAP_WIDTH; x++)
	//{
	//	for (int y = 0; y < BITMAP_HEIGHT; y++)
	//	{
	//		DrawPixelToBuffer(x, y, color);
	//	}
	//}

	UINT8* pCurrent = &mspBackBuffer[0];
	for (int i = 0; i < BITMAP_WIDTH * BITMAP_HEIGHT; ++i)
	{
		*pCurrent = static_cast<UINT8>(color.r * 255);
		*(pCurrent + 1)= static_cast<UINT8>(color.g * 255);
		*(pCurrent + 2) = static_cast<UINT8>(color.b * 255);
		*(pCurrent + 3) = static_cast<UINT8>(color.a * 255);

		pCurrent += BITMAP_BYTECOUNT;
	}
}

void BitmapPractice::FillRectToBuffer(int left, int top, int width, int height, D2D1::ColorF color)
{
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			DrawPixelToBuffer(x + left, y + top, color);
		}
	}
}

void BitmapPractice::DrawCircleToBuffer(int x, int y, int radius, D2D1::ColorF color)
{
	const double pi{ 3.141592 };

	for (int angle = 0; angle < 180; ++angle)
	{
		int x1 = cos(angle / static_cast<int>(pi)) * radius;
		int y1 = sin(angle / static_cast<int>(pi)) * radius;

		DrawPixelToBuffer(x + x1, y + y1, color);
	}
}

void BitmapPractice::DrawLineToBuffer(int x1, int y1, int x2, int y2, D2D1::ColorF color)
{
	int dx{ x2 - x1 };
	int dy{ y2 - y1 };

	if (dx == 0 && dy == 0)
	{
		return;
	}

	int increase{};

	if (abs(dx) > abs(dy))
	{
		increase = (dx < 0) ? -1 : 1;

		int y{};

		for (int x = x1; x != x2; x += increase)
		{
			int y = dy / dx * (x - x1) + y1;

			DrawPixelToBuffer(x, y, color);
		}
	}
	else
	{
		increase = (dy < 0) ? -1 : 1;
		
		int x{};

		for (int y = y1; y != y2; y += increase)
		{
			int x = dx / dy * (y - y1) + x1;

			DrawPixelToBuffer(x, y, color);
		}
	}
}
