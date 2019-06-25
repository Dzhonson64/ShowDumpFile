#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
/**********************************/
/*
    NOTE
    ����� ������ ��������, �� ��� ������� ���-��� ����� �� �������� ������� ����������� �������� ����

*/
/**********************************/
void dump(const char * path, int offset, int lenBlock); // ������� ����-�������
int dumpBlock(FILE * file, int offset, int lenBlock);	// ����� ������ � ���� ����� 
void adressFirstSymbol(int num);						// ���������� � ����� ������ ������� ������� ������
char * initBlockArr(FILE * file,  int lenBlock);		// ������������� ������� ��������� ������ ������
char * cat(char * str1, char* str2);					// �������� ���� ��������


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
		int l = 0;					// ���-�� ���������� ���� � ������
		int countByteLine = 16;		// ����. ���-�� ���� � ������
		fseek(file, offset, SEEK_CUR);
		int i = 0;					// ���-�� ���������� ����

		/* ���� ���-�� ���������� ���� ������ ������� ����� ������� ����� �������� � ������� */
		while (i < lenBlock) {
			if (lenBlock < countByteLine) {					// ���� ������ ����� ������ ����. ���-�� ���� � ������, �� �������� ��������
				countByteLine = lenBlock;
			}
			l = dumpBlock(file, offset, countByteLine);		// ������� ����� ����� �������� � ��������
			offset += l;									// ����������� ��������� �� ������� ������ ���-�� ���������� ����
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
	char * arrBlock = initBlockArr(file, countByteLine);	// ������ ������ �������� ������
	int sizeArrBlock = _msize(arrBlock) / sizeof(char);		// ��������� ������ �������
	adressFirstSymbol(offset);								// ���������� � ����� ������ ������� �������� ������
	
	printf("%c", ':');

	int i = 0;
	for (; i < sizeArrBlock-1; i++) {
		// ���� ���� ������� �� ������ �������, �� ������� ������� 0
		if ((int)arrBlock[i]>=0 && (int)arrBlock[i] <= 0 || arrBlock[i] == '\r' || arrBlock[i] == '\n') {
			printf("%c", '0');
		}
		printf("%X", arrBlock[i]);

		if ((i+1) % 8 == 0) { // ���� ����� ���������� �������� ������ 8, �� ������� ���������, ����� ������
			printf("%c", ':');
		}
		else {
			printf("%c", ' ');
		}
	}
	printf("%X", arrBlock[i++]);		// � ����� ������� ������ ��� �������

	while (i < 16) {					// ���� �������� ������ 16 ����, �� ������ ��� ������� �������
		printf(" %c", ' ');
		printf("%c", ' ');
		i++;
	};
	printf("%c", ':');

	/* ������� ������� ������ */
	for (int i = 0; i < sizeArrBlock; i++) {
		if (arrBlock[i] == '\n' || arrBlock[i] == '\r') {
			printf("%c", '.');
		}
		else {
			printf("%c", arrBlock[i]);
		}
			
	}
	free(arrBlock);			// ������� ������ ��� ��������� ������
	return sizeArrBlock;	// ���������� ���-�� ���������� ����
}
char * cat(char * str1, char* str2) {
	int j = 0, i = 0;
	int len2 = strlen(str2);	// ����� ������� � ������ � 16 ��

	while (i < 8 - len2) i++;	// ������� �� �������, � ������ ���������� ������ ������, ���� ����������� ������

	/* ������ ������ �������� ������ str1 ��������� ������ str2 */
	for (int k = i; str2[j] != '\0'; k++) {
		if (str2[j] >= 'a' && str2[j] <= 'z') { // ���� ������ � 16 �� �������� ��������� �����, �� ��������� �� � �������
			str2[j] = (int)str2[j] - 32;
		}
		str1[k] = str2[j++];
	}
	return str1; // ��������� ��������� ������ � ����� � 16 ��
}
char * initBlockArr(FILE * file, int lenBlock) {
	int i = 0; // ������ �������
	char temp;
	
	/* ������� ������� �������� � ������ � ������ ��������, ����� ����� �� ������ �� ���-�� �������� 16 */
	while (i < lenBlock) {
		fgetc(file);
		i++;
		if (feof(file)) {
            printf("%s - %i", "SIZE", i);
			break;
		}
	}
	fseek(file, -i, SEEK_CUR); // ���������� ��������� ����� �������� � �������� ������� 
	char * arrBlock = (char*)malloc(sizeof(char) * i);

	fread(arrBlock, sizeof(char), i, file);
	return arrBlock;
}
void adressFirstSymbol(int num) {
	char firstByteNum[9] = { '0', '0', '0', '0', '0', '0', '0', '0', '\0' };
	char firstByteNum2[9];										// ������ ��� �������� ����� � 16 ��
	itoa(num, firstByteNum2, 16);							   // ������� ����� � 16 ��
	char * newFirstByteNum2 = cat(firstByteNum, firstByteNum2); // ���������� ����� � 16 �� � ����� � 8 ������

	/* ������ ������ */
	for (int i = 0; i < 8; i++) {
		printf("%c", newFirstByteNum2[i]);
	}
}
