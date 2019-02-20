#ifndef CLAUSE_H_INCLUDED
#define CLAUSE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define OK 1
#define ERROR 0

#define TRUE 1
#define FALSE -1
#define NOTSURE 0
#define NOTCONTAIN 2

#define EVALUE(x) ((x>0)?1:-1)
#define ABS(x) ((x>0)?(x):0-(x))

typedef char boolean;
typedef char status;

typedef struct CNF {
    int literals;//变元个数
    int clauses;//子句个数
    int *floorarray;//变元所在的决策层
    boolean *boolarray;//变元数组，存放变元真假，0为假，1为真，2为未知
    int *countarray;
    struct ClauseList *root;//指向第一个子句
    struct LearnClauseList *learn_root;//指向第一个学习子句
}CNF;

typedef struct ClauseList {
    int length;//子句包含变量元的数目
    struct LiteralNode *head;//指向第一个变元
    struct LiteralNode *rmv;//指向第一个被移除的变元
    struct ClauseList *prev;//指向上一个子句
    struct ClauseList *next;//指向下一个子句
}ClauseList;

typedef struct LearnClauseList {
    boolean isInStack;//是否在回溯栈中
    int floor;//决策级数
    int countNum;//调用次数
    struct ClauseList *clause;//指向学习子句
    struct LearnClauseList *next;//指向下一个学习子句
}LearnClauseList;

typedef struct LiteralNode {
    int literal;//变元，正为正，负为非
    struct LiteralNode *next;//指向下一个变元
}LiteralNode;

status createCNF();//CNF创建
status destroyCNF();//CNF销毁
status addClause();//增加子句
status insertClauseTop(CNF *cnf, ClauseList *Cp);//插入子句在顶部
ClauseList* removeClause();//移除子句
status deleteClause();//删除子句
status deleteAllClause();//删除所有子句
status addLiteral();//增加变元
LiteralNode* removeLiteral();//移除变元
status backLiteral();//恢复变元
status deleteLiteral();//删除变元
status deleteLearnClause();//删除学习子句

status mergeClause();//合并子句
status sortLiterals();//子句变量排序
status boolarrayAssign();//保存到数组
status printClause();//打印子句
status printfLearnClause();//打印学习子句

CNF* copyCNF();//复制CNF,返回第一个头节点指针
status pasteCNF();//粘贴CNF

boolean isClauseEmpty();//子句判空
boolean isUnitClause();//是否为单子句
boolean evaluateClause();//子句判真
boolean isHaveEmptyClause();//是否有空子句
boolean isHaveClause();//子句集是否为空

ClauseList* locateUnitClause();//是否有单子句

#endif // CLAUSE_H_INCLUDED
