#include "FInputManager.h"

#include <cstring>

void FInputManager::Update()
{
	memcpy(PreviousKeyStates, CurrentKeyStates, sizeof(bool) * MAX_KEYS);
}

bool FInputManager::IsKeyDown(uint8 Key) const
{
	return CurrentKeyStates[Key];
}

bool FInputManager::IsPrevKeyDown(uint8 Key) const
{
	return PreviousKeyStates[Key];
}

bool FInputManager::IsKeyPressed(uint8 Key) const
{
	return IsKeyDown(Key) && !IsPrevKeyDown(Key);
}

bool FInputManager::IsKeyReleased(uint8 Key) const
{
	return !IsKeyDown(Key) && IsPrevKeyDown(Key);
}
