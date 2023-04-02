// PapalelProgramming.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <chrono>

using namespace std;

void randomiseMatrix(int** matrix, int n, int m) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			matrix[i][j] = rand() % 11;
		}
	}

	return;
}

int main(int argc, char** argv) {

	srand(time(NULL));
	int n1 = 1000;
	int m1 = 1000;
	int n2 = 1000;
	int m2 = 1000;

	//Матрица n1 x m1
	int** matrix1;
	//Матрица n2 x m2
	int** matrix2;

	matrix1 = (int**)malloc(sizeof(int*) * n1);
	for (int i = 0; i < n1; i++) {
		matrix1[i] = (int*)malloc(sizeof(int) * m1);
	}
	matrix2 = (int**)malloc(sizeof(int*) * n2);
	for (int i = 0; i < n2; i++) {
		matrix2[i] = (int*)malloc(sizeof(int) * m2);
	}

	//Генерируем случайные матрицы для умножения
	randomiseMatrix(matrix1, n1, m1);
	randomiseMatrix(matrix2, n2, m2);

	int** result = (int**)malloc(sizeof(int*) * n1);;
	for (int i = 0; i < n1; i++) {
		result[i] = (int*)malloc(sizeof(int) * m2);
	}

	//Устанавливаем число потоков

	double times [6];
	double maxTime = 0;

	for (int q = 0; q < 6; q++) {
		auto start_time = std::chrono::steady_clock::now();

		int threadsNum = q+1;
		omp_set_num_threads(threadsNum);
		int i, j, k;
#pragma omp parallel for shared(matrix1, matrix2, result) private(i, j, k)
		for (i = 0; i < n1; i++) {
			for (j = 0; j < m2; j++) {
				result[i][j] = 0;
				for (k = 0; k < m1; k++) {
					result[i][j] += (matrix1[i][k] * matrix2[k][j]);
				}
			}
		}

		auto end_time = chrono::steady_clock::now();
		auto elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
		times[q] = elapsed_ns.count();
		if (times[q] > maxTime) { maxTime = times[q];}
		//cout << " " << times[q] << " ns\n";

	}
	//Вывод
	for (int q = 0; q < 6; q++) {
		cout << q + 1 << "|";
		int length = times[q] / maxTime * 100;
		for (int i = 0; i < length; i++)
		{
			cout << ">";
		}
		cout << " " << times[q] << " ns\n";
	}
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"