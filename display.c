#include "display.h"

#define DEBUG 0

void CNFPage()
{
    int dpll=0;
    int i,t;
    int op=1;
    int solvertype;
    int file_len;
    char filepath[256]="\0";
    char savepath[256];
    CNF *cnf=NULL;

    status firstflag=FALSE;
    long Handle;
    struct _finddata_t FileInfo;

    while(op){
        system("cls");
        printf("\n\n");
        printf("---------------����SAT��CNF����˵�-------------\n");
        printf("-----------------------CNF----------------------\n");
        printf("��ǰ�����ļ�: %s\n",filepath);
        if(cnf){
            printf("��Ԫ��: %4d\n",cnf->literals);
            printf("�Ӿ���: %4d\n",cnf->clauses);
            printf("------------------------------------------------\n");
        }
        printf("1. ����CNF�ļ�\n");
        printf("2. ���CNF\n");
        printf("3. ��ӡCNF\n");
        printf("4. ��ӡѧϰ�Ӿ�\n");
        printf("5. ����\n");
        printf("0. ����\n");
        printf("------------------------------------------------\n");
        printf("��ѡ����Ĳ���[0~5]: ");
        scanf("%d",&op);
        switch(op)
        {
        case 1:
            if(cnf){
                destroyCNF(cnf);
                cnf=NULL;
            }

            if(firstflag==TRUE){
                char ch;
                do{
                    printf("�Ƿ�ѡ����һ��cnf�ļ�? (Y/N)  ");
                    scanf("%*c%c", &ch);
                }while(ch!='Y' && ch!='y' && ch!='N' && ch!='n');
                if(ch=='N' || ch=='n'){
                    firstflag = FALSE;
                    filepath[0] = '\0';
                }
            }
            if(firstflag==FALSE){
                printf("������CNF�ļ�(��)·��: ");
                scanf("%s",filepath);
                file_len = strlen(filepath);

                if(strcmp(filepath+file_len-4, ".cnf")==0){
                    if(!(cnf=loadCnfData(filepath))){
                        printf("�ļ���ʧ�ܣ�\n");
                    }
                    else{
                        printf("�ļ����سɹ�!\n");
                    }
                }
                else if((Handle=_findfirst(strcat(filepath, "*.cnf"), &FileInfo))==-1L){
                    printf("û���ҵ�ƥ����ļ���\n");
                    filepath[0] = '\0';
                }
                else{
                    firstflag=TRUE;
                }
            }
            else {
                if(_findnext(Handle, &FileInfo)!=0){
                    printf("û���ҵ�ƥ����ļ���\n");
                    _findclose(Handle);
                    firstflag = FALSE;
                    filepath[0] = '\0';
                }
            }

            if(firstflag==TRUE){
                filepath[file_len]='\0';
                printf("���ڼ���: %s\n",strcat(filepath, FileInfo.name));
                if(!(cnf=loadCnfData(filepath))){
                    printf("�ļ���ʧ�ܣ�\n");
                }
                else{
                    printf("�ļ����سɹ�!\n");
                }
            }
            getchar();
            getchar();
            break;
        case 2:
            if(!cnf){
                printf("CNF�����ڣ�\n");
            }
            else{
                t = -1;
                clearCnfAnswer(cnf);
                do{
                    printf("�����������(1.�Ż��������  2.�Ż�ǰ�����): ");
                    scanf("%d", &solvertype);
                }while(solvertype != 1 && solvertype != 2);
                if(solvertype==1){
                    printf("\n����ʹ��\"�Ż��������\"���...\n");
                    createLIndex(cnf);
                    t=clock();
                    dpll=DPLLRec(cnf,0);
                }
                else{
                    printf("\n����ʹ��\"�Ż�ǰ�����\"���...\n");
                    t=clock();
                    dpll=DPLLRec2(cnf,0);
                    if(dpll==NOTSURE){
                        dpll=FALSE;
                    }
                    else if(dpll==FALSE){
                        dpll=NOTSURE;
                    }
                }
                t=clock()-t;
                if(dpll==TRUE){
                    printf("��CNF�ɹ����!\n");
                    printf("��ʱ: %dms\n",t);
                    printf("��������:\n");
                    for(i=1;i<=cnf->literals;i++){
                        printf("%d, ",cnf->boolarray[i]*i);
                        if(i%10==0){
                            printf("\n");
                        }
                    }
                    printf("\n");
                    toSavePath(savepath,filepath);
                    if(saveRes(savepath,dpll,cnf,t)){
                        printf("RES����ɹ�!\n");
                    }
                    else{
                        printf("RES����ʧ��!\n");
                    }
                }
                else{
                    printf("���ʧ��!\n");
                    printf("��ʱ: %dms\n",t);
                    toSavePath(savepath,filepath);
                    if(saveRes(savepath,dpll,cnf,t)){
                        printf("RES����ɹ�!\n");
                    }
                    else{
                        printf("RES����ʧ��!\n");
                    }
                }
            }
            getchar();
            getchar();
            break;
        case 3:
            if(!cnf){
                printf("CNF�����ڣ�\n");
            }
            else{
                printf("���ڴ�ӡCNF...\n");
                printClause(cnf);
                printf("��ӡ��ɣ�\n");
            }
            getchar();
            getchar();
            break;
        case 4:
            if(!cnf){
                printf("CNF�����ڣ�\n");
            }
            else{
                printf("���ڴ�ӡCNF...\n");
                printLearnClause(cnf);
                printf("��ӡ��ɣ�\n");
            }
            getchar();
            getchar();
            break;
        case 5:
            if(!cnf){
                printf("CNF�����ڣ�\n");
            }
            else if(dpll==0){
                printf("CNF������!\n");
            }
            else{
                checkRes(cnf);
                printf("��ӡ��ɣ�\n");
            }
            getchar();
            getchar();
            break;
        case 0:
            break;
        }
    }
    if(cnf){
        destroyCNF(cnf);
        cnf=NULL;
    }
}
void sudokuPage()
{
    int t;
    int dpll;
    int op=1;
    int solvertype;
    int difficult = 0;
    char path[256];
    CNF *cnf=NULL;
    Sudoku *s=NULL;
    while(op){
        system("cls");
        printf("\n\n");
        printf("-------------����SAT��CNF����˵�---------------\n\n");
        printf("---------------------����-----------------------\n");
        printf("1. ����������\n");
        printf("2. ����������\n");
        printf("0. ����\n");
        printf("------------------------------------------------\n");
        printf("��ѡ����Ĳ���[0~2]: ");
        scanf("%d",&op);
        switch(op)
        {
        case 1:
            printf("�������������:\n");
            s=inputSudoku();
            if(!checkSudoku(s)){
                printf("�����������\n");
            }
            else{
                printf("�������...\n");
                cnf=transformToCNF(s);
                if(cnf){
                    printf("ת��CNF�ɹ�!\n");
                    printf("��Ԫ����%4d\n", cnf->literals);
                    printf("�Ӿ���: %4d\n", cnf->clauses);
                    printf("------------------------------------------------\n");
                    printf("\n����ʹ��\"�Ż��������\"���...\n");
                    createLIndex(cnf);
                    t=clock();
                    dpll=DPLLRec(cnf,0);
                    if(dpll==TRUE){
                        t=clock()-t;
                        printf("��CNF�ɹ����!\n");
                        Sudoku *ss=NULL;
                        ss=transformToSudoku(cnf);
                        PrintSudoku(ss);
                        free(ss);
                        printf("��ʱ: %dms\n",t);
                        while(1){
                            int op;
                            printf("------------------------------------------------\n");
                            printf("1. ����CNF�����(��׺��Ϊcnf): ");
                            printf("0. ����\n");
                            printf("��ѡ����Ĳ���[0~1]: ");
                            scanf("%d",&op);
                            if(op==1){
                                printf("�����뱣��·��:\n");
                                scanf("%s",path);
                                if(saveCnf(cnf,path)){
                                    printf("CNF����ɹ�!\n");
                                    toSavePath(path,path);
                                    if(saveRes(path,dpll,cnf,t)){
                                        printf("res����ɹ�!\n");
                                        break;
                                    }
                                    else{
                                        printf("res����ʧ��!\n");
                                    }
                                }
                                else{
                                    printf("CNF����ʧ��!\n");
                                }
                            }
                            else if(op==0){
                                break;
                            }
                        }
                    }
                }
                else{
                    printf("ת��CNFʧ��!\n");
                }
            }
            getchar();
            getchar();
            break;
        case 2:
            printf("�����������Ѷ�[0~25]��");
            scanf("%d",&difficult);
            if(!(s=createSudoku(difficult))){
                printf("��������ʧ�ܣ�\n");
            }
            else{
                printf("�������ɳɹ�!\n");
                PrintSudoku(s);
                printf("------------------------------------------------\n");
                do{
                    printf("�����������(1.�Ż��������  2.�Ż�ǰ�����): ");
                    scanf("%d", &solvertype);
                }while(solvertype != 1 && solvertype != 2);
                cnf=transformToCNF(s);
                if(cnf){
                    printf("ת��CNF�ɹ�!\n");
                    printf("��Ԫ����%4d\n", cnf->literals);
                    printf("�Ӿ���: %4d\n", cnf->clauses);
                    printf("------------------------------------------------\n");
                    t=clock();
                    if(solvertype==1){
                        printf("\n����ʹ��\"�Ż��������\"���...\n");
                        createLIndex(cnf);
                        dpll=DPLLRec(cnf,0);
                    }
                    else{
                        printf("\n����ʹ��\"�Ż�ǰ�����\"���...\n");
                        dpll=DPLLRec2(cnf,0);
                    }
                    if(dpll==TRUE){
                        t=clock()-t;
                        printf("��CNF�ɹ����!\n");
                        Sudoku *ss=NULL;
                        ss=transformToSudoku(cnf);
                        PrintSudoku(ss);
                        free(ss);
                        printf("��ʱ: %dms\n",t);
                        while(1){
                            int op;
                            printf("------------------------------------------------\n");
                            printf("1. ����CNF�����\n");
                            printf("0. ����\n");
                            printf("��ѡ����Ĳ���[0~1]: ");
                            scanf("%d",&op);
                            if(op==1){
                                printf("�����뱣��·��(��׺��Ϊcnf): ");
                                scanf("%s",path);
                                if(saveCnf(cnf,path)){
                                    printf("CNF����ɹ�!\n");
                                    toSavePath(path,path);
                                    if(saveRes(path,dpll,cnf,t)){
                                        printf("res����ɹ�!\n");
                                        break;
                                    }
                                    else{
                                        printf("res����ʧ��!\n");
                                    }
                                }
                                else{
                                    printf("CNF����ʧ��!\n");
                                }
                            }
                            else if(op==0){
                                break;
                            }
                        }
                    }
                    else{
                        printf("���ʧ�ܣ�\n");
                    }
                }
                else{
                    printf("ת��CNFʧ��!\n");
                }
            }
            getchar();
            getchar();
            break;
        }
        if(cnf){
            destroyCNF(cnf);
            cnf=NULL;
        }
        if(s){
            free(s);
            s=NULL;
        }
    }
}
void helpPage()
{
    system("cls");
    printf("\n\n");
    printf("---------------����SAT��CNF����˵�-------------\n");
    printf("-----------------------help---------------------\n");
    printf("help comment\n");
    printf("------------------------------------------------\n");
    printf("���س�������...");
}
