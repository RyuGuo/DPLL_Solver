#ifndef DPLL_H_INCLUDED
#define DPLL_H_INCLUDED

#include "clause.h"

#define LEARNLENGTH_MAX 50

#define CLAUSE 0
#define LITERAL 1
#define SPLIT 2

typedef struct ChangeStack{
    int tag;
    int floor;
    LiteralNode *Lp;
    ClauseList *Cp;//
    struct ChangeStack *next;
}ChangeStack;

boolean DPLLRec();//递归
boolean DPLLRec2();//递归2
boolean DPLLUnRec();//非递归

status simplySingleClause();//化简单子句
status simplifyClause();//化简子句
int chooseStrategy_MaxOccurrence();//策略_出现次数最多
int chooseStrategy_MaxOccurrenceTwoLiteral();//选取出现在只有2个变元的子句中次数最多的变元
int VSIDSStrategy(CNF *cnf);
int combineStrategy();//综合策略
status saveChange();//保存化简时的修改
status reduceChange();//恢复修改
status checkRes();//检查答案
status PrintChangeStack();//打印栈
int createLearnClause(CNF *cnf, int *a, int i);//生成学习子句
status backLearnClause(CNF *cnf, int floor);//学习子句的回溯
status deleteRepeatLearnClause();//删去重复的学习子句
status backAssign(CNF *cnf, int floor);////赋值回溯

#endif // DPLL_H_INCLUDED
