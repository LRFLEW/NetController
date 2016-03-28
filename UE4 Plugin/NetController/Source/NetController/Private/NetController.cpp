#include "NetControllerPrivatePCH.h"
#include "NetController.h"
#include "NetInputDevice.h"

#define LOCTEXT_NAMESPACE "FNetControllerModule"

TSharedPtr<class IInputDevice> FNetControllerModule::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
    return MakeShareable(new FNetInputDevice(InMessageHandler));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNetControllerModule, NetController)