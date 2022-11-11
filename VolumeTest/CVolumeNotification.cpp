#include <stdio.h>
#include "CVolumeNotification.h"

CVolumeNotification::CVolumeNotification() : _cRef(1) {}

STDMETHODIMP_(ULONG)

CVolumeNotification::AddRef() {
    return InterlockedIncrement(&_cRef);
}

STDMETHODIMP_(ULONG)

CVolumeNotification::Release() {
    LONG ref = InterlockedDecrement(&_cRef);
    if (ref == 0) {
        delete this;
    }
    return ref;
}

STDMETHODIMP CVolumeNotification::QueryInterface(REFIID riid, void **ppvInterface) {
    if (riid == IID_IUnknown || riid == __uuidof(IAudioEndpointVolumeCallback)) {
        AddRef();
        *ppvInterface = static_cast<IUnknown *>(this);
        return S_OK;
    }

    *ppvInterface = NULL;
    return E_NOINTERFACE;
}

STDMETHODIMP CVolumeNotification::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify) {
    if (pNotify == NULL) {
        return E_INVALIDARG;
    }

    if (pNotify->guidEventContext != g_guidMyContext) {
        float volume = pNotify->fMasterVolume;
        printf_s("Current Volume: %f\n", volume);
    }

    return S_OK;
}
