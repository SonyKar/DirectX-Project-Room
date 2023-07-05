#pragma once

#include <dshow.h>

#define WM_GRAPHNOTIFY  WM_APP + 1
#define ONE_SECOND 10000000

class AudioManager
{
	IGraphBuilder* graphBuilder = NULL;
	IMediaControl* mediaControl = NULL;
	IMediaEventEx* mediaEvent = NULL;
	IMediaSeeking* mediaSeeking = NULL;
	HDC hdc;

	REFERENCE_TIME rtNow = 0 * ONE_SECOND, rtStop;
public:
	~AudioManager();
	HRESULT InitDirectShow(HWND hWnd);
	void HandleGraphEvent();
	VOID GetEventHandle(HWND hWnd);
	VOID PlaySound();
	VOID rewind();
};

