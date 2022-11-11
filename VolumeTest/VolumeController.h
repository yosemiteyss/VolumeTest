#pragma once

#include <endpointvolume.h>
#include "CVolumeNotification.h"

class VolumeController {
public:
    static VolumeController *GetInstance();

    bool RegisterVolumeController();

    bool RegisterEndpointNotification();

    void Dispose();

    void SetMaxVolume();

    void SetMinVolume();

    void SetVolumeUp(float step);

    void SetVolumeDown(float step);

    void SetMute(bool is_mute);

    float GetCurrentVolume();

    VolumeController(VolumeController const &) = delete;

    void operator=(VolumeController const &) = delete;

private:
    VolumeController();

    static VolumeController *instance;

    IAudioEndpointVolume *endpoint_volume;

    CVolumeNotification *volume_notification;
};