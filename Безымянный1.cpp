#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
/**********************************/
/*
    NOTE
    Этота проект работате, но при больших кол-вах строк он начинает немного неправильно выводить дамп

*/
/**********************************/
void dump(const char * path, int offset, int lenBlock); // главная дамп-функция
int dumpBlock(FILE * file, int offset, int lenBlock);	// вывод строки в виде дампа 
void adressFirstSymbol(int num);						// вычисление и вывод адреса первого символа строки
char * initBlockArr(FILE * file,  int lenBlock);		// инициализация массива символами строки ффайла
char * cat(char * str1, char* str2);					// сложение двух массивов


int main()
{

	dump("input.txt", 20, 55);
	
}
void dump(const char * path, int offset, int lenBlock) {
	FILE * file = fopen(path, "rb");
	if (file == NULL) {
		printf("%s", "Error");
	}
	else {
		int l = 0;					// кол-во выведенных байт в строке
		int countByteLine = 16;		// макс. кол-во байт в строке
		fseek(file, offset, SEEK_CUR);
		int i = 0;					// кол-во выведенных байт

		/* Пока кол-во выведенных байт меньше длинные блока выводим байты символов и символы */
		while (i < lenBlock) {
			if (lenBlock < countByteLine) {					// если размер блока меньше макс. кол-ва байт в строке, то изменяем максимум
				countByteLine = lenBlock;
			}
			l = dumpBlock(file, offset, countByteLine);		// выводим строу дампа символов и символов
			offset += l;									// передвигаем указатель на позицию равную кол-ву выведенных байт
			fgetc(file);
			i += l;
			if (feof(file) != 0) {
				break;
			}
			printf("%c", '\n');
		}
	}
	fclose(file);
	_getch();
}


int dumpBlock(FILE * file, int offset, int countByteLine) {
	char * arrBlock = initBlockArr(file, countByteLine);	// создаём массив символов строки
	int sizeArrBlock = _msize(arrBlock) / sizeof(char);		// вычисляем размер массива
	adressFirstSymbol(offset);								// вычисление и вывод адреса первого элемента строки
	
	printf("%c", ':');

	int i = 0;
	for (; i < sizeArrBlock-1; i++) {
		// если байт состоит из одного символа, то сначала выводим 0
		if ((int)arrBlock[i]>=0 && (int)arrBlock[i] <= 0 || arrBlock[i] == '\r' || arrBlock[i] == '\n') {
			printf("%c", '0');
		}
		printf("%X", arrBlock[i]);

		if ((i+1) % 8 == 0) { // если число выведенных символов кратно 8, то выводим двоеточие, иначе пробел
			printf("%c", ':');
		}
		else {
			printf("%c", ' ');
		}
	}
	printf("%X", arrBlock[i++]);		// в конце выводим символ без пробела

	while (i < 16) {					// если вывелось меньше 16 байт, то вместо них выводим пробелы
		printf(" %c", ' ');
		printf("%c", ' ');
		i++;
	};
	printf("%c", ':');

	/* Выводим символы строки */
	for (int i = 0; i < sizeArrBlock; i++) {
		if (arrBlock[i] == '\n' || arrBlock[i] == '\r') {
			printf("%c", '.');
		}
		else {
			printf("%c", arrBlock[i]);
		}
			
	}
	free(arrBlock);			// удаляем массив для следующей строки
	return sizeArrBlock;	// возвращаем кол-во выведенных байт
}
char * cat(char * str1, char* str2) {
	int j = 0, i = 0;
	int len2 = strlen(str2);	// длина массива с числом в 16 сс

	while (i < 8 - len2) i++;	// доходим до позиции, с котрой начинается второй массив, если отсчитывать справа

	/* Делаем замену символов строки str1 символами строки str2 */
	for (int k = i; str2[j] != '\0'; k++) {
		if (str2[j] >= 'a' && str2[j] <= 'z') { // если массив с 16 сс содержит маленькие буквы, то переводим их в большие
			str2[j] = (int)str2[j] - 32;
		}
		str1[k] = str2[j++];
	}
	return str1; // возвращем изменённую строку с число в 16 сс
}
char * initBlockArr(FILE * file, int lenBlock) {
	int i = 0; // размер массива
	char temp;
	
	/* Считаем сколько символов в строке и делаем проверку, чтобы знать не меньше ли кол-во символов 16 */
	while (i < lenBlock) {
		fgetc(file);
		i++;
		if (feof(file)) {
            printf("%s - %i", "SIZE", i);
			break;
		}
	}
	fseek(file, -i, SEEK_CUR); // возвращаем указатель после подсчёта в исходную позицию 
	char * arrBlock = (char*)malloc(sizeof(char) * i);

	fread(arrBlock, sizeof(char), i, file);
	return arrBlock;
}
void adressFirstSymbol(int num) {
	char firstByteNum[9] = { '0', '0', '0', '0', '0', '0', '0', '0', '\0' };
	char firstByteNum2[9];										// массив для хранения числа в 16 сс
	itoa(num, firstByteNum2, 16);							   // перевод числа в 16 сс
	char * newFirstByteNum2 = cat(firstByteNum, firstByteNum2); // приведение числа в 16 сс в адрес с 8 битами

	/* Выввод адреса */
	for (int i = 0; i < 8; i++) {
		printf("%c", newFirstByteNum2[i]);
	}
}
