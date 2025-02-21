#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <array>
#include <omp.h>


//====================================== PAINT BALL ==================================================//
inline float ResultAx(const float(&arr)[12]);
inline float ResultBx(const float(&arr)[12]);
inline float ResultCx(const float(&arr)[12]);
inline float ResultDx(const float(&arr)[12]);

inline float ResultAy(const float(&arr)[12]);
inline float ResultBy(const float(&arr)[12]);
inline float ResultCy(const float(&arr)[12]);
inline float ResultDy(const float(&arr)[12]);

inline float ResultAz(const float(&arr)[12]);
inline float ResultBz(const float(&arr)[12]);
inline float ResultCz(const float(&arr)[12]);
inline float ResultDz(const float(&arr)[12]);

inline float ResultU(const float(&arr)[12]);
inline float ResultV(const float(&arr)[12]);
inline float ResultW(const float(&arr)[12]);

inline float ResultX0(const float(&arr)[12]);
inline float ResultY0(const float(&arr)[12]);
inline float ResultZ0(const float(&arr)[12]);
inline float ResultRadius(const float(&arr)[12]);

float** PaintBall(const int& size0, float** resultTetrUpPoints_1, const int& size1,
	float** resultTetrUpPoints_2, int& outIntersectingCount, std::vector<std::vector<int>>& Out);
//====================================================================================================//

//inline float** PaintBallPlusMuller(const int& size0, float** resultTetrUpPoints_1, const int& size1,
//	float** resultTetrUpPoints_2, int& outIntersectingCount, std::vector<std::vector<int>>& Out);