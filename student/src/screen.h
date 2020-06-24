#pragma once


#include "zlib.h"
#pragma comment(lib, "zlibwapi.lib")


class Screen {
public:
	// hard code
	const static int width = 300;
	const static int height = 300;

	static void draw(char* buff, HWND hWnd)
	{
		int nScreenWidth = Screen::width;
		int nScreenHeight = Screen::height;
		BITMAPINFOHEADER header;
		header.biSize = sizeof(BITMAPINFOHEADER);
		header.biWidth = nScreenWidth;
		header.biHeight = nScreenHeight;
		header.biPlanes = 1;
		header.biBitCount = 24;
		header.biCompression = BI_RGB;
		header.biSizeImage = 0;
		header.biXPelsPerMeter = nScreenWidth;
		header.biYPelsPerMeter = nScreenHeight;
		header.biClrUsed = 0;
		header.biClrImportant = 0;
		auto hdc = GetDC(hWnd);

		StretchDIBits(hdc,
			0, 0,
			header.biWidth,
			header.biHeight,
			0, nScreenHeight,
			header.biWidth,
			-header.biHeight,
			buff,
			(BITMAPINFO*)&header,
			DIB_RGB_COLORS, SRCCOPY
		);
	}

	static int size() {
		return Screen::height * Screen::width * 3;
	}

	//void pixelsUncompress(Bytef* dest, uLongf* destLen, const Bytef* source, uLong sourceLen) {
	//	uncompress(dest, destLen, source, sourceLen);
	//}

	static char* pixelsUncompress(const Bytef* source, uLong sourceLen) {
		uLongf outSize = Screen::size();
		char* data = (char*)malloc(outSize);
		uncompress((Bytef*)data, &outSize, source, sourceLen);
		return data;
	}



private:

};
