#pragma once
#include "D2DFramework.h"

const int BITMAP_WIDTH{ 1024 };
const int BITMAP_HEIGHT{ 768 };
const int BITMAP_BYTECOUNT{ 4 };

class BitmapPractice : public D2DFramework
{
private:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> mspFrameBitmap{};
	std::unique_ptr<UINT8[]> mspBakcBuffer;

public:
	virtual void Initialize(HINSTANCE hInstance,
		LPCWSTR title = L"Direct2D Example",
		UINT width = 1024,
		UINT height = 768) override;
	void Render() override;
};

