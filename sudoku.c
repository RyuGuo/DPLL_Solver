#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"
#include "time.h"

Sudoku* inputSudoku()
{
    int i,j;
    Sudoku *s=(Sudoku*)malloc(sizeof(Sudoku));
    s->next=NULL;
    if(!s){
        return NULL;
    }
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            scanf("%d",&s->sdk[i][j]);
        }
    }
    return s;
}
Sudoku* createSudoku(int difficult)
{
    int i,j,t;
    int c,flag=0;
    int x,y;
    srand(time(NULL));
    Sudoku *s=(Sudoku*)malloc(sizeof(Sudoku));
    Sudoku *ss=NULL;
    s->next=NULL;
    if(!s){
        return NULL;
    }
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            s->sdk[i][j]=0;
        }
    }
    if(randSudoku(s,0,0)==TRUE){//随机生成完整数独
        //PrintSudoku(s);
        for(c=0;c<81-78;c++){//挖掉3个空
            x=rand()%9;
            y=rand()%9;
            s->sdk[x][y]=0;
        }
        while(1){
            solveSudoku(s,&c);//求解该数独，得到解的个数c
            if(c>1){
                s->sdk[x][y]=t;
                flag++;
                if(flag>=difficult){
                    break;
                }
                for(ss=s->next;ss;ss=s->next){
                    s->next=ss->next;
                    free(ss);
                }
            }
            else{
                do{
                    x=rand()%9;
                    y=rand()%9;
                }while(s->sdk[x][y]==0);
                t=s->sdk[x][y];
                s->sdk[x][y]=0;
            }
        }
        return s;
    }
    else{
        return NULL;
    }
}
boolean randSudoku(Sudoku *s, int x, int y)
{
    int c=0;
    s->sdk[x][y]=rand()%9+1;
    do{
        do{
            c++;
            if(c>9){
                s->sdk[x][y]=0;
                return FALSE;
            }
            s->sdk[x][y]=s->sdk[x][y]%9+1;
        }while(judgeNumber(s,x,y)!=TRUE);
        if(x==8 && y==8){
            return TRUE;
        }
    }while(((y==8)?randSudoku(s,x+1,0):randSudoku(s,x,y+1))!=TRUE);
    return TRUE;
}
CNF* transformToCNF(Sudoku *s)
{
    /*用X(i,j,x)表示第i行第j列的格子填k是否为真*/
    /*1为真, 0为假*/
    int i,j,k,l,c,m;
    CNF *cnf=(CNF*)malloc(sizeof(CNF));
    int b[9],b2[9],b3[9];
    if(!createCNF(cnf,9*9*9,0)){
        return NULL;
    }
    /*生成每个格子的情况*/
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            //如果该格子有数字
            if(s->sdk[i][j]!=0){
                //X(i,j,x)为真,其他x为假
                for(k=1;k<=9;k++){
                    if(k==s->sdk[i][j]){
                        b[0]=LOC(i,j,k);
                    }
                    else{
                        b[0]=-LOC(i,j,k);
                    }
                    addClause(cnf,1,b);
                }

                //横排空格处不相等,向右搜索
                for(k=j+1;k<9;k++){
                    if(s->sdk[i][k]==0){
                        b[0]=-LOC(i,j,s->sdk[i][j]);
                        b[1]=-LOC(i,k,s->sdk[i][j]);
                        addClause(cnf,2,b);
                    }
                }
                //纵排空格处不相等,向下搜索
                for(k=i+1;k<9;k++){
                    if(s->sdk[k][j]==0){
                        b[0]=-LOC(i,j,s->sdk[i][j]);
                        b[1]=-LOC(k,j,s->sdk[i][j]);
                        addClause(cnf,2,b);
                    }
                }
                //九宫格空格处不相等,向右下角搜索
                for(k=i+1;k<i/3*3+3;k++){
                    for(l=j/3*3;l<j/3*3+3;l++){
                        //横排与纵排已经比较过
                        if(l==j){
                            continue;
                        }
                        if(s->sdk[k][l]==0){
                            b[0]=0-LOC(i,j,s->sdk[i][j]);
                            b[1]=0-LOC(k,l,s->sdk[i][j]);
                            addClause(cnf,2,b);
                        }
                    }
                }
            }
            //如果是空格
            else{
                //空格处只能填一个数字
                for(k=1,c=0;k<=9;k++){
                    s->sdk[i][j]=k;
                    if(judgeNumber(s->sdk,i,j)){
                        b[c]=k;
                        b2[c]=LOC(i,j,k);
                        c++;
                    }
                }
                s->sdk[i][j]=0;
                addClause(cnf,c,b2);
                for(k=0;k<c-1;k++){
                    for(l=k+1;l<c;l++){
                        b2[0]=-LOC(i,j,b[k]);
                        b2[1]=-LOC(i,j,b[l]);
                        addClause(cnf,2,b2);
                    }
                }
                //横排搜索
                for(k=j+1;k<9;k++){
                    //空格处与已填不相等
                    if(s->sdk[i][k]!=0){
                        b2[0]=0-LOC(i,j,s->sdk[i][k]);
                        b2[1]=0-LOC(i,k,s->sdk[i][k]);
                        addClause(cnf,2,b2);
                    }
                    //都是空格处不能填相同
                    else{
                        for(l=0;l<c;l++){
                            b2[0]=0-LOC(i,j,b[l]);
                            b2[1]=0-LOC(i,k,b[l]);
                            addClause(cnf,2,b2);
                        }
                    }
                }
                //纵排搜索
                for(k=i+1;k<9;k++){
                    //空格处与已填不相等
                    if(s->sdk[k][j]!=0){
                        b2[0]=0-LOC(i,j,s->sdk[k][j]);
                        b2[1]=0-LOC(k,j,s->sdk[k][j]);
                        addClause(cnf,2,b2);
                    }
                    //都是空格处不能填相同
                    else{
                        for(l=0;l<c;l++){
                            b2[0]=0-LOC(i,j,b[l]);
                            b2[1]=0-LOC(k,j,b[l]);
                            addClause(cnf,2,b2);
                        }
                    }
                }
                //九宫格搜索
                for(k=i+1;k<i/3*3+3;k++){
                    for(l=j/3*3;l<j/3*3+3;l++){
                        if(l==j){
                            continue;
                        }
                        //空格处与已填不相等
                        if(s->sdk[k][l]!=0){
                            b2[0]=0-LOC(i,j,s->sdk[k][l]);
                            b2[1]=0-LOC(k,j,s->sdk[k][l]);
                            addClause(cnf,2,b2);
                        }
                        //都是空格处不能填相同
                        else{
                            for(m=0;m<c;m++){
                                b2[0]=0-LOC(i,j,b[m]);
                                b2[1]=0-LOC(k,l,b[m]);
                                addClause(cnf,2,b2);
                            }
                        }
                    }
                }
            }
        }
    }
    //每行必须填1~9
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            b[j]=0;
        }
        for(j=0,l=0;j<9;j++){
            if(s->sdk[i][j]!=0){
                b[s->sdk[i][j]-1]=1;//存放未填的数字
            }
            else{
                b2[l]=j;//存放未填的位置
                l++;
            }
        }
        for(j=0,k=0;j<9;j++){
            if(b[j]==0){
                b[k]=j+1;
                k++;
            }
        }
        for(j=0;j<k;j++){
            for(c=0;c<l;c++){
                b3[c]=LOC(i,b2[c],b[j]);
            }
            addClause(cnf,l,b3);
        }
    }
    //每列必须填1~9
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            b[j]=0;
        }
        for(j=0,l=0;j<9;j++){
            if(s->sdk[j][i]!=0){
                b[s->sdk[j][i]-1]=1;//存放未填的数字
            }
            else{
                b2[l]=i;//存放未填的位置
                l++;
            }
        }
        for(j=0,k=0;j<9;j++){
            if(b[j]==0){
                b[k]=j+1;
            }
        }
        for(j=0;j<k;j++){
            for(c=0;c<l;c++){
                b3[c]=LOC(i,b2[c],b[j]);
            }
            addClause(cnf,l,b3);
        }
    }
    //九宫格必须填1~9
    for(i=0;i<9;i+=3){
        for(m=0;m<9;m+=3){
            for(j=0;j<9;j++){
                b[j]=0;
            }
            for(j=0,l=0;j<9;j++){
                if(s->sdk[i+j/3][m+j%3]!=0){
                    b[s->sdk[i+j/3][m+j%3]-1]=1;//存放未填的数字
                }
                else{
                    b2[l]=j;//存放未填的位置
                    l++;
                }
            }
            for(j=0,k=0;j<9;j++){
                if(b[j]==0){
                    b[k]=j+1;
                }
            }
            for(j=0;j<k;j++){
                for(c=0;c<l;c++){
                    b3[c]=LOC(i,b2[c],b[j]);
                }
                addClause(cnf,l,b3);
            }
        }
    }
    //printClause(cnf);
    return cnf;
}
Sudoku *transformToSudoku(CNF *cnf)
{
    Sudoku *s=(Sudoku*)malloc(sizeof(Sudoku));
    s->next=NULL;
    int i,j,k;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            for(k=1;k<=9;k++){
                if(cnf->boolarray[LOC(i,j,k)]==TRUE){
                    s->sdk[i][j]=k;
                }
            }
        }
    }
    return s;
}
boolean checkSudoku(Sudoku *s)
{
    int i,j,k,l;
    int flag=0;
    char b[9];
    /*横向检查*/
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            b[j]=0;
        }
        for(j=0;j<9;j++){
            if(!s->sdk[i][j]){
                flag=1;
            }
            else if(!b[s->sdk[i][j]-1]){
                b[s->sdk[i][j]-1]=1;
            }
            else{
                return FALSE;
            }
        }
    }
    /*纵向检查*/
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            b[j]=0;
        }
        for(j=0;j<9;j++){
            if(!s->sdk[j][i]){
                flag=1;
            }
            else if(!b[s->sdk[j][i]-1]){
                b[s->sdk[j][i]-1]=1;
            }
            else{
                return FALSE;
            }
        }
    }
    /*九格检查*/
    for(i=0;i<9;i+=3){
        for(j=0;j<9;j+=3){
            for(k=0;k<9;k++){
                b[k]=0;
            }
            for(k=i;k<i+3;k++){
                for(l=j;l<j+3;l++){
                    if(!s->sdk[k][l]){
                        flag=1;
                    }
                    else if(!b[s->sdk[k][l]-1]){
                        b[s->sdk[k][l]-1]=1;
                    }
                    else{
                        return FALSE;
                    }
                }
            }
        }
    }
    return (flag)?NOTSURE:TRUE;
}
boolean judgeNumber(Sudoku *s, int x, int y)
{
    int i,j;

   for(i=0;i<9;i++){
       if(s->sdk[i][y]==s->sdk[x][y]&&i!=x){
        return FALSE;
       }

   }
    for(i=0;i<9;i++){
       if(s->sdk[x][i]==s->sdk[x][y]&&i!=y){
        return FALSE;
       }

   }
   for(i=x/3*3;i<x/3*3+3;i++){
       for(j=y/3*3;j<y/3*3+3;j++){
           if(s->sdk[i][j]==s->sdk[x][y]&&(i!=x||j!=y)){
            return FALSE;
           }

       }
   }
   return TRUE;
}
boolean solveSudoku(Sudoku *s, int *solvecount)
{
    *solvecount=0;
    DFSSudoku(s,0,0,solvecount);
    return (solvecount>0)?TRUE:FALSE;
}
boolean DFSSudoku(Sudoku *s, int x, int y, int *solvecount)
{
    int i,j,k;
    Sudoku *ss;
    while(s->sdk[x][y]!=0 && x<=8){
        y++;
        if(y==9){
            x+=1;
            y=0;
        }
    }
    if(x>8){
        ss=(Sudoku*)malloc(sizeof(Sudoku));
        for(j=0;j<9;j++){
            for(k=0;k<9;k++){
                ss->sdk[j][k]=s->sdk[j][k];
            }
        }
        ss->next=s->next;
        s->next=ss;
        (*solvecount)++;
        return TRUE;
    }
    for(i=1;i<=9;i++){
        s->sdk[x][y]=i;
        switch(judgeNumber(s,x,y))
        {
        case FALSE:
            break;
        case TRUE:
            if(x==8 && y==8){
                ss=(Sudoku*)malloc(sizeof(Sudoku));
                for(j=0;j<9;j++){
                    for(k=0;k<9;k++){
                        ss->sdk[j][k]=s->sdk[j][k];
                    }
                }
                ss->next=s->next;
                s->next=ss;
                s->sdk[x][y]=0;
                (*solvecount)++;
                return TRUE;
            }
            else{
                (y==8)?DFSSudoku(s,x+1,0,solvecount):DFSSudoku(s,x,y+1,solvecount);
            }
            break;
        default:
            break;
        }
    }
    s->sdk[x][y]=0;
    return FALSE;
}
status PrintSudoku(Sudoku *s)
{
    int i,j;
    printf("打印数独:\n");
    for(j=0;j<19;j++){
        printf("-");
    }
    printf("\n");
    for(i=0;i<9;i++){
        printf("|");
        for(j=0;j<9;j++){
            printf("%d%c",s->sdk[i][j],((j+1)%3==0)?'|':' ');
        }
        printf("\n");
        if((i+1)%3==0){
            printf("|");
            for(j=0;j<17;j++){
                printf("-");
            }
            printf("|\n");
        }
    }
    return OK;
}
