#ifndef CNFFILE_H_INCLUDED
#define CNFFILE_H_INCLUDED

#include "clause.h"

#define LITERALMAX 200

typedef char status;

CNF* loadCnfData();//��ȡ����
status toSavePath(char *savepath, char *cnfpath);//�任��׺��res
status saveRes(char *respath, status DPLL, CNF *cnf, int time);//������
status saveCnf();//��������

#endif // CNFFILE_H_INCLUDED
