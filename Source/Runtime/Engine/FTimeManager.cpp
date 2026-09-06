#include <windows.h>
#include "FTimeManager.h"

FTimeManager::FTimeManager()
{
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&prevTime);
    targetFrameTime = 1.0 / targetFPS;
}

void FTimeManager::Update()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    double actualDeltaTime = static_cast<double>(currentTime.QuadPart - prevTime.QuadPart) / static_cast<double>(frequency.QuadPart);
    

    if(actualDeltaTime < targetFrameTime)
    {
        double remainingTime = targetFrameTime - actualDeltaTime;

        Sleep(static_cast<DWORD>(remainingTime * 1000));

        QueryPerformanceCounter(&currentTime);
        deltaTime = static_cast<double>(currentTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;
    }
    else
    {
        deltaTime = actualDeltaTime;

    }


    deltaTime = bisRunning ? deltaTime : 0.0;
    prevTime = currentTime;
}



