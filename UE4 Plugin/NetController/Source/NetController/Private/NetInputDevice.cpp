#include "NetControllerPrivatePCH.h"
#include "NetInputDevice.h"
#include "SocketSubsystem.h"

#define LOCTEXT_NAMESPACE "FNetInputDevice"

const FGamepadKeyNames::Type ButtonNames[] = {
	FGamepadKeyNames::FaceButtonBottom,
	FGamepadKeyNames::FaceButtonRight,
	FGamepadKeyNames::FaceButtonLeft,
	FGamepadKeyNames::FaceButtonTop,
	FGamepadKeyNames::SpecialLeft,
	FGamepadKeyNames::Invalid,
	FGamepadKeyNames::SpecialRight,
	FGamepadKeyNames::LeftThumb,
	FGamepadKeyNames::RightThumb,
	FGamepadKeyNames::LeftShoulder,
	FGamepadKeyNames::RightShoulder,
	FGamepadKeyNames::DPadUp,
	FGamepadKeyNames::DPadDown,
	FGamepadKeyNames::DPadLeft,
	FGamepadKeyNames::DPadRight,
};

const FGamepadKeyNames::Type AxesNames[] = {
	FGamepadKeyNames::LeftAnalogX,
	FGamepadKeyNames::LeftAnalogY,
	FGamepadKeyNames::RightAnalogX,
	FGamepadKeyNames::RightAnalogY,
	FGamepadKeyNames::LeftTriggerAnalog,
	FGamepadKeyNames::RightTriggerAnalog,
};

const FGamepadKeyNames::Type AxesActionNames[] = {
	FGamepadKeyNames::LeftStickLeft, FGamepadKeyNames::LeftStickRight,
	FGamepadKeyNames::LeftStickUp, FGamepadKeyNames::LeftStickDown,
	FGamepadKeyNames::RightStickLeft, FGamepadKeyNames::RightStickRight,
	FGamepadKeyNames::RightStickUp, FGamepadKeyNames::RightStickDown,
	FGamepadKeyNames::LeftTriggerThreshold, FGamepadKeyNames::Invalid,
	FGamepadKeyNames::RightTriggerThreshold, FGamepadKeyNames::Invalid,
};

#define SCALEINT16(X) (2.0 * ((X) + 32768.0) / 65535.0 - 1.0)
#define SCALEHINT16(X) ((X) / 32767.0)

float ScaleAxis(int16 value, uint8 index) {
	switch (index) {
		case 0:
		case 2:
			return SCALEINT16(value);
		case 1:
		case 3:
			return -SCALEINT16(value);
		case 4:
		case 5:
			return SCALEHINT16(value);
		default:
			return 0.0;
	}
}

FNetInputDevice::FNetInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) :
MessageHandler(InMessageHandler),
Socket(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_DGram, TEXT("NetControllerSocket"), true))
{
	Socket->Bind(*ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr(0, 1238));
	Socket->SetNonBlocking(true);
	FMemory::Memzero(ControllerData, 40);
}

FNetInputDevice::~FNetInputDevice()
{
	ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
}

void FNetInputDevice::Tick(float DeltaTime)
{
	// Nothing necessary to do (boilerplate code to complete the interface)
}

void FNetInputDevice::SendControllerEvents()
{
	uint8 *ControllerButtons = (uint8 *) &ControllerData[1];
	int16 *ControllerAxes = (int16 *) &ControllerData[16];
	uint8 *ControllerAxesActions = (uint8 *) &ControllerData[28];
	
	uint8 RecvData[40];
	uint8 *RecvButtons = (uint8 *) &RecvData[1];
	int16 *RecvAxes = (int16 *) &RecvData[16];
	uint8 *RecvAxesActions = (uint8 *) &RecvData[28];
	int32 readCount;
	do {
		Socket->Recv(RecvData, 28, readCount, ESocketReceiveFlags::None);
		
		if (readCount == 28) {
			FMemory::Memset(RecvData + 28, 0xFF, 12);
			for (uint8 i=0; i < 15; ++i) {
				if (RecvButtons[i] != ControllerButtons[i]) {
					if (ButtonNames[i] == FGamepadKeyNames::Invalid) return;
					if (RecvButtons[i]) {
						MessageHandler->OnControllerButtonPressed(ButtonNames[i], 0, false);
					} else {
						MessageHandler->OnControllerButtonReleased(ButtonNames[i], 0, false);
					}
					ControllerButtons[i] = RecvButtons[i];
				}
			}
			for (uint8 i=0; i < 6; ++i) {
				if (RecvAxes[i] != ControllerAxes[i]) {
					float v = ScaleAxis(RecvAxes[i], i);
					MessageHandler->OnControllerAnalog(AxesNames[i], 0, v);
					float dz = i < 4 ? 0.25 : 0.0;
					for (uint8 j=0; j < 2; ++j)
						RecvAxesActions[2 * i + j] = v * (j ? -1.0 : 1.0) > dz;
					ControllerAxes[i] = RecvAxes[i];
				}
			}
			for (uint8 i=0; i < 12; ++i) {
				if (RecvAxesActions[i] != 0xFF && RecvAxesActions[i] != ControllerAxesActions[i]) {
					if (AxesActionNames[i] == FGamepadKeyNames::Invalid) return;
					if (RecvAxesActions[i]) {
						MessageHandler->OnControllerButtonPressed(AxesActionNames[i], 0, false);
					} else {
						MessageHandler->OnControllerButtonReleased(AxesActionNames[i], 0, false);
					}
					ControllerAxesActions[i] = RecvAxesActions[i];
				}
			}
		}
	} while (readCount > 0);
}
 
void FNetInputDevice::SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

bool FNetInputDevice::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	// Nothing necessary to do (boilerplate code to complete the interface)
	return false;
}

void FNetInputDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
	// Nothing necessary to do (boilerplate code to complete the interface)
}

void FNetInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values)
{
	// Nothing necessary to do (boilerplate code to complete the interface)
}

#undef LOCTEXT_NAMESPACE
