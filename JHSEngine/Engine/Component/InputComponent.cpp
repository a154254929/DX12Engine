#include "InputComponent.h"

void CInputComponent::BeginInit()
{

}

void CInputComponent::Tick(float deltaTime)
{
	if (captureKeyboardInforDelegate.IsBound())
	{
		FInputKey inputKey;
		captureKeyboardInforDelegate.Execute(inputKey);
	}
}

