#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

#include "clause.h"

#define LOC(i,j,x) ((i)*81+(j)*9+(x))

typedef struct Sudoku {
    int sdk[9][9];
    struct Sudoku *next;
} Sudoku;

Sudoku* inputSudoku();//��������
Sudoku* createSudoku();//��������
CNF* transformToCNF();//ת����CNF��ʽ
Sudoku *transformToSudoku(CNF *cnf);//CNFת��������
boolean checkSudoku();//�Ƿ�Ϻ�����
boolean judgeNumber();//���������Ƿ���ȷ
boolean randSudoku();//������������ĵݹ麯��
boolean solveSudoku();//�������
boolean DFSSudoku();//DFS�������
status PrintSudoku(Sudoku *s, status isColor);//��ӡ����,�Ƿ����ɫ����һ������ɫ��ǣ��ڶ����ڿ�λ������ɫ��

#endif // SUDOKU_H_INCLUDED
