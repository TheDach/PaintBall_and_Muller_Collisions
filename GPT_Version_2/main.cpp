#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <array>

#include "PaintBall.h"
#include "Muller.h"
#include "DataHandler.h"


using namespace std;

//====================================== PATH ====================================================================//
#define pathPregrada "..\\Resurs_txt\\pregrada test new.txt"
#define pathUdarnikVnutri "..\\Resurs_txt\\\udarnik test new vnutri.txt"
#define pathUdarnikDaleko "..\\Resurs_txt\\udarnik test new daleko.txt"
#define pathUdarnik "..\\Resurs_txt\\udarnik test.txt"
#define pathCube "..\\Resurs_txt\\cube_10_elements.txt"
#define pathMillion "..\\Resurs_txt\\5_millions.txt"
#define pathPregradaCilindr "..\\Resurs_txt\\pregrada_cilindr.txt"
#define pathUdarnikCilindr "..\\Resurs_txt\\udarnik_cilindr.txt"

#define pathOutputBin1 "..\\Resurs_bin\\out1.bin"
#define pathOutputBin2 "..\\Resurs_bin\\out2.bin"



float volumeTetrahedron(const float (&coords)[12]) {
	//x1 = coords[0], y1 = coords[1], z1 = coords[2];
	//x2 = coords[3], y2 = coords[4], z2 = coords[5];
	//x3 = coords[6], y3 = coords[7], z3 = coords[8];
	//x4 = coords[9], y4 = coords[10], z4 = coords[11];

	return fabs(coords[0] * (coords[4] * coords[8] + coords[7] * coords[11] + coords[10] * coords[5] - coords[4] * coords[11] - coords[7] * coords[5] - coords[10] * coords[8]) -
		coords[3] * (coords[1] * coords[8] + coords[7] * coords[11] + coords[10] * coords[2] - coords[1] * coords[11] - coords[7] * coords[2] - coords[10] * coords[8]) +
		coords[6] * (coords[1] * coords[5] + coords[4] * coords[11] + coords[10] * coords[2] - coords[1] * coords[11] - coords[4] * coords[2] - coords[10] * coords[5]) -
		coords[9] * (coords[1] * coords[5] + coords[4] * coords[8] + coords[7] * coords[2] - coords[1] * coords[8] - coords[4] * coords[2] - coords[7] * coords[5])) / 6.0;
}

#define THREAD 4
#define FLAG_BIG_DICK false // Флаг для оптимизации алгоритма PaintBall, если фигуры большие, то ставить false

int main() {
	setlocale(LC_ALL, "Russian");
	omp_set_num_threads(THREAD);

	double start = 0;
	double end = 0;

	ConvertTxtToBin(pathPregradaCilindr, pathOutputBin1);
	ConvertTxtToBin(pathUdarnikCilindr, pathOutputBin2);


	//======================FIGURA1============================//
	int size1, size2, size3 = 0;//size1 - sizePoints(1 блок), size2 - sizeTetr (2 блок), size3 - sizeTriangle (3 блок)

	float(*arrayPoints)[3] = nullptr;//1Блок
	int(*indexPoints)[5] = nullptr;//2Блок
	int(*indexUzla)[4] = nullptr;//3Блок


	start = omp_get_wtime();
	int(*Tetrahedrons) = ReadFromFileVneshnieBinTest(pathOutputBin1, size1, size2, size3, arrayPoints, indexPoints, indexUzla);
	int** resultCreateTetrUpUzel = CreateTetrUpUzel(size3, indexPoints, Tetrahedrons);
	float** resultTetrUpPoints = CreateTetrUpPoints(size3, resultCreateTetrUpUzel, arrayPoints);
	end = omp_get_wtime();
	std::cout << "Время выполнения FIGURA1: " << end - start << " секунд." << endl;
	std::cout << "Кол-во поверхностных элементов: " << size3 << endl;

	//======================FIGURA2============================//
	int size1_2, size2_2, size3_2 = 0;//size1 - sizePoints(1 блок), size2 - sizeTetr (2 блок), size3 - sizeTriangle (3 блок)

	float(*arrayPoints_2)[3] = nullptr;//1Блок
	int(*indexPoints_2)[5] = nullptr;//2Блок
	int(*indexUzla_2)[4] = nullptr;//3Блок


	start = omp_get_wtime();
	int(*Tetrahedrons_2) = ReadFromFileVneshnieBinTest(pathOutputBin2, size1_2, size2_2, size3_2, arrayPoints_2, indexPoints_2, indexUzla_2);
	int** resultCreateTetrUpUzel_2 = CreateTetrUpUzel(size3_2, indexPoints_2, Tetrahedrons_2);
	float** resultTetrUpPoints_2 = CreateTetrUpPoints(size3_2, resultCreateTetrUpUzel_2, arrayPoints_2);
	end = omp_get_wtime();
	std::cout << "Время выполнения FIGURA2: " << end - start << " секунд." << endl;
	std::cout << "Кол-во поверхностных элементов : " << size3_2 << endl;
	//=========================================================//

	int count = 0; // - общее количество пересечений
	std::vector<std::vector<int>> Out; // - Массив, который содержит возможно пересекающиеся треугольники
	std::vector<std::vector<int>> Out2; // - Массив, который содержит 100% пересекающиеся треугольники
	//==========================================================//

	start = omp_get_wtime();
	PaintBall(size3, resultTetrUpPoints, size3_2, resultTetrUpPoints_2, count, Out, FLAG_BIG_DICK);
	end = omp_get_wtime();
	std::cout << "Время выполнения PaintBall: " << end - start << " секунд." << endl;
	start = omp_get_wtime();
	count = Muller(Out, count, resultTetrUpPoints, resultTetrUpPoints_2, Out2);
	end = omp_get_wtime();
	std::cout << "Время выполнения Muller: " << end - start << " секунд." << endl;
	std::cout << "Кол-во 100% пересечений: " << count << endl;

	//PrintPythonCollisium(count, Out2, resultTetrUpPoints, resultTetrUpPoints_2);


	

	//======================FIGURA1============================//
	if (arrayPoints) delete[] arrayPoints;
	if (indexPoints) delete[] indexPoints;
	if (indexUzla) delete[] indexUzla;
	if (Tetrahedrons) delete[] Tetrahedrons; // Убедитесь, что вы выделяли память для Tetrahedrons
	for (int i = 0; i < size3; i++) {
		if (resultCreateTetrUpUzel[i]) delete[] resultCreateTetrUpUzel[i];
	}
	delete[] resultCreateTetrUpUzel;

	for (int i = 0; i < size3; i++) {
		if (resultTetrUpPoints[i]) delete[] resultTetrUpPoints[i];
	}
	delete[] resultTetrUpPoints;

	//======================FIGURA2============================//
	if (arrayPoints_2) delete[] arrayPoints_2;
	if (indexPoints_2) delete[] indexPoints_2;
	if (indexUzla_2) delete[] indexUzla_2;
	if (Tetrahedrons_2) delete[] Tetrahedrons_2; // Убедитесь, что вы выделяли память для Tetrahedrons_2
	for (int i = 0; i < size3_2; i++) {
		if (resultCreateTetrUpUzel_2[i]) delete[] resultCreateTetrUpUzel_2[i];
	}
	delete[] resultCreateTetrUpUzel_2;

	for (int i = 0; i < size3_2; i++) {
		if (resultTetrUpPoints_2[i]) delete[] resultTetrUpPoints_2[i];
	}
	delete[] resultTetrUpPoints_2;
	//=========================================================//

	return 0;
}
