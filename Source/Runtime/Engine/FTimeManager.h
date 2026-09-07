#pragma once

class FTimeManager final
{
public:

	static FTimeManager& Get()
	{
		static FTimeManager Instance;
		return Instance;
	}

	void Resume() { bisRunning = true; }
	void Pause() { bisRunning = false; }
	[[nodiscard]] float GetDeltaTime() const { return deltaTime; }
	void Update();
	void SetTargetFPS(float _targetFPS) { targetFPS = _targetFPS; }

	FTimeManager(const FTimeManager&) = delete;
	FTimeManager& operator=(const FTimeManager&) = delete;

	FTimeManager(FTimeManager&&) = delete;
	FTimeManager&& operator=(FTimeManager&&) = delete;

private:
	FTimeManager();
	~FTimeManager() = default;

	LARGE_INTEGER prevTime;
	LARGE_INTEGER  frequency;
	
	float targetFPS = 60.0f;
	float targetFrameTime;
	float deltaTime = 0.0f;

	bool bisRunning = false;
};
