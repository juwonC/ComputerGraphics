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

	mspRenderTarget->DrawBitmap(mspFrameBitmap.Get());

	mspRenderTarget->EndDraw();
}
