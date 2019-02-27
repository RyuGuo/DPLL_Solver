#include "display.h"

int main()
{
    int op=1;
    srand(time(NULL));
    while(op){
        system("cls");
        printf("\n\n");
        printf("---------------基于SAT的CNF程序菜单-------------\n");
        printf("------------------------------------------------\n");
        printf("1. CNF文件\n");
        printf("2. 数独游戏\n");
        printf("3. 帮助\n");
        printf("0. 退出\n");
        printf("------------------------------------------------\n");
        printf("请选择你的操作[0~3]: ");
        scanf("%d",&op);
        switch(op)
        {
        case 1:
            CNFPage();
            getchar();
            getchar();
            break;
        case 2:
            sudokuPage();
            getchar();
            getchar();
            break;
        case 3:
            helpPage();
            getchar();
            getchar();
        case 0:
            break;
        }
    }
    printf("欢迎下次使用本系统！\n");
    getchar();
    getchar();
    return 0;
}
