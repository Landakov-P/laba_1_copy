#include "Lab1.h"


const char* msgs[] = { "0. Выйти", "1. Добавить не нулевой элемент (в случае существования элемента с такими индексами, значение перезаписывается)", "2. Вывести исходную и результирующую матрицы" };

const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

const int SZ = 4;

namespace Lab1 {

	// Функция ввода целого числа 
	int getInt(int& a) {
		int rc = 0;
		cin >> a;
		if (!cin.good())
			rc = 1;				// Обнаружена ошибка ввода
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return rc;
	}


	// Выбор номера альтернативы
	int dialog(const char* msgs[], int N) {
		const char* errmsgs = "";
		int rc, i;
		do {
			puts(errmsgs);
			errmsgs = "Ошибка! Повторите ввод:!";
			for (i = 0; i < N; ++i)					// Вывод списка альтернатив
				puts(msgs[i]);
			printf("Выберите: --> ");
		} while (getInt(rc) < 0 || rc >= N);
		return rc;
	}


	// Функция ввода кол-ва строк и столбцов
	void Input(SparseMatrix& matrix) {
		const char* em = "Неверное число (Число должно быть натуральным). Попробуйте еще раз:\n--> ";
		cout << "Введите кол-во строк: --> ";
		while (getInt(matrix.m) || (matrix.m < 1))	// Обнаружена ошибка ввода или конец файла
			cout << em;
		cout << "Введите кол-во столбцов: --> ";
		while (getInt(matrix.n) || (matrix.n < 1))	// Обнаружена ошибка ввода или конец файла
			cout << em;
	}


	//Функция добавления новой строки-списка в матрицу по возрастанию
	void AddNewRow(SparseMatrix& matrix, int i, int j, int val) {
		SparseMatrixRowList* Rows = matrix.Rowlist;		// Указатель на список строк
		/* 1 случай: список пуст */
		if (!matrix.Rowlist) {
			matrix.Rowlist = new SparseMatrixRowList();
			matrix.Rowlist->i = i;
			matrix.Rowlist->next = nullptr;
			matrix.Rowlist->row = nullptr;
			AddToRow(matrix.Rowlist->row, j, val);
		}
		else {
			/* Перемещаем указатель на тот элемент, который будет стоять после нового*/
			while (Rows && (i > Rows->i))
				Rows = Rows->next;
			/* 2 случай: Такая строка уже существует. Тогда добавляем элемент в данный список */
			if (Rows && (i == Rows->i))
				AddToRow(Rows->row, j, val);
			else {
				SparseMatrixRowList* NewElem = new SparseMatrixRowList();
				NewElem->i = i;
				NewElem->row = nullptr;
				SparseMatrixRowList* RowsPrev = matrix.Rowlist;
				while ((RowsPrev->next != Rows) && (RowsPrev != Rows))
					RowsPrev = RowsPrev->next;
				/* 3 случай: элемент добавляется в начало */
				if (RowsPrev == Rows) {
					matrix.Rowlist = NewElem;
					NewElem->next = Rows;
					AddToRow(matrix.Rowlist->row, j, val);
				}
				/* 4 случай: обычное добавление */
				else {
					NewElem->next = Rows;
					RowsPrev->next = NewElem;
					AddToRow(NewElem->row, j, val);
				}
			}
		}
	}



	// Функция добавления нового не нулевого элемента в строку-список по возрастанию
	void AddToRow(RowOfSparseMatrix*& row, int j, int val) {
		/* Проверка существования ненулевого элемента с такими индексами */
		RowOfSparseMatrix* tmp = row;
		while (tmp) {
			if (tmp->j == j) {
				tmp->val = val;
				return;
			}
			tmp = tmp->next;
		}
		tmp = row;
		/* 1 случай: список пуст */
		if (!tmp) {
			row = new RowOfSparseMatrix();
			row->j = j;
			row->val = val;
			row->next = nullptr;
		}
		else {
			/* Перемещаем указатель на тот элемент, который будет стоять после нового*/
			while (tmp && (j > tmp->j))
				tmp = tmp->next;
			/* 2 случай: если элемент с такими индексами уже есть */
			if (tmp && (j == tmp->j))
				tmp->val = val;
			else {
				RowOfSparseMatrix* NewElem = new RowOfSparseMatrix();
				NewElem->j = j;
				NewElem->val = val;
				NewElem->next = nullptr;
				RowOfSparseMatrix* tmpPrev = row;
				while ((tmpPrev->next != tmp) && (tmpPrev != tmp))
					tmpPrev = tmpPrev->next;
				/* 3 случай: элемент добавляется в начало */
				if (tmpPrev == tmp) {
					row = NewElem;
					NewElem->next = tmp;
				}
				/* 4 случай: обычное добавление */
				else {
					NewElem->next = tmp;
					tmpPrev->next = NewElem;
				}
			}
		}
	}



	// Функция добавления нового ненулевого элемента в матрицу
	int D_Add(SparseMatrix& matrix) {
		int i, j, val;
		const char* em = "Неверный индекс (Число должно быть натуральным и не выходящим за границы массива). Попробуйте еще раз:\n --> ";
		cout << "Введите индекс строки: --> ";
		while ((getInt(i) < 0) || (i >= matrix.m))
			cout << em;
		cout << "Введите индекс столбца: --> ";
		while ((getInt(j) < 0) || (j >= matrix.n))
			cout << em;
		cout << "Введите значение элемента: --> ";
		while ((getInt(val) < 0) || (!val))
			cout << "Ошибка ввода (Число должно быть целым и отличным от нуля). Попробуйте еще раз:\n --> ";
		AddNewRow(matrix, i, j, val);
		return 1;
	}

	void print(SparseMatrix& matrix) {
		SparseMatrixRowList* Row = matrix.Rowlist;
		/* Для каждой строки */
		for (int i = 0; i < matrix.m; ++i) {
			if (Row && Row->i == i) {
				RowOfSparseMatrix* now = Row->row;
				for (int j = 0; j < matrix.n; ++j) {
					if (now && now->j == j) {
						cout << setw(4) << right << now->val;
						now = now->next;
					}
					else
						cout << "   0";
				}
				cout << endl;
				Row = Row->next;
			}
			else {
				for (int j = 0; j < matrix.n; ++j) {

					cout << "   0";
				}
				cout << endl;
			}
		}
	}

	// Функция вывода матрицы и создания вектора
	int D_Show(SparseMatrix& matrix) {
		print(matrix);
		SparseMatrix* ResultMatrix = CreateNewMatrix(matrix);
		std::cout << "\n\n";
		print(*ResultMatrix);
		return 1;
	}

	void findMin(RowOfSparseMatrix* row, int& minInd, int& min) {
		min = row->val;
		while (row) {
			if (row->val < min) {
				min = row->val;
				minInd = row->j;
			}
			row = row->next;
		}
	}


	SparseMatrix* CreateNewMatrix(SparseMatrix& matrix) {
		SparseMatrix* ResultMatrix = new SparseMatrix;		// Результирующая разряженная матрица
		ResultMatrix->m = matrix.m;
		ResultMatrix->n = matrix.n;
		ResultMatrix->Rowlist = nullptr;
		SparseMatrixRowList* rows = matrix.Rowlist;
		while (rows) {
			int count = 0;
			SparseMatrixRowList* tmp = rows;
			while (tmp) {
				tmp = tmp->next;
				++count;
			}
			RowOfSparseMatrix* now = rows->row;
			int minInd = 0, min;
			if (count == matrix.n) {
				min = 0;
			}
			else {
				min = now->val;
			}
			findMin(now, minInd, min);			// Индекс самого минимального элемента (и одновременно "сдвиг" всех элементов строки влево)
			while (now) {
				if ((now->j - minInd) >= 0)
					AddNewRow(*ResultMatrix, rows->i, now->j - minInd, now->val);
				now = now->next;
			}
			rows = rows->next;
		}
		return ResultMatrix;
	}


	// Функция удаления 
	void Delete(SparseMatrix& matrix) {
		SparseMatrixRowList* rows = matrix.Rowlist;
		while (rows) {
			matrix.Rowlist = matrix.Rowlist->next;
			RowOfSparseMatrix* tmp = rows->row;
			while (tmp) {
				rows->row = rows->row->next;
				delete tmp;
				tmp = rows->row;
			}
			rows = matrix.Rowlist;
		}
	}
}