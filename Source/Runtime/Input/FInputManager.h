#pragma once

#include "Runtime/Core/IntTypes.h"

class FInputManager final
{
public:
	static constexpr int32 MAX_KEYS = 256;

	static FInputManager& Get()
	{
		static FInputManager Instance;
		return Instance;
	}
	
	void Update();

	[[nodiscard]] bool IsKeyDown(uint32 Key) const;
	[[nodiscard]] bool IsPrevKeyDown(uint32 Key) const;
	[[nodiscard]] bool IsKeyPressed(uint32 Key) const;
	[[nodiscard]] bool IsKeyReleased(uint32 Key) const;

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
