#include "display.h"
#include <io.h>
#include <string.h>

#define DEBUG 1

char path1[]="C:\\Users\\hasee\\Desktop\\��������ۺϿγ��������ָ��ѧ����\\SAT���Ա�ѡ����\\��������\\M\\bart17.shuffled-231.cnf";


void CNFPage()
{
    int dpll=0;
    int i,t;
    int op=1;
    char filepath[256]="\0";
    char savepath[256];
    CNF *cnf=NULL;
    #if DEBUG
    int firstflag=0;
    long Handle;
    struct _finddata_t FileInfo;
    if((Handle=_findfirst("C:\\Users\\hasee\\Desktop\\��������ۺϿγ��������ָ��ѧ����\\SAT���Ա�ѡ����\\��������\\M\\*.cnf",&FileInfo))==-1L)
        printf("û���ҵ�ƥ�����Ŀ\n");
    #endif // DEBUG
    while(op){
        system("cls");
        printf("\n\n");
        printf("---------------����SAT��CNF����˵�-------------\n");
        printf("-----------------------CNF----------------------\n");
        printf("��ǰ�����ļ�: %s\n",filepath);
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
            printf("������CNF�ļ�·��: ");
            scanf("%s",filepath);
            #if DEBUG
            if(firstflag==0){
                strcpy(filepath,"C:\\Users\\hasee\\Desktop\\��������ۺϿγ��������ָ��ѧ����\\SAT���Ա�ѡ����\\��������\\M\\");
                strcat(filepath,FileInfo.name);
                strcpy(filepath,path1);
                printf("%s\n",filepath);
            }
            else
            {
                if(_findnext(Handle,&FileInfo)==0){
                    strcpy(filepath,"C:\\Users\\hasee\\Desktop\\��������ۺϿγ��������ָ��ѧ����\\SAT���Ա�ѡ����\\��������\\M\\");
                    strcat(filepath,FileInfo.name);
                    printf("%s\n",filepath);
                }
                else{
                    _findclose(Handle);
                }
            }
            firstflag=1;
            #endif // DEBUG
            if(cnf){
                destroyCNF(cnf);
                cnf=NULL;
            }
            if(!(cnf=loadCnfData(filepath))){
                printf("�ļ���ʧ�ܣ�\n");
            }
            else{
                printf("�ļ����سɹ�!\n");
            }
            getchar();
            getchar();
            break;
        case 2:
            if(!cnf){
                printf("CNF�����ڣ�\n");
            }
            else{
                t=clock();
                dpll=DPLLRec(cnf,0);
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
                printfLearnClause(cnf);
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
                t=clock();
                cnf=transformToCNF(s);
                if(cnf){
                    printf("ת��CNF�ɹ�!\n");
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
                            printf("1. ����CNF�����(��׺��Ϊcnf)\n");
                            //printf("2. ����CNF\n");
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
                                        printf("����ɹ�!\n");
                                        op=0;
                                    }
                                    else{
                                        printf("����ʧ��!\n");
                                    }
                                }
                                else{
                                    printf("CNF����ʧ��!\n");
                                }
                            }
                            /*else if(op==2){
                                printf("�����뱣��CNF·��: \n");
                                scanf("%s",path);
                                if(saveCnf(cnf,path)){
                                    printf("CNF����ɹ�!\n");
                                }
                                else{
                                    printf("CNF����ʧ��!\n");
                                }
                            }*/
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
            if(!(s=createSudoku(20))){
                printf("��������ʧ�ܣ�\n");
            }
            else{
                printf("�������ɳɹ�!\n");
                PrintSudoku(s);
                printf("------------------------------------------------\n");
                getchar();
                getchar();
                printf("�������...\n");
                t=clock();
                cnf=transformToCNF(s);
                if(cnf){
                    printf("ת��CNF�ɹ�!\n");
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
                            printf("1. ����CNF�����\n");
                            //printf("2. ����CNF\n");
                            printf("0. ����\n");
                            printf("��ѡ����Ĳ���[0~1]: ");
                            scanf("%d",&op);
                            if(op==1){
                                printf("�����뱣��·��(��׺��Ϊcnf):\n");
                                scanf("%s",path);
                                if(saveCnf(cnf,path)){
                                    printf("CNF����ɹ�!\n");
                                    toSavePath(path,path);
                                    if(saveRes(path,dpll,cnf,t)){
                                        printf("����ɹ�!\n");
                                        op=0;
                                    }
                                    else{
                                        printf("����ʧ��!\n");
                                    }
                                }
                                else{
                                    printf("CNF����ʧ��!\n");
                                }
                            }
                            /*else if(op==2){
                                printf("�����뱣��CNF·��: \n");
                                scanf("%s",path);
                                if(saveCnf(cnf,path)){
                                    printf("CNF����ɹ�!\n");
                                }
                                else{
                                    printf("CNF����ʧ��!\n");
                                }
                            }*/
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
