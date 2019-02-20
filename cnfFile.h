#ifndef CNFFILE_H_INCLUDED
#define CNFFILE_H_INCLUDED

#include "clause.h"

#define LITERALMAX 200

typedef char status;

CNF* loadCnfData();//读取数据
status toSavePath(char *savepath, char *cnfpath);//变换后缀名res
status saveRes(char *respath, status DPLL, CNF *cnf, int time);//保存结果
status saveCnf();//保存数据

#endif // CNFFILE_H_INCLUDED
