#include <stdio.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <math.h>
#include "VolumeController.h"

VolumeController *VolumeController::instance = NULL;

VolumeController::VolumeController() : endpoint_volume(NULL), volume_notification(NULL) {}

VolumeController *VolumeController::GetInstance() {
    if (instance == NULL) {
        instance = new VolumeController();
    }

    return instance;
}

bool VolumeController::RegisterVolumeController() {
    HRESULT hr;
    IMMDevice *default_device = NULL;
    IMMDeviceEnumerator *device_enumator = NULL;

    CoInitialize(NULL);

    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator),
                          (LPVOID * ) & device_enumator);
    if (hr != S_OK) {
        return false;
    }

    hr = device_enumator->GetDefaultAudioEndpoint(eRender, eConsole, &default_device);
    if (hr != S_OK) {
        return false;
    }

    hr = default_device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL,
                                  (LPVOID * ) & endpoint_volume);
    if (hr != S_OK) {
        return false;
    }

    return true;
}

bool VolumeController::RegisterEndpointNotification() {
    HRESULT hr;

    if (endpoint_volume) {
        volume_notification = new CVolumeNotification();
        hr = endpoint_volume->RegisterControlChangeNotify(volume_notification);
        if (hr != S_OK) {
            return false;
        }
    }

    return true;
}

void VolumeController::Dispose() {
    if (endpoint_volume) {
        if (volume_notification) {
            endpoint_volume->UnregisterControlChangeNotify(volume_notification);
            volume_notification->Release();
        }
        endpoint_volume->Release();
    }

    CoUninitialize();

    if (instance) {
        delete instance;
        instance = NULL;
    }
}

void VolumeController::SetMaxVolume() {
    UINT current_step, step_count;

    if (endpoint_volume) {
        endpoint_volume->GetVolumeStepInfo(&current_step, &step_count);
        this->SetMute(false);
        for (UINT index = current_step; index < step_count; index++) {
            SetVolumeUp(-1);
        }
    }
}

void VolumeController::SetMinVolume() {
    UINT current_step, step_count;

    if (endpoint_volume) {
        endpoint_volume->GetVolumeStepInfo(&current_step, &step_count);
        this->SetMute(false);
        for (UINT index = current_step; index > 0; index--) {
            SetVolumeDown(-1);
        }
    }
}

void VolumeController::SetVolumeUp(float step) {
    if (endpoint_volume == NULL) {
        return;
    }

    if (step < 0) {
        endpoint_volume->VolumeStepUp(NULL);
    } else {
        float current_volume = GetCurrentVolume();
        endpoint_volume->SetMasterVolumeLevelScalar(current_volume + step, NULL);
    }
}

void VolumeController::SetVolumeDown(float step) {
    if (endpoint_volume == NULL) {
        return;
    }

    if (step < 0) {
        endpoint_volume->VolumeStepDown(NULL);
    } else {
        float current_volume = GetCurrentVolume();
        endpoint_volume->SetMasterVolumeLevelScalar(current_volume - step, NULL);
    }
}

void VolumeController::SetMute(bool is_mute) {
    if (is_mute) {
        endpoint_volume->SetMute(TRUE, NULL);
    } else {
        endpoint_volume->SetMute(FALSE, NULL);
    }
}

float VolumeController::GetCurrentVolume() {
    UINT current_step, step_count;
    float current_volume = 0;

    if (endpoint_volume) {
        endpoint_volume->GetVolumeStepInfo(&current_step, &step_count);
        printf_s("current step: %d, step count: %d\n", current_step, step_count);
        current_volume = current_step * 1.0f / step_count;
    }

    return current_volume;
}
