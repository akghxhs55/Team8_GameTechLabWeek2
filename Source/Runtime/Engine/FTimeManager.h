#pragma once

class FTimeManager final
{
public:

	static FTimeManager& Get()
	{
		static FTimeManager Instance;
		return Instance;
	}

	void Resume() { isRunning = true; }
	void Pause() { isRunning = false; };
	float GetDeltaTime() const { return deltaTime; };
	void Update();
	void SetTargetFPS(float _targetFPS) { targetFPS = _targetFPS; };

private:
	FTimeManager();
	~FTimeManager() = default;

	LARGE_INTEGER prevTime;
	LARGE_INTEGER  frequency;
	
	float targetFPS = 60.0f;
	float targetFrameTime;
	float deltaTime = 0.0f;

	bool isRunning = false;


};
