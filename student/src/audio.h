#pragma once


#pragma comment(lib, "Winmm")


static bool hasAudioData = false;
static int audioSize = 10000;
static char* audioData = NULL;
static int audioOffset = 0;
static int audioBufferSize = 5000;


void CALLBACK waveOutProc(
	HWAVEOUT hwo,
	UINT uMsg,
	DWORD_PTR dwInstance,
	DWORD_PTR dwParam1,
	DWORD_PTR dwParam2
) {
	// #define MM_WOM_OPEN 0x3BB
	// #define MM_WOM_CLOSE 0x3BC
	// #define MM_WOM_DONE 0x3BD
	if (uMsg == WOM_OPEN) {
		printf("callback open: %d (%d)\n", uMsg, WOM_OPEN);
	}
	else if (uMsg == WOM_CLOSE) {
		printf("callback close: %d (%d)\n", uMsg, WOM_CLOSE);
	}
	else {
		printf("callback done: %d (%d)\n", uMsg, WOM_DONE);
		LPWAVEHDR header = (LPWAVEHDR)dwParam1;
		if (audioOffset >= audioSize) {
			audioOffset = 0;
		}
		header->lpData = audioData + audioOffset;
		audioOffset += audioBufferSize;
		header->dwBufferLength = audioBufferSize;
		waveOutPrepareHeader(hwo, header, sizeof(WAVEHDR));
		waveOutWrite(
			hwo,
			header,
			sizeof(WAVEHDR));
	}
}
