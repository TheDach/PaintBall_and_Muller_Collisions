#include "Volume.h"

float volume(const float tet[4][3]) {
	float matrix[4][4] = {
		{ tet[0][0], tet[0][1], tet[0][2], 1.0f },
		{ tet[1][0], tet[1][1], tet[1][2], 1.0f },
		{ tet[2][0], tet[2][1], tet[2][2], 1.0f },
		{ tet[3][0], tet[3][1], tet[3][2], 1.0f }
	};

	// Определитель матрицы 4x4 (вычисление через разложение по строкам или столбцам)
	float det = 0.0f;
	for (int i = 0; i < 4; i++) {
		float submatrix[3][3];
		for (int j = 1; j < 4; j++) {
			int colIndex = 0;
			for (int k = 0; k < 4; k++) {
				if (k != i) {
					submatrix[j - 1][colIndex] = matrix[j][k];
					colIndex++;
				}
			}
		}
		// Вычисление определителя 3x3
		float subDet = submatrix[0][0] * (submatrix[1][1] * submatrix[2][2] - submatrix[1][2] * submatrix[2][1])
			- submatrix[0][1] * (submatrix[1][0] * submatrix[2][2] - submatrix[1][2] * submatrix[2][0])
			+ submatrix[0][2] * (submatrix[1][0] * submatrix[2][1] - submatrix[1][1] * submatrix[2][0]);
		det += (i % 2 == 0 ? 1 : -1) * matrix[0][i] * subDet;
	}

	// Объем тетраэдра
	return std::abs(det) / 6.0f;
}
bool isInsideTetrahedron(const float tet[4][3], const float point[3]) {
	float volOrig = volume(tet);

	// Разбиваем тетраэдр на 4 маленьких тетраэдра с одной из вершин как точкой
	float tet1[4][3] = { {point[0], point[1], point[2]}, {tet[1][0], tet[1][1], tet[1][2]}, {tet[2][0], tet[2][1], tet[2][2]}, {tet[3][0], tet[3][1], tet[3][2]} };
	float tet2[4][3] = { {tet[0][0], tet[0][1], tet[0][2]}, {point[0], point[1], point[2]}, {tet[2][0], tet[2][1], tet[2][2]}, {tet[3][0], tet[3][1], tet[3][2]} };
	float tet3[4][3] = { {tet[0][0], tet[0][1], tet[0][2]}, {tet[1][0], tet[1][1], tet[1][2]}, {point[0], point[1], point[2]}, {tet[3][0], tet[3][1], tet[3][2]} };
	float tet4[4][3] = { {tet[0][0], tet[0][1], tet[0][2]}, {tet[1][0], tet[1][1], tet[1][2]}, {tet[2][0], tet[2][1], tet[2][2]}, {point[0], point[1], point[2]} };

	float vol1 = volume(tet1);
	float vol2 = volume(tet2);
	float vol3 = volume(tet3);
	float vol4 = volume(tet4);

	// Если сумма объемов маленьких тетраэдров равна исходному объему, то точка внутри
	return std::abs(vol1 + vol2 + vol3 + vol4 - volOrig) < 1e-6;
}
bool checkIntersection(const float tet1[4][3], const float tet2[4][3], int& Uzel) {
	for (int i = 0; i < 4; i++) {
		if (isInsideTetrahedron(tet1, tet2[i])) {
			Uzel = i;
			return true;
		}
		if (isInsideTetrahedron(tet2, tet1[i])) {
			Uzel = -i;
			return true;
		}
	}
	return false;
}


int V(std::vector<std::vector<int>>& Out, int size, float** resultTetrUpPoints, float** resultTetrUpPoints_2, std::vector<std::vector<int>>& Out2)
{
	int count = 0;  // Общее количество пересечений
	int Uzel = -1;  // Индекс для функции проверки пересечения

	// Применение OpenMP для распараллеливания цикла
#pragma omp parallel
	{
		// Локальные переменные для каждого потока
		int localCount = 0;
		std::vector<std::vector<int>> localOut2;

		// Массивы для хранения точек тетраэдров
		float arr0[12] = { 0 }, arr1[12] = { 0 };

#pragma omp for nowait
		for (int i = 0; i < size; i++) {
			int index1 = Out[i][0];
			int index2 = Out[i][1];

			// Копирование данных из глобальных массивов в локальные
			for (int j = 0; j < 12; j++) {
				arr0[j] = resultTetrUpPoints[index1][j];
				arr1[j] = resultTetrUpPoints_2[index2][j];
			}

			// Преобразуем массивы arr0 и arr1 в двумерные массивы для работы с функцией проверки пересечения
			float tet1[4][3] = {
				{arr0[0], arr0[1], arr0[2]},
				{arr0[3], arr0[4], arr0[5]},
				{arr0[6], arr0[7], arr0[8]},
				{arr0[9], arr0[10], arr0[11]}
			};

			float tet2[4][3] = {
				{arr1[0], arr1[1], arr1[2]},
				{arr1[3], arr1[4], arr1[5]},
				{arr1[6], arr1[7], arr1[8]},
				{arr1[9], arr1[10], arr1[11]}
			};

			// Проверяем пересечение тетраэдров
			if (checkIntersection(tet1, tet2, Uzel)) {
				// Если пересечение есть, добавляем результат в локальный массив
				localOut2.push_back({ index1, index2, Uzel });
				localCount++;
			}
		}

		// Обновление глобальных данных после завершения параллельной части
#pragma omp critical
		{
			count += localCount;
			Out2.insert(Out2.end(), localOut2.begin(), localOut2.end());
		}
	}

	return count;
}


// Объемы в соло тесты
int V(const int& size3, const int& size3_2, float** resultTetrUpPoints, float** resultTetrUpPoints_2, std::vector<std::vector<int>>& Out2) {
	int count = 0;  // Общее количество пересечений
	int Uzel = -1;  // Индекс для функции проверки пересечения

	// Применение OpenMP для распараллеливания цикла
#pragma omp parallel
	{
		// Локальные переменные для каждого потока
		int localCount = 0;
		std::vector<std::vector<int>> localOut2;

		// Массивы для хранения точек тетраэдров
		float arr0[12] = { 0 }, arr1[12] = { 0 };

#pragma omp for nowait
		for (int i = 0; i < size3; i++) {
			for (int k = 0; k < size3_2; k++) {

				// Копирование данных из глобальных массивов в локальные
				for (int j = 0; j < 12; j++) {
					arr0[j] = resultTetrUpPoints[i][j];
					arr1[j] = resultTetrUpPoints_2[k][j];
				}

				// Преобразуем массивы arr0 и arr1 в двумерные массивы для работы с функцией проверки пересечения
				float tet1[4][3] = {
					{arr0[0], arr0[1], arr0[2]},
					{arr0[3], arr0[4], arr0[5]},
					{arr0[6], arr0[7], arr0[8]},
					{arr0[9], arr0[10], arr0[11]}
				};

				float tet2[4][3] = {
					{arr1[0], arr1[1], arr1[2]},
					{arr1[3], arr1[4], arr1[5]},
					{arr1[6], arr1[7], arr1[8]},
					{arr1[9], arr1[10], arr1[11]}
				};

				// Проверяем пересечение тетраэдров
				if (checkIntersection(tet1, tet2, Uzel)) {
					// Если пересечение есть, добавляем результат в локальный массив
					localOut2.push_back({ i, k, Uzel });
					localCount++;
				}
			}
		}

		// Обновление глобальных данных после завершения параллельной части
#pragma omp critical
		{
			count += localCount;
			Out2.insert(Out2.end(), localOut2.begin(), localOut2.end());
		}
	}

	return count;
}
