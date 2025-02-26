#include "PaintBall.h"

//====================================== PAINT BALL ==================================================//
inline float ResultAx(const float(&arr)[12])
{
	return (powf(arr[0], 2) + powf(arr[1], 2) + powf(arr[2], 2)) *
		(arr[4] * (arr[8] - arr[11]) +
			arr[7] * (arr[11] - arr[5]) +
			arr[10] * (arr[5] - arr[8]));
}
inline float ResultBx(const float(&arr)[12])
{
	return (powf(arr[3], 2) + powf(arr[4], 2) + powf(arr[5], 2)) *
		(arr[7] * (arr[11] - arr[2]) +
			arr[10] * (arr[2] - arr[8]) +
			arr[1] * (arr[8] - arr[11]));
}
inline float ResultCx(const float(&arr)[12])
{
	return (powf(arr[6], 2) + powf(arr[7], 2) + powf(arr[8], 2)) *
		(arr[10] * (arr[2] - arr[5]) +
			arr[1] * (arr[5] - arr[11]) +
			arr[4] * (arr[11] - arr[2]));
}
inline float ResultDx(const float(&arr)[12])
{
	return (powf(arr[9], 2) + powf(arr[10], 2) + powf(arr[11], 2)) *
		(arr[1] * (arr[5] - arr[8]) +
			arr[4] * (arr[8] - arr[2]) +
			arr[7] * (arr[2] - arr[5]));
}

inline float ResultAy(const float(&arr)[12])
{
	return (powf(arr[0], 2) + powf(arr[1], 2) + powf(arr[2], 2)) *
		(arr[5] * (arr[6] - arr[9]) +
			arr[8] * (arr[9] - arr[3]) +
			arr[11] * (arr[3] - arr[6]));
}
inline float ResultBy(const float(&arr)[12])
{
	return (powf(arr[3], 2) + powf(arr[4], 2) + powf(arr[5], 2)) *
		(arr[8] * (arr[9] - arr[0]) +
			arr[11] * (arr[0] - arr[6]) +
			arr[2] * (arr[6] - arr[9]));
}
inline float ResultCy(const float(&arr)[12])
{
	return (powf(arr[6], 2) + powf(arr[7], 2) + powf(arr[8], 2)) *
		(arr[11] * (arr[0] - arr[3]) +
			arr[2] * (arr[3] - arr[9]) +
			arr[5] * (arr[9] - arr[0]));
}
inline float ResultDy(const float(&arr)[12])
{
	return (powf(arr[9], 2) + powf(arr[10], 2) + powf(arr[11], 2)) *
		(arr[2] * (arr[3] - arr[6]) +
			arr[5] * (arr[6] - arr[0]) +
			arr[8] * (arr[0] - arr[3]));
}

inline float ResultAz(const float(&arr)[12])
{
	return (powf(arr[0], 2) + powf(arr[1], 2) + powf(arr[2], 2)) *
		(arr[3] * (arr[7] - arr[10]) +
			arr[6] * (arr[10] - arr[4]) +
			arr[9] * (arr[4] - arr[7]));
}
inline float ResultBz(const float(&arr)[12])
{
	return (powf(arr[3], 2) + powf(arr[4], 2) + powf(arr[5], 2)) *
		(arr[6] * (arr[10] - arr[1]) +
			arr[9] * (arr[1] - arr[7]) +
			arr[0] * (arr[7] - arr[10]));
}
inline float ResultCz(const float(&arr)[12])
{
	return (powf(arr[6], 2) + powf(arr[7], 2) + powf(arr[8], 2)) *
		(arr[9] * (arr[1] - arr[4]) +
			arr[0] * (arr[4] - arr[10]) +
			arr[3] * (arr[10] - arr[1]));
}
inline float ResultDz(const float(&arr)[12])
{
	return (powf(arr[9], 2) + powf(arr[10], 2) + powf(arr[11], 2)) *
		(arr[0] * (arr[4] - arr[7]) +
			arr[3] * (arr[7] - arr[1]) +
			arr[6] * (arr[1] - arr[4]));
}

inline float ResultU(const float(&arr)[12])
{
	return (arr[2] - arr[5]) * (arr[6] * arr[10] - arr[9] * arr[7]) -
		(arr[5] - arr[8]) * (arr[9] * arr[1] - arr[0] * arr[10]);
}
inline float ResultV(const float(&arr)[12])
{
	return (arr[8] - arr[11]) * (arr[0] * arr[4] - arr[3] * arr[1]) -
		(arr[11] - arr[2]) * (arr[3] * arr[7] - arr[6] * arr[4]);
}
inline float ResultW(const float(&arr)[12])
{
	return (arr[2] - arr[8]) * (arr[9] * arr[4] - arr[3] * arr[10]) -
		(arr[5] - arr[11]) * (arr[0] * arr[7] - arr[6] * arr[1]);
}

inline float ResultX0(const float(&arr)[12])
{
	return (ResultAx(arr) - ResultBx(arr) +
		ResultCx(arr) - ResultDx(arr)) /
		(2.0f * (ResultU(arr) + ResultV(arr) + ResultW(arr)));
}
inline float ResultY0(const float(&arr)[12])
{
	return (ResultAy(arr) - ResultBy(arr) +
		ResultCy(arr) - ResultDy(arr)) /
		(2.0f * (ResultU(arr) + ResultV(arr) + ResultW(arr)));
}
inline float ResultZ0(const float(&arr)[12])
{
	return (ResultAz(arr) - ResultBz(arr) +
		ResultCz(arr) - ResultDz(arr)) /
		(2.0f * (ResultU(arr) + ResultV(arr) + ResultW(arr)));
}
inline float ResultRadius(const float(&arr)[12])
{
	return sqrtf(powf(arr[0] - ResultX0(arr), 2) +
		powf(arr[1] - ResultY0(arr), 2) +
		powf(arr[2] - ResultZ0(arr), 2));
}


float** PaintBall(const int& size0, float** resultTetrUpPoints_1, const int& size1,
	float** resultTetrUpPoints_2, int& outIntersectingCount, std::vector<std::vector<int>>& Out, const bool &flag) {
	float* arrayResultX0 = new float[size0];
	float* arrayResultY0 = new float[size0];
	float* arrayResultZ0 = new float[size0];
	float* arrayResulRadius = new float[size0];

	float* arrayResultX02 = new float[size1];
	float* arrayResultY02 = new float[size1];
	float* arrayResultZ02 = new float[size1];
	float* arrayResulRadius2 = new float[size1];

	// ======================================= Result X0, Y0, Z0, Radius ======================================= //
	std::cout << std::endl << "Начало посчета: Result X0, Y0, Z0, Radius" << std::endl;
	double time_start = omp_get_wtime();
	float arr0[12] = { 0 };
	float arr1[12] = { 0 };
	for (int i = 0; i < size0; i++) {
		for (int j = 0; j < 12; j++)
		{
			arr0[j] = resultTetrUpPoints_1[i][j];
			//cout << resultTetrUpPoints_1[i][j] << "  ";
		}
		arrayResultX0[i] = ResultX0(arr0);
		arrayResultY0[i] = ResultY0(arr0);
		arrayResultZ0[i] = ResultZ0(arr0);
		arrayResulRadius[i] = ResultRadius(arr0);
	}
	//cout << '\n';
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < 12; j++)
		{
			arr1[j] = resultTetrUpPoints_2[i][j];
		}
		arrayResultX02[i] = ResultX0(arr1);
		arrayResultY02[i] = ResultY0(arr1);
		arrayResultZ02[i] = ResultZ0(arr1);
		arrayResulRadius2[i] = ResultRadius(arr1);
	}

	double time_end = omp_get_wtime();
	double duration = time_end - time_start;
	std::cout << "Время выполнения: " << duration << "Секунд" << std::endl;
	// ========================================================================================================= //

	int count1 = 0;
	int count2 = 0;

	// ========================= Блок if сделан для ускорения процесса не больших фигур ======================== //
	if (flag) {
		unsigned int sizeArray = size0 * size1;
		int counter = 0;
		float* arrayLength = new float[sizeArray];
		float* arraySumRadius = new float[sizeArray];

		std::cout << std::endl << "Начало посчета: SumRadius и Length" << std::endl;
		time_start = omp_get_wtime();
		for (int i = 0; i < size0; i++) {
			for (int j = 0; j < size1; j++) {
				arrayLength[counter] = sqrtf(powf(arrayResultX02[j] - arrayResultX0[i], 2) +
					powf(arrayResultY02[j] - arrayResultY0[i], 2) +
					powf(arrayResultZ02[j] - arrayResultZ0[i], 2));
				arraySumRadius[counter] = arrayResulRadius2[j] + arrayResulRadius[i];

				counter++;
			}
		}
		time_end = omp_get_wtime();
		duration = time_end - time_start;
		std::cout << "Время выполнения: " << duration << "Секунд" << std::endl;

		std::cout << std::endl << "Поиск пересечений..." << std::endl;
		time_start = omp_get_wtime();

#pragma omp parallel
		{
			int local_count1 = 0;
			//int local_count2 = 0;
			std::vector<std::vector<int>> local_Out;

#pragma omp for
			/*for (int i = 0; i < size0; i++) {
				for (int j = 0; j < size1; j++) {
					if (arraySumRadius[i * size1 + j] > arrayLength[i * size1 + j]) {
						local_count1++;
						local_Out.push_back({ i, j });
					}
				}
			}*/

			for (int i = 0; i < sizeArray; i++) {
				if (arraySumRadius[i] > arrayLength[i]) {

					local_count1++;
					local_Out.push_back({ i / size1, i % size1 });
				}
			}

#pragma omp critical
			{
				count1 += local_count1;
				//count2 += local_count2;
				Out.insert(Out.end(), local_Out.begin(), local_Out.end());
			}

		}
		time_end = omp_get_wtime();
		duration = time_end - time_start;
	} 
	// ========================= Блок else сделан для процесса c большими фигурами ============================= //
	else {
		std::cout << std::endl << "Поиск пересечений..." << std::endl;
		time_start = omp_get_wtime();
#pragma omp parallel
		{
			int local_count1 = 0;
			//int local_count2 = 0;
			std::vector<std::vector<int>> local_Out;

#pragma omp for
			
			for (int i = 0; i < size0; i++) {
				for (int j = 0; j < size1; j++) {
					float length = sqrtf(powf(arrayResultX02[j] - arrayResultX0[i], 2) +
						powf(arrayResultY02[j] - arrayResultY0[i], 2) +
						powf(arrayResultZ02[j] - arrayResultZ0[i], 2));
					float sumRadius = arrayResulRadius2[j] + arrayResulRadius[i];

					if (sumRadius > length) {
						local_count1++;

						local_Out.push_back({ i, j });////

						//for (int l = 0; l < 12; l++) {
						//	arr0_new[l] = resultTetrUpPoints_1[i][l];
						//	arr1_new[l] = resultTetrUpPoints_2[j][l];
						//}
						//if (tetras_intersect(arr0_new, arr1_new)) {
						//	local_count2++;
						//	//local_Out.push_back({ i, j });
						//	
						//}
					}
				}
			}

			// Объединяем локальные результаты с глобальными
#pragma omp critical
			{
				count1 += local_count1;
				//count2 += local_count2;
				Out.insert(Out.end(), local_Out.begin(), local_Out.end());
			}
		}

		time_end = omp_get_wtime();
		duration = time_end - time_start;
	}


	std::cout << "Время выполнения: " << duration << "Секунд" << std::endl;
	std::cout << "Кол-во пересечений: " << count1 << std::endl;
	//std::cout << "Кол-во 100% пересечений: " << count2 << std::endl;

	outIntersectingCount = count1;

	delete[]arrayResultX0;
	delete[]arrayResultY0;
	delete[]arrayResultZ0;
	delete[]arrayResulRadius;

	delete[]arrayResultX02;
	delete[]arrayResultY02;
	delete[]arrayResultZ02;
	delete[]arrayResulRadius2;

	return nullptr;
}
//====================================================================================================//


//inline float** PaintBallPlusMuller(const int& size0, float** resultTetrUpPoints_1, const int& size1,
//	float** resultTetrUpPoints_2, int& outIntersectingCount, std::vector<std::vector<int>>& Out) {
//	float* arrayResultX0 = new float[size0];
//	float* arrayResultY0 = new float[size0];
//	float* arrayResultZ0 = new float[size0];
//	float* arrayResulRadius = new float[size0];
//
//	float* arrayResultX02 = new float[size1];
//	float* arrayResultY02 = new float[size1];
//	float* arrayResultZ02 = new float[size1];
//	float* arrayResulRadius2 = new float[size1];
//
//
//	std::cout << std::endl << "Начало посчета: Result X0, Y0, Z0, Radius" << std::endl;
//	double time_start = omp_get_wtime();
//	float arr0[12] = { 0 };
//	float arr1[12] = { 0 };
//	for (int i = 0; i < size0; i++) {
//		for (int j = 0; j < 12; j++)
//		{
//			arr0[j] = resultTetrUpPoints_1[i][j];
//			//cout << resultTetrUpPoints_1[i][j] << "  ";
//		}
//		arrayResultX0[i] = ResultX0(arr0);
//		arrayResultY0[i] = ResultY0(arr0);
//		arrayResultZ0[i] = ResultZ0(arr0);
//		arrayResulRadius[i] = ResultRadius(arr0);
//	}
//	//cout << '\n';
//	for (int i = 0; i < size1; i++) {
//		for (int j = 0; j < 12; j++)
//		{
//			arr1[j] = resultTetrUpPoints_2[i][j];
//		}
//		arrayResultX02[i] = ResultX0(arr1);
//		arrayResultY02[i] = ResultY0(arr1);
//		arrayResultZ02[i] = ResultZ0(arr1);
//		arrayResulRadius2[i] = ResultRadius(arr1);
//	}
//
//	double time_end = omp_get_wtime();
//	double duration = time_end - time_start;
//	std::cout << "Время выполнения: " << duration << "Секунд" << std::endl;
//
//
//	int count1 = 0;
//	int count2 = 0;
//	std::cout << std::endl << "Поиск пересечений..." << std::endl;
//	double time_start2 = omp_get_wtime();
//
//
//#pragma omp parallel
//	{
//		int local_count1 = 0;
//		int local_count2 = 0;
//		std::vector<std::vector<int>> local_Out;
//
//#pragma omp for
//		for (int i = 0; i < size0; i++) {
//			for (int j = 0; j < size1; j++) {
//				float length = sqrtf(powf(arrayResultX02[j] - arrayResultX0[i], 2) +
//					powf(arrayResultY02[j] - arrayResultY0[i], 2) +
//					powf(arrayResultZ02[j] - arrayResultZ0[i], 2));
//				float sumRadius = arrayResulRadius2[j] + arrayResulRadius[i];
//
//				if (sumRadius > length) {
//					local_count1++;
//					float arr0_new[12] = { 0 };
//					float arr1_new[12] = { 0 };
//
//					for (int l = 0; l < 12; l++) {
//						arr0_new[l] = resultTetrUpPoints_1[i][l];
//						arr1_new[l] = resultTetrUpPoints_2[j][l];
//					}
//					if (tetras_intersect(arr0_new, arr1_new)) {
//						local_count2++;
//						local_Out.push_back({ i, j });
//					}
//				}
//			}
//		}
//
//		// Объединяем локальные результаты с глобальными
//#pragma omp critical
//		{
//			count1 += local_count1;
//			count2 += local_count2;
//			Out.insert(Out.end(), local_Out.begin(), local_Out.end());
//		}
//	}
//
//	double time_end2 = omp_get_wtime();
//	double duration2 = time_end2 - time_start2;
//	std::cout << "Время выполнения: " << duration2 << "Секунд" << std::endl;
//	std::cout << "Кол-во пересечений: " << count1 << std::endl;
//	std::cout << "Кол-во 100% пересечений: " << count2 << std::endl;
//
//	outIntersectingCount = count2;
//
//	delete[]arrayResultX0;
//	delete[]arrayResultY0;
//	delete[]arrayResultZ0;
//	delete[]arrayResulRadius;
//
//	delete[]arrayResultX02;
//	delete[]arrayResultY02;
//	delete[]arrayResultZ02;
//	delete[]arrayResulRadius2;
//
//	return nullptr;
//}
