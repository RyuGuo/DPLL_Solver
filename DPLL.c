#include "DPLL.h"
#include <time.h>

#define DEBUG2 0

#define VSIDS 2
#define VSIDSCOUNT 100
#define LEARNLENGTH_MAX 20
#define OUTTIME 120000

int startTime;
status reset=FALSE;
int MAXC = 10;

enum strategy {mom, vsids, first, occurency, weightsat};

boolean DPLLRec(CNF *cnf,const int f)
{
    /* S为公式对应的子句集。若其满足，返回TURE；否则返回FALSE. */
    int s,l;
    int count=0;
    int ret=f;//回溯层数
    ClauseList *Cp;
    ChangeStack st;
    st.floor=f;
    st.next=NULL;
    //VSIDS对每个文字设立一个计数器。当文字在某个子句中出现，包括原始问题中的子句和学习子句，该文字对应的计数器就加1
    if(f==0) {
        startTime = clock();
        LiteralNode *Lp;
        for(Cp=cnf->root; Cp; Cp=Cp->next) {
            for(Lp=Cp->head; Lp; Lp=Lp->next) {
                if(Lp->literal>0) {
                    cnf->countarray[Lp->literal]++;
                } else {
                    cnf->countarray[0-Lp->literal+cnf->literals]+=1;
                }
            }
        }
    }
    do {
        count++;
        if(reset==TRUE) {
            reset=FALSE;
            /*如果重启动，可以把那些没有移除变元的、很长的学习子句删除*/
            LearnClauseList *lcp=cnf->learn_root,*lcq=NULL;
            while(lcp) {
                if(lcp->clause->rmv==NULL && lcp->clause->length>LEARNLENGTH_MAX) {
                    lcq=lcp->next;
                    deleteClauseLIndex(cnf, lcp->clause);
                    deleteLearnClause(cnf,lcp);
                    lcp=lcq;
                } else {
                    lcp=lcp->next;
                }
            }
        }
        while((Cp=locateUnitClause(cnf))!=NULL) {//单子句传播
            boolarrayAssign(cnf,Cp->head->literal,f);//记录到数组
            simplifySingleClause_2(cnf, Cp, &st);//化简单子句并保存修改
            if(isHaveEmptyClause(cnf)==TRUE) { //如果有空子句
                if(clock()-startTime>=OUTTIME) {
                    printf("超时 >%dms\n",OUTTIME);
                    return -1-f;
                }
                int *a=(int*)malloc(sizeof(int)*(cnf->literals+3));
                reduceChange(cnf, &st,-1, a);//还原到初始集
                /*如果取值不成立，可能要添加学习子句并非时序回溯...*/
                ret=a[0];
                free(a);
                backLearnClause(cnf,f);
                //赋值回溯
                backAssign(cnf,f);
                if(count>MAXC) {
                    MAXC++;
                    //printf("重启动...\n");
                    reset=TRUE;
                    //学习子句的回溯
                    return 0-f;
                }
                return ret-f;
            } else if(isHaveClause(cnf)==FALSE) { //如果没有子句
                reduceChange(cnf, &st,-1, NULL);//还原到初始集
                //学习子句的回溯
                backLearnClause(cnf, f);
                return TRUE;
            }
        }//while
        /* VSIDS */
        l=combineStrategy(cnf,mom);//综合策略选出变元

        addClause(cnf,1,&l);
        saveChange(&st,SPLIT,cnf->root,NULL);//保存插入操作

        //printf("递归:%d, 选%d\n",f,l);

        s=DPLLRec(cnf,f+1);
        reduceChange(cnf,&st,1,NULL);
    } while(s==FALSE);
    if(s==TRUE) {
        reduceChange(cnf,&st,-1,NULL);
        //学习子句的回溯
        backLearnClause(cnf,f);
        return TRUE;
    }
    //按返回值多层回溯
    else {
        reduceChange(cnf,&st,-1,NULL);
        //学习子句的回溯
        backLearnClause(cnf,f);
        //赋值回溯
        backAssign(cnf,f);
        return s+1;
    }
}
boolean DPLLRec2(CNF *cnf, const int f)
{
    /* S为公式对应的子句集。若其满足，返回TURE；否则返回FALSE. */
    int l;
    ClauseList *Cp;
    ChangeStack st;
    st.next=NULL;
    if(f==0) {
        startTime=clock();
    }
    while((Cp=locateUnitClause(cnf))!=NULL) {//单子句传播
        boolarrayAssign(cnf,Cp->head->literal,f);//记录到数组
        simplySingleClause(cnf, Cp->head->literal, &st);//化简单子句
        if(isHaveEmptyClause(cnf)==TRUE) { //如果有空子句
            reduceChange(cnf,&st,-1,NULL);
            backAssign(cnf,f);
            if(clock()-startTime>=OUTTIME) {
                printf("超时 >%dms\n",OUTTIME);
                return NOTSURE;
            }
            return FALSE;
        } else if(isHaveClause(cnf)==FALSE) { //如果没有子句
            reduceChange(cnf,&st,-1,NULL);
            return TRUE;
        }
    }//while
    l=combineStrategy(cnf,mom);//综合策略选出变元
    boolarrayAssign(cnf,l,f);//记录到数组
    addClause(cnf,1,&l);//添加到子句集中
    saveChange(&st,SPLIT,cnf->root,NULL);
    int s=DPLLRec2(cnf, f+1);//递归
    if(s==TRUE) { //如果是真
        reduceChange(cnf,&st,-1,NULL);
        return TRUE;
    }
    if(s==NOTSURE) {
        reduceChange(cnf,&st,-1,NULL);
        backAssign(cnf,f);
        return NOTSURE;
    }
    reduceChange(cnf,&st,1,NULL);
    l=-l;//取假值
    boolarrayAssign(cnf,l,f);//记录到数组
    addClause(cnf,1,&l);//添加到子句集中
    saveChange(&st,SPLIT,cnf->root,NULL);
    s=DPLLRec2(cnf, f+1);//递归
    reduceChange(cnf,&st,-1,NULL);
    if(s==FALSE) {
        backAssign(cnf,f);
    }
    return s;
}
boolean DPLLUnRec();//非递归

status simplySingleClause(CNF *cnf, int literal, ChangeStack *head)
{
    LiteralNode *Lp;
    ClauseList *Cp=cnf->root;
    ClauseList *Cq;
    while(Cp) {
        switch(evaluateClause(Cp,literal)) {
        case TRUE:
            removeClause(cnf, Cp);
            if(head) {
                saveChange(head,CLAUSE,Cp,NULL);
            }
            Cp=Cp->next;
            //printClause(cnf);
            break;
        case NOTSURE:
            Lp=removeLiteral(Cp,-literal);
            if(head) {
                saveChange(head,LITERAL,Cp,Lp);
            }
            //如果为空语句,把空语句置顶返回
            //如果为单语句,置顶
            if(Cp->length<=1) {
                Cq=Cp->next;
                removeClause(cnf,Cp);
                insertClauseTop(cnf,Cp);
                if(Cp->length==0) {
                    return ERROR;
                } else {
                    Cp=Cq;
                }
            } else {
                Cp=Cp->next;
            }
            //printClause(cnf);
            break;
        default:
            Cp=Cp->next;
            break;
        }
    }
    return OK;
}
status simplifySingleClause_2(CNF *cnf, ClauseList *Cp, ChangeStack *head)
{
    int literal = Cp->head->literal;
    int index = ABS(literal);

    LiteralIndex *li;
    ClauseList *CCp;
    LiteralNode *Lp;
    //由于插入的单子句没有加入索引，必须另外删除
    removeClause(cnf, Cp);
    saveChange(head,CLAUSE,Cp,NULL);
    for(li=cnf->lindex[index].next; li; li=li->next) {
        CCp=li->Cp;
        Lp=li->Lp;
        if(CCp->isremoved==FALSE) {
            if(Lp->literal == literal) {
                removeClause(cnf, CCp);
                if(head) {
                    saveChange(head,CLAUSE,CCp,NULL);
                }
            } else if(Lp->literal == -literal) {
                Lp = removeLiteral(CCp,-literal);
                if(head) {
                    saveChange(head,LITERAL,CCp,Lp);
                }
                //如果为空语句,把空语句置顶返回
                //如果为单语句,置顶
                if(CCp->length<=1) {
                    removeClause(cnf,CCp);
                    insertClauseTop(cnf,CCp);
                    if(Cp->length==0) {
                        return ERROR;
                    }
                }
            }
        }
    }
    return OK;
}
int chooseStrategy_MaxOccurrence(CNF *cnf)
{
    int i;
    int max=0;
    int index=0;
    ClauseList *Cp;
    LiteralNode *Lp;
    int *visitedplus=(int*)malloc((cnf->literals+1)*sizeof(int));
    int *visitedminus=(int*)malloc((cnf->literals+1)*sizeof(int));
    for(i=1; i<=cnf->literals; i++) {
        visitedplus[i]=visitedminus[i]=0;
    }
    for(Cp=cnf->root; Cp; Cp=Cp->next) {
        for(Lp=Cp->head; Lp; Lp=Lp->next) {
            if(Lp->literal>0) {
                visitedplus[Lp->literal]++;
            } else {
                visitedminus[0-Lp->literal]++;
            }
        }
    }
    for(i=1; i<=cnf->literals; i++) {
        if(max<visitedplus[i]+visitedminus[i]) {
            max=visitedplus[i]+visitedminus[i];
            index = i;
        }
    }
    index=(visitedplus[index]<visitedminus[index])?-index:index;
    free(visitedplus);
    free(visitedminus);
    return index;
}
int MOMStrategy(CNF *cnf)
{
    int i;
    double max=-1;
    int index=0;
    ClauseList *Cp;
    LiteralNode *Lp;
    double *J=(double*)malloc(sizeof(double)*(cnf->literals+1));
    for(i=1; i<=cnf->literals; i++) {
        J[i]=0;
    }
    for(Cp=cnf->root; Cp; Cp=Cp->next) {
        for(Lp=Cp->head; Lp; Lp=Lp->next) {
            J[ABS(Lp->literal)]+=1.0/(1<<(Cp->length));
        }
    }
    for(i=1; i<=cnf->literals; i++) {
        if(max<J[i] && cnf->boolarray[i]==NOTSURE) {
            max=J[i];
            index = i;
        }
    }
    int zheng=0,fu=0;
    for(Cp=cnf->root; Cp; Cp=Cp->next) {
        for(Lp=Cp->head; Lp; Lp=Lp->next) {
            if(Lp->literal>0) {
                zheng++;
            } else {
                fu++;
            }
        }
    }
    if(zheng<fu) {
        index=-index;
    }
    free(J);
    return index;
}
int VSIDSStrategy(CNF *cnf)
{
    int i;
    static int count=0;
    int max=-1;
    int index=0;
    for(i=1; i<=cnf->literals; i++) {
        if(max<cnf->countarray[i]+cnf->countarray[i+cnf->literals] && cnf->boolarray[i]==NOTSURE) {
            max=cnf->countarray[i]+cnf->countarray[i+cnf->literals];
            index = i;
        }
    }
    count++;
    if(count>VSIDSCOUNT) {
        count=0;
        for(i=1; i<=cnf->literals*2; i++) {
            cnf->countarray[i]/=VSIDS;
        }
    }
    index=(cnf->countarray[index]>cnf->countarray[index+cnf->literals])?index:-index;
    return index;
}
int weightStrategy(CNF *cnf)
{
    int i;
    int index=0;
    ClauseList *Cp;
    int W=0;

    Cp=cnf->root;
    int ran = rand()%(cnf->clauses);
    for(i=1; i<ran; i++) {
        Cp=Cp->next;
    }
    Cp->weight++;

    i=0;
    while(W==0 && i<=cnf->literals) {
        i++;
        for(; i<=cnf->literals && cnf->boolarray[i]!=NOTSURE; i++) {
            ;
        }
        for(Cp=cnf->root; Cp; Cp=Cp->next) {
            if(evaluateClause(Cp, i)!=TRUE) {
                W-=Cp->weight;
            }
            if(evaluateClause(Cp, -i)!=TRUE) {
                W+=Cp->weight;
            }
        }
        if(W>0) {
            index = i;
        } else if(W<0) {
            index = -i;
        }
    }
    return index;
}
int combineStrategy(CNF *cnf, enum strategy s)
{
    int index=0;
    switch(s) {
    case mom:
        index = MOMStrategy(cnf);
        break;
    case vsids:
        index = VSIDSStrategy(cnf);
        break;
    case first:
        index = cnf->root->head->literal;
        break;
    case occurency:
        index = chooseStrategy_MaxOccurrence(cnf);
        break;
    case weightsat:
        index = weightStrategy(cnf);
        break;
    }
    return index;
}
status saveChange(ChangeStack *head, int tag, ClauseList* Cp, LiteralNode *Lp)
{
    //head是不含数据的头节点指针
    if(head==NULL) {
        return FALSE;
    }
    ChangeStack *newChange = (ChangeStack*)malloc(sizeof(ChangeStack));
    if(!newChange) {
        return ERROR;
    }
    newChange->tag = tag;
    switch(tag) {
    case CLAUSE:
        newChange->Cp=Cp;//保存子句
        break;
    case LITERAL:
        newChange->Cp=Cp;//保存所在子句地址
        newChange->Lp=Lp;//保存变元
        break;
    case SPLIT:
        newChange->Cp=Cp;//保存子句
        break;
    default:
        break;
    }
    newChange->next=head->next;
    head->next=newChange;
    return OK;
}
status reduceChange(CNF *cnf, ChangeStack *head, int time, int *learnarray)
{
    //learnarray组成:第0个元素为长度，第1个开始是变元,第i+1个为唯一蕴含点
    //学习子句计划
    //加入a[]:变元x在当前层的赋值状态X=boolarray[x]*x加入a[]
    //第一次撤销肯定是冲突的子句C变空，所以第一次撤销后该子句C的DIVIDE后的所有变元都加入a[]

    //1.将冲突的变元加入a[]_0

    //2.第i次撤销后，先检查该子句是否有元素，若该子句的元素只有一个(tag为CLAUSE)，且元素在a[]_0内（元素的值取反后与a[]_0比较），该子句C的DIVIDE后的所有变元都加入a[]_0
    //“若该子句的元素只有一个”：是不是蕴含关系
    //“且元素在a[]_0内”：是否与冲突子句有关

    //3.将所有撤销完成后，得到a[]_1，删去a[]_1中本层所有的变量赋值（决策以及推倒出）的变元，得到a[]_2
    //本层：a[]_1中变元i的floorarray[i]的最大值F

    //4.得到需要返回的决策层，即a[]_2中变元i的floorarray[i]的最大值toF

    //5.加入本层的决策X得到a[]
    //决策X：a[]_1中变元i的floorarray[i]的值为F且为最后一个(由后往前遍历)

    //6.插入学习子句Cl => a[]得到最后蕴含关系
    int i=1,j;
    int toF=head->floor;
    status *G=NULL;
    status *Gvisited=NULL;
    LiteralNode *Lp;
    ChangeStack *Sp=head->next;
    status flag=FALSE;
    status isLearn=FALSE;
    while(Sp && time!=0) {
        //PrintChangeStack(head);
        switch(Sp->tag) {
        case CLAUSE:
            insertClauseTop(cnf,Sp->Cp);
            break;
        case LITERAL:
            //子句变元遵循规律：DIVIDE后的变元决策层为降序
            //撤销本层的变元删除操作只需要将本决策层的变元放在DIVIDE前即可
            backLiteral(Sp->Cp,Sp->Lp);
            if(learnarray && flag==FALSE) {
                //将冲突的变元加入a[]_0
                if(Sp->Cp->length==1) {
                    flag=TRUE;
                    G=(status*)malloc(sizeof(status)*(cnf->literals+2));
                    Gvisited = (status*)malloc(sizeof(status)*(cnf->literals+1));
                    for(j=1; j<=cnf->literals; j++) {
                        Gvisited[j]=FALSE;
                    }
                    learnarray[i]=Sp->Cp->head->literal;
                    i++;
                    learnarray[i]=0-Sp->Cp->head->literal;
                    i++;
                    Gvisited[ABS(Sp->Cp->head->literal)]=TRUE;
                }
            }
            break;
        case SPLIT:
            deleteClause(cnf, Sp->Cp);
            break;
        default:
            break;
        }

        if(learnarray && isLearn==FALSE) {

            if(Sp->Cp->length==1) {
                //是否元素在a[]_0内（元素的值取反后与a[]_0比较）
                //监视哨learnarray[i]=0-Sp->Cp->head->literal
                learnarray[i]=0-Sp->Cp->head->literal;
                for(j=1; Sp->Cp->head->literal!=-learnarray[j]; j++) {
                    ;
                }
                if(j!=i) {
                    //访问该顶点
                    G[j]=TRUE;
                    int t=j;
                    //是否全部访问过
                    for(j=2; j<i; j++) {
                        if(G[j]!=TRUE) {
                            break;
                        }
                    }
                    if(j==i && i!=3 && isLearn==FALSE) {
                        isLearn=TRUE;

                        learnarray[0]=i-1;
                        learnarray[i]=-learnarray[t];
                        int f=createLearnClause(cnf,learnarray+1,learnarray[0],-learnarray[t]);
                        if(f<toF) {
                            toF=f;
                        }
                    } else {
                        for(Lp=Sp->Cp->rmv; Lp; Lp=Lp->next) {
                            //删去重复的变元
                            if(Gvisited[ABS(Lp->literal)]==FALSE) {
                                learnarray[i]=Lp->literal;
                                G[i]=FALSE;
                                Gvisited[ABS(Lp->literal)] = TRUE;
                                if(cnf->floorarray[ABS(learnarray[i])]<head->floor) {
                                    G[i]=TRUE;
                                }
                                i++;
                            }
                        }
                    }
                }
            }
        }
        time--;
        head->next=head->next->next;
        free(Sp);
        Sp=head->next;
    }
    if(learnarray) {
        free(G);
        free(Gvisited);
        learnarray[0]=toF;
    }
    return OK;
}
int createLearnClause(CNF *cnf, int *a, int i, int X)
{
    int j,l;
    int *copy = a;
    int F=0,toF=0;
    //由后往前减少赋值次数
    for(j=i-1; j>=0; j--) {
        if(F<cnf->floorarray[ABS(copy[j])]) {
            F=cnf->floorarray[ABS(copy[j])];
        }
    }
    //3.将所有撤销完成后，得到a[]_1，删去a[]_1中本层所有的变量赋值（决策以及推倒出）的变元，得到a[]_2
    for(l=0,j=0; j<i; j++) {
        if(cnf->floorarray[ABS(copy[j])]!=F) {
            copy[l]=copy[j];
            l++;
        }
    }
    //4.得到需要返回的决策层，即a[]_2中变元i的floorarray[i]的最大值toF
    for(j=0; j<l; j++) {
        if(toF<cnf->floorarray[ABS(copy[j])]) {
            toF=cnf->floorarray[ABS(copy[j])];
        }
    }
    // **如果学习子句长度过长，只生成决策单子句，在回溯的时候将其删除
    int L=l+1;//长学习子句的原长

    //排序变元，使得变元决策层为降序
    Floor *FF = (Floor*)malloc(sizeof(Floor)*l);
    for(i=0; i<l; i++) {
        FF[i].a = copy[i];
        FF[i].floor = cnf->floorarray[ABS(copy[i])];
    }
    qsort(FF, l, sizeof(Floor), compare_Des);
    for(i=0; i<l; i++) {
        copy[i] = FF[i].a;
    }
    free(FF);

    //5.加入本层的决策X得到a[]
    copy[l]=-X;
    l++;
    int tmp;
    for(j=l-1; j>0; j--) {
        tmp=copy[j-1];
        copy[j-1]=copy[j];
        copy[j]=tmp;
    }

    //6.插入学习子句Cl => a[]得到最后蕴含关系
    addClause(cnf,l,copy);

    LearnClauseList *lcp=(LearnClauseList*)malloc(sizeof(LearnClauseList));
    lcp->isInStack=FALSE;
    lcp->floor=F;
    lcp->clause=cnf->root;
    lcp->countNum=L;
    lcp->next=cnf->learn_root;
    cnf->learn_root=lcp;

    /* VSIDS */
    for(i=0; i<L; i++) {
        if(copy[i]>0) {
            cnf->countarray[copy[i]]++;
        } else {
            cnf->countarray[-copy[i]+cnf->literals]++;
        }
    }

    //添加学习子句的索引
    addLIndex(cnf,lcp->clause);

    //学习子句的变元有赋值
    while(cnf->root->head) {
        removeLiteral(cnf->root,cnf->root->head->literal);
    }
    return toF;
}
status backLearnClause(CNF *cnf, int floor)
{
    LiteralNode *Lp;
    LearnClauseList *lcp=cnf->learn_root;
    while(lcp) {
        if(lcp->isInStack==FALSE) {
            for(Lp=lcp->clause->rmv; Lp && cnf->floorarray[ABS(Lp->literal)]==floor; Lp=lcp->clause->rmv) {
                backLiteral(lcp->clause,Lp);
            }
            if(Lp==NULL) {
                lcp->isInStack=TRUE;
            }
        }
        lcp=lcp->next;
    }
    return OK;
}
status deleteRepeatLearnClause(CNF *cnf, LearnClauseList *Lcp)
{
    LiteralNode *Lp1,*Lp2;
    LearnClauseList *lcp=cnf->learn_root;

    lcp=Lcp->next;
    if(lcp) {
        Lp1=lcp->clause->head;
        if(Lp1 && Lp1->literal==Lcp->clause->head->literal) {
            for(Lp1=lcp->clause->rmv,Lp2=cnf->learn_root->clause->rmv; Lp1 && Lp2; Lp1=Lp1->next,Lp2=Lp2->next) {
                if(Lp1->literal!=Lp2->literal) {
                    break;
                }
            }
            if(Lp1==NULL && Lp2==NULL) {
                LearnClauseList *tmp=lcp->next;
                deleteLearnClause(cnf,lcp);
                lcp=tmp;
            } else {
                lcp=lcp->next;
            }
        }
    }
    return OK;
}
status backAssign(CNF *cnf, int floor)
{
    int j;
    for(j=1; j<=cnf->literals; j++) {
        if(cnf->floorarray[j]==floor) {
            cnf->boolarray[j]=NOTSURE;
            cnf->floorarray[j]=-1;
        }
    }
    return OK;
}
status checkRes(CNF *cnf)
{
    int i,b;
    int flag;
    ClauseList *Cp;
    LiteralNode *Lp;
    printf("------------------------------------------------\n");
    printf("正在检查: \n");
    for(Cp=cnf->root,i=1; Cp; Cp=Cp->next,i++) {
        printf("第%d个子句: ",i);
        for(Lp=Cp->head,flag=0; Lp; Lp=Lp->next) {
            b=cnf->boolarray[(ABS(Lp->literal))];
            if(EVALUE(Lp->literal)==b) {
                printf("√");
                flag=1;
                break;
            } else if(b==NOTSURE) {
                printf("%dNOTsure ",ABS(Lp->literal));
            } else if(EVALUE(Lp->literal)==-b) {
                continue;
            }
        }
        if(!flag) {
            printf("x");
        }
        if(i%4==0) {
            printf("\n");
        } else {
            printf("\t\t");
        }
    }
    printf("\n");
    return OK;
}
status PrintChangeStack(ChangeStack *head)
{
    ChangeStack *p;
    printf("---------------------------\n");
    printf("打印Change栈\n");
    for(p=head->next; p; p=p->next) {
        switch(p->tag) {
        case CLAUSE:
            printf("CLAUSE:\n");
            printf("loc:%p\n",p->Cp);
            break;
        case LITERAL:
            printf("LITERAL:\n");
            printf("Literal:%p Clause:%p\n",p->Lp,p->Cp);
            break;
        case SPLIT:
            printf("SPLIT:\n");
            printf("split:%p\n",p->Cp);
            break;
        default:
            break;
        }
    }
    return OK;
}

int compare_Des(const void *a,const void *b)
{
    return ((Floor*)b)->floor - ((Floor*)a)->floor;
}
