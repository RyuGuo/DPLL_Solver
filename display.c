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
        printf("---------------基于SAT的CNF程序菜单-------------\n");
        printf("-----------------------CNF----------------------\n");
        printf("当前加载文件: %s\n",filepath);
        if(cnf){
            printf("变元数: %4d\n",cnf->literals);
            printf("子句数: %4d\n",cnf->clauses);
            printf("------------------------------------------------\n");
        }
        printf("1. 加载CNF文件\n");
        printf("2. 求解CNF\n");
        printf("3. 打印CNF\n");
        printf("4. 打印学习子句\n");
        printf("5. 检查答案\n");
        printf("0. 回退\n");
        printf("------------------------------------------------\n");
        printf("请选择你的操作[0~5]: ");
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
                    printf("是否选择下一个cnf文件? (Y/N)  ");
                    scanf("%*c%c", &ch);
                }while(ch!='Y' && ch!='y' && ch!='N' && ch!='n');
                if(ch=='N' || ch=='n'){
                    firstflag = FALSE;
                    filepath[0] = '\0';
                }
            }
            if(firstflag==FALSE){
                printf("请输入CNF文件(夹)路径: ");
                scanf("%s",filepath);
                file_len = strlen(filepath);

                if(strcmp(filepath+file_len-4, ".cnf")==0){
                    if(!(cnf=loadCnfData(filepath))){
                        printf("文件打开失败！\n");
                    }
                    else{
                        printf("文件加载成功!\n");
                    }
                }
                else if((Handle=_findfirst(strcat(filepath, "*.cnf"), &FileInfo))==-1L){
                    printf("没有找到匹配的文件！\n");
                    filepath[0] = '\0';
                }
                else{
                    firstflag=TRUE;
                }
            }
            else {
                if(_findnext(Handle, &FileInfo)!=0){
                    printf("没有找到匹配的文件！\n");
                    _findclose(Handle);
                    firstflag = FALSE;
                    filepath[0] = '\0';
                }
            }

            if(firstflag==TRUE){
                filepath[file_len]='\0';
                printf("正在加载: %s\n",strcat(filepath, FileInfo.name));
                if(!(cnf=loadCnfData(filepath))){
                    printf("文件打开失败！\n");
                }
                else{
                    printf("文件加载成功!\n");
                }
            }
            getchar();
            getchar();
            break;
        case 2:
            if(!cnf){
                printf("CNF不存在！\n");
            }
            else{
                t = -1;
                clearCnfAnswer(cnf);
                do{
                    printf("请输入求解器(1.优化后求解器  2.优化前求解器): ");
                    scanf("%d", &solvertype);
                }while(solvertype != 1 && solvertype != 2);
                if(solvertype==1){
                    printf("\n正在使用\"优化后求解器\"求解...\n");
                    createLIndex(cnf);
                    t=clock();
                    dpll=DPLLRec(cnf,0);
                }
                else{
                    printf("\n正在使用\"优化前求解器\"求解...\n");
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
                    printf("该CNF成功解出!\n");
                    printf("用时: %dms\n",t);
                    printf("求解答案如下:\n");
                    for(i=1;i<=cnf->literals;i++){
                        printf("%d, ",cnf->boolarray[i]*i);
                        if(i%10==0){
                            printf("\n");
                        }
                    }
                    printf("\n");
                    toSavePath(savepath,filepath);
                    if(saveRes(savepath,dpll,cnf,t)){
                        printf("RES保存成功!\n");
                    }
                    else{
                        printf("RES保存失败!\n");
                    }
                }
                else{
                    printf("求解失败!\n");
                    printf("用时: %dms\n",t);
                    toSavePath(savepath,filepath);
                    if(saveRes(savepath,dpll,cnf,t)){
                        printf("RES保存成功!\n");
                    }
                    else{
                        printf("RES保存失败!\n");
                    }
                }
            }
            getchar();
            getchar();
            break;
        case 3:
            if(!cnf){
                printf("CNF不存在！\n");
            }
            else{
                printf("正在打印CNF...\n");
                printClause(cnf);
                printf("打印完成！\n");
            }
            getchar();
            getchar();
            break;
        case 4:
            if(!cnf){
                printf("CNF不存在！\n");
            }
            else{
                printf("正在打印CNF...\n");
                printLearnClause(cnf);
                printf("打印完成！\n");
            }
            getchar();
            getchar();
            break;
        case 5:
            if(!cnf){
                printf("CNF不存在！\n");
            }
            else if(dpll==0){
                printf("CNF求解错误!\n");
            }
            else{
                checkRes(cnf);
                printf("打印完成！\n");
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
        printf("-------------基于SAT的CNF程序菜单---------------\n\n");
        printf("---------------------数独-----------------------\n");
        printf("1. 求解你的数独\n");
        printf("2. 求解随机数独\n");
        printf("0. 回退\n");
        printf("------------------------------------------------\n");
        printf("请选择你的操作[0~2]: ");
        scanf("%d",&op);
        switch(op)
        {
        case 1:
            printf("请输入你的数独:\n");
            s=inputSudoku();
            if(!checkSudoku(s)){
                printf("数独输入错误！\n");
            }
            else{
                printf("正在求解...\n");
                cnf=transformToCNF(s);
                if(cnf){
                    printf("转化CNF成功!\n");
                    printf("变元数：%4d\n", cnf->literals);
                    printf("子句数: %4d\n", cnf->clauses);
                    printf("------------------------------------------------\n");
                    printf("\n正在使用\"优化后求解器\"求解...\n");
                    createLIndex(cnf);
                    t=clock();
                    dpll=DPLLRec(cnf,0);
                    if(dpll==TRUE){
                        t=clock()-t;
                        printf("该CNF成功解出!\n");
                        Sudoku *ss=NULL;
                        ss=transformToSudoku(cnf);
                        PrintSudoku(ss);
                        free(ss);
                        printf("用时: %dms\n",t);
                        while(1){
                            int op;
                            printf("------------------------------------------------\n");
                            printf("1. 保存CNF及结果(后缀名为cnf): ");
                            printf("0. 回退\n");
                            printf("请选择你的操作[0~1]: ");
                            scanf("%d",&op);
                            if(op==1){
                                printf("请输入保存路径:\n");
                                scanf("%s",path);
                                if(saveCnf(cnf,path)){
                                    printf("CNF保存成功!\n");
                                    toSavePath(path,path);
                                    if(saveRes(path,dpll,cnf,t)){
                                        printf("res保存成功!\n");
                                        break;
                                    }
                                    else{
                                        printf("res保存失败!\n");
                                    }
                                }
                                else{
                                    printf("CNF保存失败!\n");
                                }
                            }
                            else if(op==0){
                                break;
                            }
                        }
                    }
                }
                else{
                    printf("转化CNF失败!\n");
                }
            }
            getchar();
            getchar();
            break;
        case 2:
            printf("请输入数独难度[0~25]：");
            scanf("%d",&difficult);
            if(!(s=createSudoku(difficult))){
                printf("数独生成失败！\n");
            }
            else{
                printf("数独生成成功!\n");
                PrintSudoku(s);
                printf("------------------------------------------------\n");
                do{
                    printf("请输入求解器(1.优化后求解器  2.优化前求解器): ");
                    scanf("%d", &solvertype);
                }while(solvertype != 1 && solvertype != 2);
                cnf=transformToCNF(s);
                if(cnf){
                    printf("转化CNF成功!\n");
                    printf("变元数：%4d\n", cnf->literals);
                    printf("子句数: %4d\n", cnf->clauses);
                    printf("------------------------------------------------\n");
                    t=clock();
                    if(solvertype==1){
                        printf("\n正在使用\"优化后求解器\"求解...\n");
                        createLIndex(cnf);
                        dpll=DPLLRec(cnf,0);
                    }
                    else{
                        printf("\n正在使用\"优化前求解器\"求解...\n");
                        dpll=DPLLRec2(cnf,0);
                    }
                    if(dpll==TRUE){
                        t=clock()-t;
                        printf("该CNF成功解出!\n");
                        Sudoku *ss=NULL;
                        ss=transformToSudoku(cnf);
                        PrintSudoku(ss);
                        free(ss);
                        printf("用时: %dms\n",t);
                        while(1){
                            int op;
                            printf("------------------------------------------------\n");
                            printf("1. 保存CNF及结果\n");
                            printf("0. 回退\n");
                            printf("请选择你的操作[0~1]: ");
                            scanf("%d",&op);
                            if(op==1){
                                printf("请输入保存路径(后缀名为cnf): ");
                                scanf("%s",path);
                                if(saveCnf(cnf,path)){
                                    printf("CNF保存成功!\n");
                                    toSavePath(path,path);
                                    if(saveRes(path,dpll,cnf,t)){
                                        printf("res保存成功!\n");
                                        break;
                                    }
                                    else{
                                        printf("res保存失败!\n");
                                    }
                                }
                                else{
                                    printf("CNF保存失败!\n");
                                }
                            }
                            else if(op==0){
                                break;
                            }
                        }
                    }
                    else{
                        printf("求解失败！\n");
                    }
                }
                else{
                    printf("转化CNF失败!\n");
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
    printf("---------------基于SAT的CNF程序菜单-------------\n");
    printf("-----------------------help---------------------\n");
    printf("help comment\n");
    printf("------------------------------------------------\n");
    printf("按回车键继续...");
}
