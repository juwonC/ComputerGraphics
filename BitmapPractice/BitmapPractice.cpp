#include "BitmapPractice.h"

void BitmapPractice::Initialize(HINSTANCE hInstance, LPCWSTR title, UINT width, UINT height)
{
	D2DFramework::Initialize(hInstance, title, width, height);

	mspBakcBuffer = std::make_unique<UINT8[]>(
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

	ClearBuffer(D2D1::ColorF(D2D1::ColorF::LightCoral));

	//DrawPixelToBuffer(10, 10, D2D1::ColorF::White);
	FillRectToBuffer(0, 0, 100, 100, D2D1::ColorF::Green);
	FillRectToBuffer(50, 50, 100, 100, D2D1::ColorF(1, 0, 0, 0.5f));

	PresentBuffer();

	mspRenderTarget->DrawBitmap(mspFrameBitmap.Get());

	mspRenderTarget->EndDraw();
}

void BitmapPractice::PresentBuffer()
{
	mspFrameBitmap->CopyFromMemory(
		nullptr,
		&mspBakcBuffer[0],
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

	mspBakcBuffer[index]		= static_cast<UINT8>(mspBakcBuffer[index] * inverse + r * color.a);
	mspBakcBuffer[index + 1]	= static_cast<UINT8>(mspBakcBuffer[index + 1] * inverse + g * color.a);
	mspBakcBuffer[index + 2]	= static_cast<UINT8>(mspBakcBuffer[index + 2] * inverse + b * color.a);
	mspBakcBuffer[index + 3]	= 255;
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

	UINT8* pCurrent = &mspBakcBuffer[0];
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
