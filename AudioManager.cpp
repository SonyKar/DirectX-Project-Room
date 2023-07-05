#include "AudioManager.h"

AudioManager::~AudioManager()
{
    mediaSeeking->Release();
    mediaEvent->Release();
    mediaControl->Release();
    graphBuilder->Release();
}

HRESULT AudioManager::InitDirectShow(HWND hWnd)
{
    HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL,
        CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&graphBuilder);

    hr = graphBuilder->QueryInterface(IID_IMediaControl, (void**)&mediaControl);
    hr = graphBuilder->QueryInterface(IID_IMediaEventEx, (void**)&mediaEvent);
    hr = graphBuilder->QueryInterface(IID_IMediaSeeking, (void**)&mediaSeeking);

    hr = graphBuilder->RenderFile(L"chair.wav", NULL);

    mediaEvent->SetNotifyWindow((OAHWND)hWnd, WM_GRAPHNOTIFY, 0);

    return S_OK;
}

void AudioManager::HandleGraphEvent()
{
    long evCode;
    LONG_PTR param1, param2;

    while (SUCCEEDED(mediaEvent->GetEvent(&evCode, &param1, &param2, 0)))
    {
        mediaEvent->FreeEventParams(evCode, param1, param2);
        switch (evCode)
        {
        case EC_COMPLETE:
            rewind();
            mediaControl->Stop();
            return;
        case EC_USERABORT:  
        case EC_ERRORABORT:
            PostQuitMessage(0);
            return;
        }
    }
}

void AudioManager::GetEventHandle(HWND hWnd)
{
    hdc = GetDC(hWnd);
}

void AudioManager::PlaySound()
{
    mediaControl->Run();
}

void AudioManager::rewind()
{
    mediaSeeking->GetDuration(&rtStop);
    mediaSeeking->SetPositions(
        &rtNow, AM_SEEKING_AbsolutePositioning,
        &rtStop, AM_SEEKING_AbsolutePositioning
    );
}
