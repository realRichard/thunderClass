#pragma once


#include "zlib.h"
#pragma comment(lib, "zlibwapi.lib")


class Screen {
public:
    int width;
    int height;

    Screen(int width, int height) {
        this->width = width;
        this->height = height;
    }

    char* capture()
    {
        //int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        //int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
        int width = this->width;
        int height = this->height;
        int nScreenWidth = width;
        int nScreenHeight = height;
        HWND hDesktopWnd = GetDesktopWindow();
        HDC hDesktopDC = GetDC(hDesktopWnd);
        HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
        HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, nScreenWidth, nScreenHeight);
        SelectObject(hCaptureDC, hCaptureBitmap);

        BitBlt(hCaptureDC, 0, 0, nScreenWidth, nScreenHeight, hDesktopDC, 0, 0, SRCCOPY | CAPTUREBLT);

        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
        bmi.bmiHeader.biWidth = nScreenWidth;
        bmi.bmiHeader.biHeight = -nScreenHeight;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* pixels = new RGBQUAD[(long int)nScreenWidth * (long int)nScreenHeight];

        GetDIBits(
            hCaptureDC,
            hCaptureBitmap,
            0,
            nScreenHeight,
            pixels,
            &bmi,
            DIB_RGB_COLORS
        );

        const int size = nScreenWidth * nScreenWidth * 3;
        char* bytes = new char[size];
        int index = 0;
        for (int x = 0; x < nScreenWidth; x++) {
            for (int y = 0; y < nScreenHeight; y++) {
                int p;
                p = (x * nScreenHeight) + y;
                bytes[index] = pixels[p].rgbRed;
                index++;
                bytes[index] = pixels[p].rgbGreen;
                index++;
                bytes[index] = pixels[p].rgbBlue;
                index++;
            }
        }

        delete[] pixels;
        return bytes;
    }

    int size() {
        return this->width * this->height * 3;
    }

    int pixelsCompress(const unsigned char* inputData, size_t inputDataSize, char** outputData, size_t* outputDataSize) {
        // ����ѹ���������Ҫ�����ռ�
        int outSize = compressBound(inputDataSize);
        // prepend header, 5 bytes, the first four bytes to record the size of compressed data, the five byte is the type of data
        size_t totalSize = outSize + 4 + 1;

        *outputData = (char*)malloc(totalSize);
        int err = compress((Bytef*)(*(outputData)+4 + 1), (uLongf*)&outSize, inputData, (uLong)inputDataSize);
        if (err != Z_OK) {
            printf("compress failed\n");
        }
        *outputDataSize = outSize + 4 + 1;
        // write size, recv function will know receive how much data
        *(*(outputData)+0) = outSize >> 0;
        *(*(outputData)+1) = outSize >> 8;
        *(*(outputData)+2) = outSize >> 16;
        *(*(outputData)+3) = outSize >> 24;
        // the five byte, 3 represent pixels data
        *(*(outputData)+4) = PIXELTYPE;
        return 0;
    }

private:

};

