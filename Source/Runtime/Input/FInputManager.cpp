#include "FInputManager.h"

#include "Runtime/Core/IntTypes.h"
#include <Windows.h>
#include <cstring>

void FInputManager::Update()
{
	memcpy(PreviousKeyStates, CurrentKeyStates, sizeof(bool) * MAX_KEYS);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		CurrentKeyStates[i] = GetAsyncKeyState(i) & 0x8000 ? true : false;
	}
}

bool FInputManager::IsKeyDown(uint32 Key) const
{
	if (Key >= MAX_KEYS)
	{
		return false;
	}
	return CurrentKeyStates[Key];
}

bool FInputManager::IsPrevKeyDown(uint32 Key) const
{
	if (Key >= MAX_KEYS)
	{
		return false;
	}
	return PreviousKeyStates[Key];
}

bool FInputManager::IsKeyPressed(uint32 Key) const
{
	if (Key >= MAX_KEYS)
	{
		return false;
	}
	return IsKeyDown(Key) && !IsPrevKeyDown(Key);
}

bool FInputManager::IsKeyReleased(uint32 Key) const
{
	if (Key >= MAX_KEYS)
	{
		return false;
	}
	return !IsKeyDown(Key) && IsPrevKeyDown(Key);
}
