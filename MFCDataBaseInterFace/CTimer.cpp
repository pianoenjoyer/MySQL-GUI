#include "pch.h"
#include "CTimer.h"

CTimer::CTimer()
	: start(std::chrono::high_resolution_clock::now())  // Initialize start with the current time
{

}

CTimer::~CTimer()
{
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
}