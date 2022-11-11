#pragma once

#include <endpointvolume.h>

// https://learn.microsoft.com/en-us/windows/win32/coreaudio/endpoint-volume-controls

// Client's proprietary event-context GUID.
extern GUID g_guidMyContext;

class CVolumeNotification : public IAudioEndpointVolumeCallback {
public:
    CVolumeNotification();

    STDMETHODIMP_(ULONG)

    AddRef();

    STDMETHODIMP_(ULONG)

    Release();

    STDMETHODIMP QueryInterface(REFIID riid, void **ppvInterface);

    STDMETHODIMP OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);

private:
    LONG _cRef;
};