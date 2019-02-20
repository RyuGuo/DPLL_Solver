#include "clause.h"

status createCNF(CNF *cnf, int literals, int clauses)
{
    cnf->literals=literals;
    cnf->clauses=clauses;
    cnf->root=NULL;
    cnf->learn_root=NULL;
    cnf->boolarray = (boolean*)malloc((cnf->literals+1)*sizeof(boolean));
    cnf->floorarray = (int*)malloc((cnf->literals+1)*sizeof(int));
    cnf->countarray = (int*)malloc((cnf->literals+1)*sizeof(int));
    cnf->floorarray[0]=-1;
    if(cnf->boolarray && cnf->floorarray){
        int i;
        for(i=1;i<=cnf->literals;i++){
            cnf->boolarray[i]=NOTSURE;
            cnf->floorarray[i]=-1;
            cnf->countarray[i]=0;
        }
        return OK;
    }
    else {
        return ERROR;
    }
}
status destroyCNF(CNF *cnf)
{
    if(!cnf){
        return ERROR;
    }
    deleteAllClause(cnf);
    free(cnf->boolarray);
    free(cnf->floorarray);
    free(cnf);
    return OK;
}
status addClause(CNF *cnf, int n, int *clause)
{
    int i;
//    for(i=0;i<n;i++){
//        printf("c=%d,i=%d,j=%d,x=%d\n",clause[i],(ABS(clause[i]))/81,(ABS(clause[i]-1)/9)%9,(ABS(clause[i])-1)%9+1);
//    }
//    printf("\n");
//    getchar();
    ClauseList *newClause = (ClauseList*)malloc(sizeof(ClauseList));
    if(!newClause || n<=0){
        return ERROR;
    }
    newClause->head=NULL;
    newClause->rmv=NULL;
    newClause->length=0;
    for(i=0;i<n;i++){
        if(!addLiteral(newClause, clause[i])){
            return ERROR;
        }
    }
    if(cnf->root){
        cnf->root->prev=newClause;
    }
    newClause->next = cnf->root;
    newClause->prev = NULL;

    cnf->root = newClause;
    cnf->clauses++;
    return OK;
}
status insertClauseTop(CNF *cnf, ClauseList *Cp)
{
    if(cnf->root){
        cnf->root->prev=Cp;
    }
    Cp->next = cnf->root;
    Cp->prev = NULL;
    cnf->root = Cp;
    cnf->clauses++;
    return OK;
}
ClauseList* removeClause(CNF *cnf, ClauseList *Cp)
{
    ClauseList *p=Cp->prev;
    if(Cp->next){
        Cp->next->prev=p;
    }
    if(p){
        p->next=Cp->next;
    }
    else{
        cnf->root=Cp->next;
    }
    cnf->clauses--;
    return Cp;
}
status deleteClause(CNF *cnf, ClauseList *Cp)
{
    LiteralNode *lp;
    ClauseList *p=Cp->prev;
    if(Cp->next){
        Cp->next->prev=p;
    }
    if(p){
        p->next=Cp->next;
    }
    else{
        cnf->root=Cp->next;
    }
    for(lp=Cp->head;lp;lp=Cp->head){
        Cp->head=lp->next;
        free(lp);
    }
    for(lp=Cp->rmv;lp;lp=Cp->rmv){
        Cp->rmv=lp->next;
        free(lp);
    }
    free(Cp);
    cnf->clauses--;
    return OK;
}
status deleteAllClause(CNF *cnf)
{
    ClauseList *p;
    LearnClauseList *lcp;
    for(p=cnf->root;p;p=cnf->root){
        deleteClause(cnf, p);
    }
    for(lcp=cnf->learn_root;lcp;lcp=cnf->learn_root){
        cnf->learn_root=lcp->next;
        free(lcp);
    }
    return OK;
}
status addLiteral(ClauseList *Cp, int literal)
{
    LiteralNode *p = (LiteralNode*)malloc(sizeof(LiteralNode));
    if(!p){
        return ERROR;
    }
    p->literal = literal;
    p->next = Cp->head;
    Cp->head = p;
    Cp->length++;
    return OK;
}
status deleteLearnClause(CNF *cnf, LearnClauseList *Lcp)
{
    LearnClauseList *lcq=cnf->learn_root;
    if(Lcp==cnf->learn_root){
        cnf->learn_root=Lcp->next;
    }
    else{
        for(lcq=cnf->learn_root;lcq;lcq=lcq->next){
            if(lcq->next==Lcp){
                lcq->next=Lcp->next;
                break;
            }
        }
    }
    deleteClause(cnf,Lcp->clause);
    free(Lcp);
    return OK;
}
LiteralNode* removeLiteral(ClauseList *Cp, int literal)
{
    LiteralNode *p,*q=NULL;
    for(p=Cp->head;p;p=p->next){
        if(p->literal == literal){
            Cp->length--;
            if(!q){
                Cp->head=p->next;
            }
            else{
                q->next=p->next;
            }
            p->next=Cp->rmv;
            Cp->rmv=p;
            return p;
        }
        q=p;
    }
    return NULL;
}
status backLiteral(ClauseList *Cp, LiteralNode *Lp)
{
    LiteralNode *Lq=NULL;
    if(Cp->rmv==Lp){
        Cp->rmv=Lp->next;
    }
    else{
        for(Lq=Cp->rmv;Lq && Lq->next!=Lp;Lq=Lq->next){
            ;
        }
        if(Lq==NULL){
            return ERROR;
        }
        Lq->next=Lp->next;
    }
    Lp->next=Cp->head;
    Cp->head=Lp;
    Cp->length++;
    return OK;
}
status deleteLiteral(ClauseList *Cp, int literal)
{
    LiteralNode *p,*q=NULL;
    for(p=Cp->head;p;p=p->next){
        if(p->literal == literal){
            Cp->length--;
            if(!q){
                Cp->head=p->next;
            }
            else{
                q->next=p->next;
            }
            free(p);
            return OK;
        }
        q=p;
    }
    return ERROR;
}
status boolarrayAssign(CNF *cnf, int literal,int f)
{
    cnf->boolarray[ABS(literal)]=EVALUE(literal);//记录到数组
    cnf->floorarray[ABS(literal)]=f;//记录决策层数
    return OK;
}
status printClause(CNF *cnf)
{
    printf("------------------------------------------------\n");
    printf("literals: %d\n",cnf->literals);
    printf("clauses: %d\n", cnf->clauses);
    printf("boolarray: 0x%p\n", cnf->boolarray);
    int i;
    for(i=1;i<=cnf->literals;i++){
        printf("%d:%d, ",i,cnf->boolarray[i]);
        if(i%10==0){
            printf("\n");
        }
    }
    printf("floorarray: 0x%p\n", cnf->floorarray);
    for(i=1;i<=cnf->literals;i++){
        printf("%d:%d, ",i,cnf->floorarray[i]);
        if(i%10==0){
            printf("\n");
        }
    }
    ClauseList *Cp;
    LiteralNode *Lp;
    printf("\n");

    printf("clause:\n");
    for(Cp=cnf->root,i=1;Cp;Cp=Cp->next,i++){
        printf("%d:%p length: %d prev:%p next:%p\n",i, Cp,Cp->length,Cp->prev,Cp->next);
        for(Lp=Cp->head;Lp;Lp=Lp->next){
            printf("%d ",Lp->literal);
        }
        if(Cp->rmv){
            printf("|| ");
            for(Lp=Cp->rmv;Lp;Lp=Lp->next){
                printf("%d ",Lp->literal);
            }
        }

        printf("\n");
    }
    return OK;
}
status printfLearnClause(CNF *cnf)
{
    printf("------------------------------------------------\n");
    printf("learn clause:\n");
    int i;
    LearnClauseList *lcp;
    ClauseList *Cp;
    LiteralNode *Lp;
    for(lcp=cnf->learn_root,i=1;lcp;lcp=lcp->next,i++){
        Cp=lcp->clause;
        printf("%d:%p length: %d isInStack:%s 决策级数:%d\n",i, Cp,Cp->length,(lcp->isInStack==TRUE)?"TRUE":"FALSE",lcp->floor);
        for(Lp=Cp->head;Lp;Lp=Lp->next){
            printf("%d ",Lp->literal);
        }
        if(Cp->rmv){
            printf("|| ");
            for(Lp=Cp->rmv;Lp;Lp=Lp->next){
                printf("%d ",Lp->literal);
            }
        }
        printf("\n");
    }
    return OK;
}
CNF* copyCNF(CNF *cnf)
{
    int i;
    ClauseList *Cp,*Cpp;
    LiteralNode *Lp,*Lpp;
    CNF *newCNF=(CNF*)malloc(sizeof(CNF));
    if(!newCNF){
        return NULL;
    }
    newCNF->clauses=cnf->clauses;
    newCNF->literals=cnf->literals;
    newCNF->boolarray=(boolean*)malloc(cnf->literals*sizeof(boolean));
    newCNF->floorarray=(int*)malloc(cnf->literals*sizeof(boolean));
    newCNF->root=NULL;
    newCNF->learn_root=NULL;
    if(!newCNF){
        return NULL;
    }
    for(i=1;i<=newCNF->literals;i++){
        newCNF->boolarray[i]=cnf->boolarray[i];
        newCNF->floorarray[i]=cnf->floorarray[i];
    }
    for(Cp=cnf->root;Cp;Cp=Cp->next){
        Cpp=(ClauseList*)malloc(sizeof(ClauseList));
        if(!Cpp){
            return NULL;
        }
        Cpp->length=Cp->length;
        Cpp->head=NULL;
        Cpp->rmv=NULL;
        for(Lp=Cp->head;Lp;Lp=Lp->next){
            Lpp=(LiteralNode*)malloc(sizeof(LiteralNode));
            if(!Lpp){
                return NULL;
            }
            Lpp->literal=Lp->literal;
            Lpp->next=Cpp->head;
            Cpp->head=Lpp;
        }
        for(Lp=Cp->rmv;Lp;Lp=Lp->next){
            Lpp=(LiteralNode*)malloc(sizeof(LiteralNode));
            if(!Lpp){
                return NULL;
            }
            Lpp->literal=Lp->literal;
            Lpp->next=Cpp->head;
            Cpp->rmv=Lpp;
        }
        Cpp->prev=NULL;
        Cpp->next=newCNF->root;
        if(Cpp->next){
            Cpp->next->prev=Cpp;
        }
        newCNF->root=Cpp;
    }
    return newCNF;
}
status pasteCNF(CNF *oldCNF, CNF *newCNF)
{
    deleteAllClause(oldCNF);
    oldCNF->literals=newCNF->literals;
    oldCNF->clauses=newCNF->clauses;
    oldCNF->root=newCNF->root;
    oldCNF->learn_root=newCNF->learn_root;
    return OK;
}
boolean isClauseEmpty(CNF *cnf)
{
    return (cnf->clauses)?FALSE:TRUE;
}
boolean isUnitClause(ClauseList *Cp)
{
    return (Cp->length==1)?TRUE:FALSE;
}
ClauseList* locateUnitClause(CNF *cnf)
{
    ClauseList *p;
    for(p=cnf->root;p;p=p->next){
        if(isUnitClause(p)==TRUE){
            return p;
        }
    }
    return NULL;
}
boolean isHaveEmptyClause(CNF *cnf)
{
    ClauseList *p;
    for(p=cnf->root;p;p=p->next){
        if(!(p->length)){
            return TRUE;
        }
    }
    return FALSE;
}
boolean isHaveClause(CNF *cnf)
{
    return (cnf->clauses)?TRUE:FALSE;
}
boolean evaluateClause(ClauseList *Cp, int literal)
{
    int flag=NOTCONTAIN;
    LiteralNode *p;
    for(p=Cp->head;p;p=p->next){
        if(literal==p->literal){
            return TRUE;
        }
        else if(-literal==p->literal){
            /* 如果遇到-49 49 0 的情况就会有问题*/
            //return NOTSURE;
            flag=NOTSURE;
        }
    }
    return flag;
}
