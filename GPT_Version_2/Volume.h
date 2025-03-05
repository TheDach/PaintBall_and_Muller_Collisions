#pragma once
#include <vector>

float volume(const float tet[4][3]);
bool isInsideTetrahedron(const float tet[4][3], const float point[3]);
bool checkIntersection(const float tet1[4][3], const float tet2[4][3], int& Uzel);

int V(std::vector<std::vector<int>>& Out, int size, float** resultTetrUpPoints, float** resultTetrUpPoints_2, std::vector<std::vector<int>>& Out2);

// Îבתולû ג סמכמ עוסעû
int V(const int& size3, const int& size3_2, float** resultTetrUpPoints, float** resultTetrUpPoints_2, std::vector<std::vector<int>>& Out2);