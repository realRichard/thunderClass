#pragma once


#pragma comment(lib, "Winmm")


// audio
static char* audioData = NULL;
static int audioSize = 20000 + 4 + 1 + 4 + 1;
static int audioBufferSize = 10000 + 4 + 1;
static int audioOffset = 0;


void CALLBACK waveInProc(
    HWAVEIN   hwi,
    UINT      uMsg,
    DWORD_PTR dwInstance,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2
) {
    //#define MM_WIM_OPEN         0x3BE           /* waveform input */
    //#define MM_WIM_CLOSE        0x3BF
    //#define MM_WIM_DATA         0x3C0
    if (uMsg == WIM_CLOSE) {
        printf("callback close %d: (%d)\n", uMsg, WIM_CLOSE);
    }
    else if (uMsg == WIM_OPEN) {
        printf("callback open %d: (%d)\n", uMsg, WIM_OPEN);
    }
    else if (uMsg == WIM_DATA) {
        printf("callback data %d: (%d)\n", uMsg, WIM_DATA);

        LPWAVEHDR header = (LPWAVEHDR)dwParam1;
        // the first five bytes is used to data header
        header->lpData = audioData + audioOffset + 5;

        //header->dwBufferLength = audioBufferSize - 5;
        header->dwBufferLength = 10000;
        //header.dwBytesRecorded = size - audioOffset;
        header->dwBytesRecorded = header->dwBufferLength;
        header->dwUser = NULL;
        header->dwFlags = 0;
        //header->dwFlags = WHDR_BEGINLOOP;

        int waveInPrepare = waveInPrepareHeader(hwi, header, sizeof(WAVEHDR));
        MMSYSERR_INVALHANDLE;
        printf("waveInPrepare: (%d)\n", waveInPrepare);
        int waveInAddBuff = waveInAddBuffer(
            hwi,
            header,
            sizeof(WAVEHDR)
        );
        audioOffset += audioBufferSize;
        if (audioOffset >= audioSize) {
            audioOffset = 0;
        }
        MMSYSERR_NOERROR;
        MMSYSERR_INVALHANDLE;
        printf("waveInAddBuff: (%d)\n", waveInAddBuff);
    }
}
