#include <chrono>
#include <type_traits>
#include <Windows.h>
#include "windef.h"
#include <random>

inline static std::random_device rd{};
inline static std::mt19937 gen{ rd() };

void clicker_left();
void clicker_right();
void blockhit();
void binds();
void badauto();
void kopacniemaprzebacz();
void pinspoofer();
void init();
class CTimer
{
public:
	CTimer(std::chrono::milliseconds everyXms);
	bool isElapsed();
	void setEvery(std::chrono::milliseconds everyXms);
	void reset();
private:
	std::chrono::steady_clock::time_point timerBegin;
	std::chrono::milliseconds targetMs;
};