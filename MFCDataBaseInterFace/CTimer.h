#pragma once
#include <chrono>

class CTimer 
{
private:
	std::chrono::high_resolution_clock::time_point start;

public:

	CTimer();
	~CTimer();

};

