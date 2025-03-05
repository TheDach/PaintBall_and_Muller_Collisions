#pragma once

#include <iostream>
#include <cmath>
#include <array>
#include <omp.h>
#include <vector>


//====================================== 100% ÏÅĞÅÑÅ×ÅÍÈß | MULLER ====================================//
int coplanar_tri_tri(float N[3], float V0[3], float V1[3], float V2[3],
	float U0[3], float U1[3], float U2[3]);
int tri_tri_intersect(float V0[3], float V1[3], float V2[3],
	float U0[3], float U1[3], float U2[3]);
void PullTriangle(const float(&tetr)[12], float(&out)[9], int index);
bool tetras_intersect(const float(&tetr0)[12], const float(&tetr1)[12]);

//int Muller(std::vector<std::vector<int>>& Out, int size, float** resultTetrUpPoints,
//	float** resultTetrUpPoints_2, std::vector<std::vector<int>>& Out2);
int Muller(std::vector<std::vector<int>>& Out, int size, float** resultTetrUpPoints,
	float** resultTetrUpPoints_2, std::vector<std::vector<int>>& Out2);

// Muller â ñîëî òåñòû
int Muller(int size1, int size2, float** resultTetrUpPoints,
	float** resultTetrUpPoints_2, std::vector<std::vector<int>>& Out2);

//====================================================================================================//