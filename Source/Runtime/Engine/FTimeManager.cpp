#include <windows.h>
#include "FTimeManager.h"

FTimeManager::FTimeManager()
{
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&PrevTime);
    TargetFrameTime = 1.0f / TargetFPS;
}

void FTimeManager::Update()
{
    LARGE_INTEGER CurrentTime;
    QueryPerformanceCounter(&CurrentTime);

    float ActualDeltaTime = static_cast<float>(CurrentTime.QuadPart - PrevTime.QuadPart) / static_cast<float>(Frequency.QuadPart);
    if(ActualDeltaTime < TargetFrameTime)
    {
        float RemainingTime = TargetFrameTime - ActualDeltaTime;

        Sleep(static_cast<DWORD>(RemainingTime * 1000));

        QueryPerformanceCounter(&CurrentTime);
        DeltaTime = static_cast<float>(CurrentTime.QuadPart - PrevTime.QuadPart) / static_cast<float>(Frequency.QuadPart);
    }
    else
    {
        DeltaTime = ActualDeltaTime;
    }

    DeltaTime = bIsRunning ? DeltaTime : 0.0f;
    PrevTime = CurrentTime;
}
