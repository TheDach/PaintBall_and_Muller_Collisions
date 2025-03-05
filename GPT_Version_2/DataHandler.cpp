#include "DataHandler.h"

void ConvertTxtToBin(const std::string& inputFile, const std::string& outputFile) {
	std::ifstream in(inputFile);
	std::ofstream out(outputFile, std::ios::binary);
	if (!in.is_open()) {
		std::cout << "ERROR: Unable to open input file." << std::endl;
		return;
	}

	if (!out.is_open()) {
		std::cout << "ERROR: Unable to open output file." << std::endl;
		return;
	}
	std::cout << "Start convert!" << std::endl;
	// Считываем количество точек
	int sizePoint;
	in >> sizePoint;
	out.write(reinterpret_cast<char*>(&sizePoint), sizeof(int));

	// Считываем и записываем точки (вершины)
	for (int i = 0; i < sizePoint; i++) {
		float x, y, z;
		in >> x >> y >> z;
		out.write(reinterpret_cast<char*>(&x), sizeof(float));
		out.write(reinterpret_cast<char*>(&y), sizeof(float));
		out.write(reinterpret_cast<char*>(&z), sizeof(float));
	}

	// Считываем количество тетраэдров
	int sizeTetrahedrons;
	in >> sizeTetrahedrons;
	out.write(reinterpret_cast<char*>(&sizeTetrahedrons), sizeof(int));

	// Считываем и записываем тетраэдры (индексы вершин)
	for (int i = 0; i < sizeTetrahedrons; i++) {
		int j, id_x, id_y, id_z, id_d;
		in >> j >> id_x >> id_y >> id_z >> id_d;
		out.write(reinterpret_cast<char*>(&j), sizeof(int)); // Хотя это значение нам не нужно, мы записываем его
		out.write(reinterpret_cast<char*>(&id_x), sizeof(int));
		out.write(reinterpret_cast<char*>(&id_y), sizeof(int));
		out.write(reinterpret_cast<char*>(&id_z), sizeof(int));
		out.write(reinterpret_cast<char*>(&id_d), sizeof(int));
	}

	// Считываем количество треугольников
	int sizeTriangle;
	in >> sizeTriangle;
	out.write(reinterpret_cast<char*>(&sizeTriangle), sizeof(int));

	// Считываем и записываем треугольники (индексы вершин)
	for (int i = 0; i < sizeTriangle; i++) {
		int j, a, b, c;
		in >> j >> a >> b >> c;
		out.write(reinterpret_cast<char*>(&j), sizeof(int)); // Хотя это значение нам не нужно, мы записываем его
		out.write(reinterpret_cast<char*>(&a), sizeof(int));
		out.write(reinterpret_cast<char*>(&b), sizeof(int));
		out.write(reinterpret_cast<char*>(&c), sizeof(int));
	}

	std::cout << "Conversion completed successfully!" << std::endl;

	in.close();
	out.close();
}
int* ReadFromFileVneshnieBinTest(std::string path, int& size1, int& size2, int& size3, float(*&arrayPoints)[3], int(*&indexPoints)[5], int(*&indexUzla)[4]) {
	std::ifstream in;
	in.open(path, std::ios::in | std::ios::binary);
	if (in.is_open()) {
		std::cout << std::endl << "File opened" << std::endl;

		int sizePoint;
		in.read(reinterpret_cast<char*>(&sizePoint), sizeof(int));
		size1 = sizePoint;

		arrayPoints = new float[sizePoint][3];
		for (int i = 0; i < sizePoint; i++) {
			in.read(reinterpret_cast<char*>(&arrayPoints[i][0]), 3 * sizeof(float));
		}

		int sizeTetrahedrons;
		in.read(reinterpret_cast<char*>(&sizeTetrahedrons), sizeof(int));
		size2 = sizeTetrahedrons;

		indexPoints = new int[sizeTetrahedrons][5];
		for (int i = 0; i < sizeTetrahedrons; i++) {
			in.read(reinterpret_cast<char*>(&indexPoints[i][0]), 5 * sizeof(int));
		}

		int sizeTriangle;
		in.read(reinterpret_cast<char*>(&sizeTriangle), sizeof(int));
		indexUzla = new int[sizeTriangle][4];
		for (int i = 0; i < sizeTriangle; i++) {
			in.read(reinterpret_cast<char*>(&indexUzla[i][0]), 4 * sizeof(int));
		}

		int* Tetrahedrons = new int[sizeTriangle];
		int countcount = 0;

		// Количество потоков
		int numThreads = omp_get_max_threads();

		// Локальный массив для каждого потока
		std::vector<int*> localTetrahedrons(numThreads);
		std::vector<int> localCount(numThreads, 0);

		// Параллельная обработка
#pragma omp parallel
		{

			int threadId = omp_get_thread_num();  // ID текущего потока
			localTetrahedrons[threadId] = new int[sizeTriangle];

			// Параллельная обработка цикла по треугольникам
#pragma omp for
			for (int i = 0; i < sizeTriangle; i++) {
				int a = indexUzla[i][1];
				int b = indexUzla[i][2];
				int c = indexUzla[i][3];

				for (int j = 0; j < sizeTetrahedrons; j++) {
					if ((indexPoints[j][1] == a || indexPoints[j][2] == a || indexPoints[j][3] == a || indexPoints[j][4] == a) &&
						(indexPoints[j][1] == b || indexPoints[j][2] == b || indexPoints[j][3] == b || indexPoints[j][4] == b) &&
						(indexPoints[j][1] == c || indexPoints[j][2] == c || indexPoints[j][3] == c || indexPoints[j][4] == c))
					{
						localTetrahedrons[threadId][localCount[threadId]] = j;
						localCount[threadId]++;
						break;
					}
				}
			}
		}

		// Объединение локальных массивов в общий
		countcount = 0;
		for (int i = 0; i < numThreads; i++) {
			for (int j = 0; j < localCount[i]; j++) {
				Tetrahedrons[countcount++] = localTetrahedrons[i][j];
			}
			delete[] localTetrahedrons[i];  // Освобождение локальных массивов
		}

		std::cout << "\rЗагрузка завершена!" << std::endl;
		size3 = sizeTriangle;
		in.close();
		return Tetrahedrons;
	}
	else {
		std::cout << "ERROR: File is not opened" << std::endl;
		in.close();
		return nullptr;
	}
}
int** CreateTetrUpUzel(int size3, int(*indexPoints)[5], int(*Tetrahedrons))
{

	int** arr = new int* [size3]; // Выделяем память для массива указателей
	for (int i = 0; i < size3; i++)
	{
		int index = Tetrahedrons[i];
		arr[i] = new int[4]; // Выделяем память для каждого из 4 элементов
		for (int j = 0; j < 4; j++)
		{
			arr[i][j] = indexPoints[index][j + 1];
		}
	}
	return arr;
}
float** CreateTetrUpPoints(int size3, int** TetrUpUzel, float(*arrayPoints)[3])
{
	float** arr = new float* [size3];
	for (int i = 0; i < size3; i++)
	{
		int index1 = TetrUpUzel[i][0];
		int index2 = TetrUpUzel[i][1];
		int index3 = TetrUpUzel[i][2];
		int index4 = TetrUpUzel[i][3];
		arr[i] = new float[12];


		arr[i][0] = arrayPoints[index1 - 1][0];
		arr[i][1] = arrayPoints[index1 - 1][1];
		arr[i][2] = arrayPoints[index1 - 1][2];

		arr[i][3] = arrayPoints[index2 - 1][0];
		arr[i][4] = arrayPoints[index2 - 1][1];
		arr[i][5] = arrayPoints[index2 - 1][2];

		arr[i][6] = arrayPoints[index3 - 1][0];
		arr[i][7] = arrayPoints[index3 - 1][1];
		arr[i][8] = arrayPoints[index3 - 1][2];

		arr[i][9] = arrayPoints[index4 - 1][0];
		arr[i][10] = arrayPoints[index4 - 1][1];
		arr[i][11] = arrayPoints[index4 - 1][2];
	}

	return arr;
}


// Volume
int* ReadFromFileVneshnieBinForV(std::string path, int& size1, int& size2, int& size3, float(*&arrayPoints)[3], int(*&indexPoints)[5], int(*&indexUzla)[4]) {
	std::ifstream in;
	in.open(path, std::ios::in | std::ios::binary);
	if (in.is_open()) {
		std::cout << "File opened" << std::endl;

		int sizePoint;
		in.read(reinterpret_cast<char*>(&sizePoint), sizeof(int));
		size1 = sizePoint;

		arrayPoints = new float[sizePoint][3];
		for (int i = 0; i < sizePoint; i++) {
			in.read(reinterpret_cast<char*>(&arrayPoints[i][0]), 3 * sizeof(float));
		}

		int sizeTetrahedrons;
		in.read(reinterpret_cast<char*>(&sizeTetrahedrons), sizeof(int));
		size2 = sizeTetrahedrons;

		indexPoints = new int[sizeTetrahedrons][5];
		for (int i = 0; i < sizeTetrahedrons; i++) {
			in.read(reinterpret_cast<char*>(&indexPoints[i][0]), 5 * sizeof(int));
		}

		int sizeTriangle;
		in.read(reinterpret_cast<char*>(&sizeTriangle), sizeof(int));
		indexUzla = new int[sizeTriangle][4];
		for (int i = 0; i < sizeTriangle; i++) {
			in.read(reinterpret_cast<char*>(&indexUzla[i][0]), 4 * sizeof(int));
		}

		int max = -1;
		for (int i = 0; i < sizeTriangle; i++) {
			for (int j = 0; j < 4; j++) {
				if (indexUzla[i][j] > max) {
					max = indexUzla[i][j];  // Обновляем max, если нашли большее значение
				}
			}
		}
		std::cout << "Max: " << max << std::endl;



		std::set<std::set<int>> uniqueTetrandons;
		std::set<int> indexArr;
		int count = 0;
		for (int a = 1; a <= max; a++) {
			std::set<int> currentSet; // Создаем новый набор для текущего a
			for (int j = 0; j < sizeTetrahedrons; j++) {
				if (indexPoints[j][1] == a || indexPoints[j][2] == a || indexPoints[j][3] == a || indexPoints[j][4] == a) {
					currentSet.insert(indexPoints[j][1]);
					currentSet.insert(indexPoints[j][2]);
					currentSet.insert(indexPoints[j][3]);
					currentSet.insert(indexPoints[j][4]);
					indexArr.insert(j);
					uniqueTetrandons.insert(currentSet);
					count++;
					currentSet.clear();
				}
			}
		}
		//printUnique(uniqueTetrandons, arrayPoints);
		int* Tetrahedrons = new int[indexArr.size()];
		int i = 0;
		for (auto it = indexArr.begin(); it != indexArr.end(); ++it) {
			Tetrahedrons[i] = *it;
			i++;
		}
		std::cout << std::endl;
		int index2 = 0;

		std::cout << "\rЗагрузка завершена!" << std::endl;
		//std::cout << uniqueTetrandons.size()<<std::endl;
		size3 = uniqueTetrandons.size();

		in.close();
		return Tetrahedrons;
	}
	else {
		std::cout << "ERROR: File is not opened" << std::endl;
		in.close();

	}
}

void WriteToFileBinary(const std::string& outputPath, int size1, int size2, const int size3,
	float(*arrayPoints)[3], int(*indexPoints)[5], int(*indexUzla)[4], int* Tetrahedrons) {
	std::ofstream out;
	out.open(outputPath, std::ios::out | std::ios::binary);
	if (out.is_open()) {
		std::cout << "File opened for writing" << std::endl;

		// Записываем размеры
		out.write(reinterpret_cast<const char*>(&size1), sizeof(int));
		out.write(reinterpret_cast<const char*>(&size2), sizeof(int));
		out.write(reinterpret_cast<const char*>(&size3), sizeof(int));

		// Записываем массивы
		out.write(reinterpret_cast<const char*>(arrayPoints), size1 * 3 * sizeof(float));
		out.write(reinterpret_cast<const char*>(indexPoints), size2 * 5 * sizeof(int));
		out.write(reinterpret_cast<const char*>(indexUzla), size3 * 4 * sizeof(int));

		// Записываем Tetrahedrons
		out.write(reinterpret_cast<const char*>(Tetrahedrons), size3 * sizeof(int));

		std::cout << "Data written to file successfully!" << std::endl;
		out.close();
	}
	else {
		std::cout << "ERROR: File is not opened for writing" << std::endl;
	}
}

void ReadFromFileBinary(const std::string& inputPath, int& size1, int& size2, int& size3,
	float(*&arrayPoints)[3], int(*&indexPoints)[5], int(*&indexUzla)[4], int*& Tetrahedrons) {
	std::ifstream in;
	in.open(inputPath, std::ios::in | std::ios::binary);
	if (in.is_open()) {
		std::cout << "File opened for reading" << std::endl;

		// Считываем размеры
		in.read(reinterpret_cast<char*>(&size1), sizeof(int));
		in.read(reinterpret_cast<char*>(&size2), sizeof(int));
		in.read(reinterpret_cast<char*>(&size3), sizeof(int));

		std::cout << size1 << "\t" << size2 << "\t" << size3 << std::endl;

		// Выделяем память для массивов
		arrayPoints = new float[size1][3];
		in.read(reinterpret_cast<char*>(arrayPoints), size1 * 3 * sizeof(float));

		indexPoints = new int[size2][5];
		in.read(reinterpret_cast<char*>(indexPoints), size2 * 5 * sizeof(int));

		indexUzla = new int[size3][4];
		in.read(reinterpret_cast<char*>(indexUzla), size3 * 4 * sizeof(int));

		// Выделяем память для Tetrahedrons
		Tetrahedrons = new int[size3];
		in.read(reinterpret_cast<char*>(Tetrahedrons), size3 * sizeof(int));

		std::cout << "Data read from file successfully!" << std::endl;

		in.close();
	}
	else {
		std::cout << "ERROR: File is not opened for reading" << std::endl;
	}
}
//

void PrintArrayPoints(int size1, float(*arrayPoints)[3]) {
	std::cout << "Количество точек: " << size1 << std::endl;
	for (int i = 0; i < size1; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << arrayPoints[i][j] << '\t';
		}
		std::cout << std::endl;
	}
}
void PrintIndexPoints(int size2, int(*indexPoints)[5]) {
	std::cout << "Количество тетраэдров: " << size2 << std::endl;
	for (int i = 0; i < size2; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			std::cout << indexPoints[i][j] << '\t';
		}
		std::cout << std::endl;
	}
}
void PrintIndexUzla(int size3, int(*indexUzla)[4]) {
	std::cout << "Количество поверхностных треугольников: " << size3 << std::endl;
	for (int i = 0; i < size3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << indexUzla[i][j] << '\t';
		}
		std::cout << std::endl;
	}
}
void PrintTetrahedronsUp(int size1, int size2, int size3, int(*Tetrahedrons))
{
	if (Tetrahedrons != nullptr) {
		std::cout << "Данные успешно считаны из файла!" << std::endl;
		std::cout << "Количество точек: " << size1 << std::endl;
		std::cout << "Количество тетраэдров: " << size2 << std::endl;
		std::cout << "Количество внешних тетраэдров: " << size3 << std::endl;
		std::cout << "Отображение поверхностных тетраэдров: Построчное соотношение из блока3 с блоком2:" << std::endl;

		for (int i = 0; i < size3; i++) {
			//std::cout << "Строчка #" << i << " из 3блока - это строка#" << Tetrahedrons[i] << " из 2блока\n";
			std::cout << Tetrahedrons[i] << "\n";
		}
	}
	else {
		std::cout << "Ошибка при чтении файла." << std::endl;
	}
}
void PrintResultCreateTetrUpUzel(int size3, int** resultCreateTetrUpUzel)
{
	for (int i = 0; i < size3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << resultCreateTetrUpUzel[i][j] << ' ';
		}
		std::cout << '\n';
	}
}
void PrintResultCreateTetrUpPoints(int size3, float** resultCreateTetrUpPoints)
{
	for (int i = 0; i < size3; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			std::cout << resultCreateTetrUpPoints[i][j] << ' ';
		}
		std::cout << '\n';
	}
}
void PrintPythonCollisium(int count, std::vector<std::vector<int>>& Out, float** resultTetrUpPoints, float** resultTetrUpPoints_2) {
	int index1 = 0;
	int index2 = 0;
	for (int i = 0; i < count; i++) {
		//cout << "Тетр. " << index1 << " фигуры 0 пересекается с тетр. " << index2 << " фигуры 1\n";
		std::cout << "np.array([";
		index1 = Out[i][0];
		index2 = Out[i][1];
		for (int j = 0; j < 12; j++) {
			if (j != 11) {
				std::cout << resultTetrUpPoints[index1][j] << ','; // 1 фигура
			}
			else {
				std::cout << resultTetrUpPoints[index1][j];
			}
		}
		std::cout << "]), ";
		std::cout << std::endl;

		std::cout << "np.array([";
		for (int j = 0; j < 12; j++) {
			if (j != 11) {
				std::cout << resultTetrUpPoints_2[index2][j] << ',';
			}
			else {
				std::cout << resultTetrUpPoints_2[index2][j];
			}
		}
		std::cout << "]), ";
		std::cout << std::endl;
		std::cout << "\n\n";
	}
}


void PrintAll(int size1, int size2, int size3, float(*arrayPoints)[3], int(*indexPoints)[5], int(*indexUzla)[4], int(*Tetrahedrons))
{
	PrintArrayPoints(size1, arrayPoints);
	PrintIndexPoints(size2, indexPoints);
	PrintIndexUzla(size3, indexUzla);
	PrintTetrahedronsUp(size1, size2, size3, Tetrahedrons);
}
void PrintOut(std::vector<std::vector<int>>& Out, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << Out[i][j] << '\t';
		}
		std::cout << '\n';
	}
}