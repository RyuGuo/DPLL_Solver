#ifndef DPLL_H_INCLUDED
#define DPLL_H_INCLUDED

#include "clause.h"

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

boolean DPLLRec();//�ݹ�
boolean DPLLRec2();//�ݹ�2
boolean DPLLUnRec();//�ǵݹ�

status simplySingleClause();//�����Ӿ�
status simplifyClause();//�����Ӿ�
int chooseStrategy_MaxOccurrence();//����_���ִ������
int chooseStrategy_MaxOccurrenceTwoLiteral();//ѡȡ������ֻ��2����Ԫ���Ӿ��д������ı�Ԫ
int VSIDSStrategy(CNF *cnf);
int combineStrategy();//�ۺϲ���
status saveChange();//���滯��ʱ���޸�
status reduceChange();//�ָ��޸�
status checkRes();//����
status PrintChangeStack();//��ӡջ
int createLearnClause(CNF *cnf, int *a, int i, int X);//����ѧϰ�Ӿ�
status backLearnClause(CNF *cnf, int floor);//ѧϰ�Ӿ�Ļ���
status deleteRepeatLearnClause();//ɾȥ�ظ���ѧϰ�Ӿ�
status backAssign(CNF *cnf, int floor);////��ֵ����

#endif // DPLL_H_INCLUDED
