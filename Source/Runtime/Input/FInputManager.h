#pragma once

#include "Runtime/Core/IntTypes.h"

constexpr uint32 MAX_KEYS = 256;

class FInputManager final
{
public:
	static FInputManager& Get()
	{
		static FInputManager Instance;
		return Instance;
	}
	
	void Update();

	[[nodiscard]] bool IsKeyDown(uint8 Key) const;
	[[nodiscard]] bool IsPrevKeyDown(uint8 Key) const;
	[[nodiscard]] bool IsKeyPressed(uint8 Key) const;
	[[nodiscard]] bool IsKeyReleased(uint8 Key) const;

	FInputManager(const FInputManager&) = delete;
	FInputManager& operator=(const FInputManager&) = delete;

	FInputManager(FInputManager&&) = delete;
	FInputManager& operator=(FInputManager&&) = delete;

private:
	FInputManager() = default;
	~FInputManager() = default;

	bool CurrentKeyStates[MAX_KEYS] = {};
	bool PreviousKeyStates[MAX_KEYS] = {};
};
