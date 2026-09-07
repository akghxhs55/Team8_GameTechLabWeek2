#pragma once

#include "Runtime/Core/IntTypes.h"
#include "Runtime/Math/FVector2.h"

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
	[[nodiscard]] bool IsKeyJustPressed(uint32 Key) const;
	[[nodiscard]] bool IsKeyJustReleased(uint32 Key) const;

	// TODO: 없애야 함
	void AddMouseInput(const FVector2& Delta);
	[[nodiscard]] FVector2 GetMouseDelta() const;

	FInputManager(const FInputManager&) = delete;
	FInputManager& operator=(const FInputManager&) = delete;

	FInputManager(FInputManager&&) = delete;
	FInputManager& operator=(FInputManager&&) = delete;

private:
	FInputManager() = default;
	~FInputManager() = default;

	[[nodiscard]] bool IsPrevKeyDown(uint32 Key) const;

	bool CurrentKeyStates[MAX_KEYS] = {};
	bool PreviousKeyStates[MAX_KEYS] = {};
	FVector2 MouseDelta{};
};
