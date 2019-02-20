#ifndef SUDOKU_H_INCLUDED
#define SUDOKU_H_INCLUDED

#include "clause.h"

#define LOC(i,j,x) ((i)*81+(j)*9+(x))

typedef struct Sudoku {
    int sdk[9][9];
    struct Sudoku *next;
}Sudoku;

Sudoku* inputSudoku();//输入数独
Sudoku* createSudoku();//生成数独
CNF* transformToCNF();//转化成CNF格式
Sudoku *transformToSudoku(CNF *cnf);//CNF转化成数独
boolean checkSudoku();//是否合乎规则
boolean judgeNumber();//填入数字是否正确
boolean randSudoku();//随机生成数独的递归函数
boolean solveSudoku();//解答数独
boolean DFSSudoku();//DFS求解数独
status PrintSudoku();//打印数独

#endif // SUDOKU_H_INCLUDED
