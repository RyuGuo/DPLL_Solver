#include "display.h"
#include <io.h>
#include <string.h>

#define DEBUG 1

char path1[]="C:\\Users\\hasee\\Desktop\\程序设计综合课程设计任务及指导学生包\\SAT测试备选算例\\满足算例\\M\\bart17.shuffled-231.cnf";


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
    if((Handle=_findfirst("C:\\Users\\hasee\\Desktop\\程序设计综合课程设计任务及指导学生包\\SAT测试备选算例\\满足算例\\M\\*.cnf",&FileInfo))==-1L)
        printf("没有找到匹配的项目\n");
    #endif // DEBUG
    while(op){
        system("cls");
        printf("\n\n");
        printf("---------------基于SAT的CNF程序菜单-------------\n");
        printf("-----------------------CNF----------------------\n");
        printf("当前加载文件: %s\n",filepath);
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
            printf("请输入CNF文件路径: ");
            scanf("%s",filepath);
            #if DEBUG
            if(firstflag==0){
                strcpy(filepath,"C:\\Users\\hasee\\Desktop\\程序设计综合课程设计任务及指导学生包\\SAT测试备选算例\\满足算例\\M\\");
                strcat(filepath,FileInfo.name);
                strcpy(filepath,path1);
                printf("%s\n",filepath);
            }
            else
            {
                if(_findnext(Handle,&FileInfo)==0){
                    strcpy(filepath,"C:\\Users\\hasee\\Desktop\\程序设计综合课程设计任务及指导学生包\\SAT测试备选算例\\满足算例\\M\\");
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
                printf("文件打开失败！\n");
            }
            else{
                printf("文件加载成功!\n");
            }
            getchar();
            getchar();
            break;
        case 2:
            if(!cnf){
                printf("CNF不存在！\n");
            }
            else{
                t=clock();
                dpll=DPLLRec(cnf,0);
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
                printfLearnClause(cnf);
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
                t=clock();
                cnf=transformToCNF(s);
                if(cnf){
                    printf("转化CNF成功!\n");
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
                            printf("1. 保存CNF及结果(后缀名为cnf)\n");
                            //printf("2. 保存CNF\n");
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
                                        printf("保存成功!\n");
                                        op=0;
                                    }
                                    else{
                                        printf("保存失败!\n");
                                    }
                                }
                                else{
                                    printf("CNF保存失败!\n");
                                }
                            }
                            /*else if(op==2){
                                printf("请输入保存CNF路径: \n");
                                scanf("%s",path);
                                if(saveCnf(cnf,path)){
                                    printf("CNF保存成功!\n");
                                }
                                else{
                                    printf("CNF保存失败!\n");
                                }
                            }*/
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
            if(!(s=createSudoku(20))){
                printf("数独生成失败！\n");
            }
            else{
                printf("数独生成成功!\n");
                PrintSudoku(s);
                printf("------------------------------------------------\n");
                getchar();
                getchar();
                printf("正在求解...\n");
                t=clock();
                cnf=transformToCNF(s);
                if(cnf){
                    printf("转化CNF成功!\n");
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
                            printf("1. 保存CNF及结果\n");
                            //printf("2. 保存CNF\n");
                            printf("0. 回退\n");
                            printf("请选择你的操作[0~1]: ");
                            scanf("%d",&op);
                            if(op==1){
                                printf("请输入保存路径(后缀名为cnf):\n");
                                scanf("%s",path);
                                if(saveCnf(cnf,path)){
                                    printf("CNF保存成功!\n");
                                    toSavePath(path,path);
                                    if(saveRes(path,dpll,cnf,t)){
                                        printf("保存成功!\n");
                                        op=0;
                                    }
                                    else{
                                        printf("保存失败!\n");
                                    }
                                }
                                else{
                                    printf("CNF保存失败!\n");
                                }
                            }
                            /*else if(op==2){
                                printf("请输入保存CNF路径: \n");
                                scanf("%s",path);
                                if(saveCnf(cnf,path)){
                                    printf("CNF保存成功!\n");
                                }
                                else{
                                    printf("CNF保存失败!\n");
                                }
                            }*/
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
