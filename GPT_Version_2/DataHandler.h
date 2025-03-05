#pragma once
#include <vector>
#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <omp.h>

void ConvertTxtToBin(const std::string& inputFile, const std::string& outputFile);
int* ReadFromFileVneshnieBinTest(std::string path, int& size1, int& size2, int& size3, float(*&arrayPoints)[3], int(*&indexPoints)[5], int(*&indexUzla)[4]);
int** CreateTetrUpUzel(int size3, int(*indexPoints)[5], int(*Tetrahedrons));
float** CreateTetrUpPoints(int size3, int** TetrUpUzel, float(*arrayPoints)[3]);

// Volume
int* ReadFromFileVneshnieBinForV(std::string path, int& size1, int& size2, int& size3, float(*&arrayPoints)[3], int(*&indexPoints)[5], int(*&indexUzla)[4]);
void WriteToFileBinary(const std::string& outputPath, int size1, int size2, int size3,
	float(*arrayPoints)[3], int(*indexPoints)[5], int(*indexUzla)[4], int* Tetrahedrons);

void ReadFromFileBinary(const std::string& inputPath, int& size1, int& size2, int& size3,
	float(*&arrayPoints)[3], int(*&indexPoints)[5], int(*&indexUzla)[4], int*& Tetrahedrons);
//

void PrintArrayPoints(int size1, float(*arrayPoints)[3]);
void PrintIndexPoints(int size2, int(*indexPoints)[5]);
void PrintIndexUzla(int size3, int(*indexUzla)[4]);
void PrintTetrahedronsUp(int size1, int size2, int size3, int(*Tetrahedrons));
void PrintResultCreateTetrUpUzel(int size3, int** resultCreateTetrUpUzel);
void PrintResultCreateTetrUpPoints(int size3, float** resultCreateTetrUpPoints);
void PrintPythonCollisium(int count, std::vector<std::vector<int>>& Out, float** resultTetrUpPoints, float** resultTetrUpPoints_2);

void PrintAll(int size1, int size2, int size3, float(*arrayPoints)[3], int(*indexPoints)[5], int(*indexUzla)[4], int(*Tetrahedrons));
void PrintOut(std::vector<std::vector<int>>& Out, int size);
