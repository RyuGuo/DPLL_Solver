#include "display.h"

int main()
{
    int op=1;
    srand(time(NULL));
    while(op){
        system("cls");
        printf("\n\n");
        printf("---------------����SAT��CNF����˵�-------------\n");
        printf("------------------------------------------------\n");
        printf("1. CNF�ļ�\n");
        printf("2. ������Ϸ\n");
        printf("3. ����\n");
        printf("0. �˳�\n");
        printf("------------------------------------------------\n");
        printf("��ѡ����Ĳ���[0~3]: ");
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
    printf("��ӭ�´�ʹ�ñ�ϵͳ��\n");
    getchar();
    getchar();
    return 0;
}
