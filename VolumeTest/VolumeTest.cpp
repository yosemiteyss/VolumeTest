#include <iostream>
#include <Windows.h>
#include <memory>
#include "VolumeController.h"

GUID g_guidMyContext = GUID_NULL;

int main() {
    CoCreateGuid(&g_guidMyContext);

    auto volume_controller = std::unique_ptr<VolumeController>(VolumeController::GetInstance());
    volume_controller->RegisterVolumeController();
    volume_controller->RegisterEndpointNotification();

    float currentVolume = volume_controller->GetCurrentVolume();
    std::cout << "Current volume: " << currentVolume << std::endl;

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    volume_controller->Dispose();
}



