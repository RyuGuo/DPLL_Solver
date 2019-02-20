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
    int literals;//��Ԫ����
    int clauses;//�Ӿ����
    int *floorarray;//��Ԫ���ڵľ��߲�
    boolean *boolarray;//��Ԫ���飬��ű�Ԫ��٣�0Ϊ�٣�1Ϊ�棬2Ϊδ֪
    int *countarray;
    struct ClauseList *root;//ָ���һ���Ӿ�
    struct LearnClauseList *learn_root;//ָ���һ��ѧϰ�Ӿ�
}CNF;

typedef struct ClauseList {
    int length;//�Ӿ��������Ԫ����Ŀ
    struct LiteralNode *head;//ָ���һ����Ԫ
    struct LiteralNode *rmv;//ָ���һ�����Ƴ��ı�Ԫ
    struct ClauseList *prev;//ָ����һ���Ӿ�
    struct ClauseList *next;//ָ����һ���Ӿ�
}ClauseList;

typedef struct LearnClauseList {
    boolean isInStack;//�Ƿ��ڻ���ջ��
    int floor;//���߼���
    int countNum;//���ô���
    struct ClauseList *clause;//ָ��ѧϰ�Ӿ�
    struct LearnClauseList *next;//ָ����һ��ѧϰ�Ӿ�
}LearnClauseList;

typedef struct LiteralNode {
    int literal;//��Ԫ����Ϊ������Ϊ��
    struct LiteralNode *next;//ָ����һ����Ԫ
}LiteralNode;

status createCNF();//CNF����
status destroyCNF();//CNF����
status addClause();//�����Ӿ�
status insertClauseTop(CNF *cnf, ClauseList *Cp);//�����Ӿ��ڶ���
ClauseList* removeClause();//�Ƴ��Ӿ�
status deleteClause();//ɾ���Ӿ�
status deleteAllClause();//ɾ�������Ӿ�
status addLiteral();//���ӱ�Ԫ
LiteralNode* removeLiteral();//�Ƴ���Ԫ
status backLiteral();//�ָ���Ԫ
status deleteLiteral();//ɾ����Ԫ
status deleteLearnClause();//ɾ��ѧϰ�Ӿ�

status mergeClause();//�ϲ��Ӿ�
status sortLiterals();//�Ӿ��������
status boolarrayAssign();//���浽����
status printClause();//��ӡ�Ӿ�
status printfLearnClause();//��ӡѧϰ�Ӿ�

CNF* copyCNF();//����CNF,���ص�һ��ͷ�ڵ�ָ��
status pasteCNF();//ճ��CNF

boolean isClauseEmpty();//�Ӿ��п�
boolean isUnitClause();//�Ƿ�Ϊ���Ӿ�
boolean evaluateClause();//�Ӿ�����
boolean isHaveEmptyClause();//�Ƿ��п��Ӿ�
boolean isHaveClause();//�Ӿ伯�Ƿ�Ϊ��

ClauseList* locateUnitClause();//�Ƿ��е��Ӿ�

#endif // CLAUSE_H_INCLUDED