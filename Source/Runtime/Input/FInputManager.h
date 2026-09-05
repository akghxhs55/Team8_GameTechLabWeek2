#pragma once

#include "Runtime/Core/IntTypes.h"
#include "Runtime/Math/FVector2.h"

// TODO: 키 꾹 누르는 입력이 안됨
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
	// TODO: 없애야 함
	[[nodiscard]] bool IsKeyDown(uint32 Key) const;
	[[nodiscard]] bool IsPrevKeyDown(uint32 Key) const;
	[[nodiscard]] bool IsKeyPressed(uint32 Key) const;
	[[nodiscard]] bool IsKeyReleased(uint32 Key) const;

	void AddMouseInput(const FVector2& Delta);
	[[nodiscard]] FVector2 GetMouseDelta() const;

	FInputManager(const FInputManager&) = delete;
	FInputManager& operator=(const FInputManager&) = delete;

	FInputManager(FInputManager&&) = delete;
	FInputManager& operator=(FInputManager&&) = delete;

private:
	FInputManager() = default;
	~FInputManager() = default;

	bool CurrentKeyStates[MAX_KEYS] = {};
	bool PreviousKeyStates[MAX_KEYS] = {};
	FVector2 MouseDelta{};
};
