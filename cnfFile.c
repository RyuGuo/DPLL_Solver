#include "cnfFile.h"

CNF* loadCnfData(char *path)
{
    int i,j;
    char ch;
    int l[200];
    int literals,clauses;
    FILE *fp;
    fp=fopen(path,"r");
    if(!fp){
        return NULL;
    }
    CNF *cnf=(CNF*)malloc(sizeof(CNF));
    if(!cnf){
        return NULL;
    }
    while((ch=fgetc(fp))=='c'){
        while((ch=fgetc(fp))!='\n'){
            ;
        }
    }
    for(i=0;i<5;i++){
        ch=fgetc(fp);
    }
    fscanf(fp,"%d",&literals);
    fscanf(fp,"%d",&clauses);
    if(!createCNF(cnf,literals,0)){
        fclose(fp);
        return ERROR;
    }
    for(i=0;i<clauses;i++){
        j=0;
        do{
            fscanf(fp,"%d",&l[j]);
            j++;
        }while(l[j-1]!=0);
        if(!addClause(cnf,j-1,l)){
            fclose(fp);
            return NULL;
        }
    }
    fclose(fp);
    return cnf;
}
status toSavePath(char *respath, char *cnfpath)
{
    int i=-1;
    do{
        i++;
        respath[i]=cnfpath[i];
    }while(cnfpath[i]!='\0');
    i-=3;
    respath[i++]='r';
    respath[i++]='e';
    respath[i]='s';
    return OK;
}
status saveRes(char *respath, status DPLL, CNF *cnf, int time)
{
    int i;
    FILE *fp=fopen(respath,"w");
    if(!fp){
        return ERROR;
    }
    if(DPLL==TRUE){
        for(i=1;i<=cnf->literals;i++){
            if(cnf->boolarray[i]==0){
                DPLL=-1;
                break;
            }
        }
    }
    fprintf(fp,"s %d\n",DPLL);
    fprintf(fp,"v");
    for(i=1;i<=cnf->literals;i++){
        fprintf(fp," %d",cnf->boolarray[i]*i);
    }
    fprintf(fp,"\nt %d",time);
    fclose(fp);
    return OK;
}
status saveCnf(CNF* cnf, char *path)
{
    FILE *fp=fopen(path,"w");
    ClauseList *Cp;
    LiteralNode *Lp;
    if(!fp){
        return ERROR;
    }
    fprintf(fp,"p cnf %d %d\n",cnf->literals,cnf->clauses);
    for(Cp=cnf->root;Cp;Cp=Cp->next){
        for(Lp=Cp->head;Lp;Lp=Lp->next){
            fprintf(fp,"%d ",Lp->literal);
        }
        fprintf(fp,"0\n");
    }
    fclose(fp);
    return OK;
}
