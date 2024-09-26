#include <chrono>
#include <iostream>
#include <ratio>
#include <thread>
#include <type_traits>
#include "cipa.h"
#include <Windows.h>
#include "windef.h"
#include <random>

CTimer::CTimer(std::chrono::milliseconds everyXms)
{
    setEvery(everyXms);
}

bool CTimer::isElapsed()
{
    std::chrono::steady_clock::time_point timerNow = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(timerNow - timerBegin) >= targetMs)
    {
        timerBegin = timerNow;
        return true;
    }
    return false;
}

void CTimer::setEvery(std::chrono::milliseconds everyXms)
{
    timerBegin = std::chrono::steady_clock::now();
    targetMs = everyXms;
}

void CTimer::reset()
{
    timerBegin = std::chrono::steady_clock::now();
}