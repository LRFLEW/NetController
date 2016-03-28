#pragma once

#include "InputDevice.h"

class FNetControllerModule : public IInputDeviceModule
{
public:
    virtual TSharedPtr<class IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
};
