#include "DPLL.h"

#define DEBUG2 0

#define VSIDS 3
#define VSIDSCOUNT 10
#define MAXC 8
#define LEARNLENGTH_MAX 20

status reset=FALSE;

//boolean DPLLRec(CNF *cnf,const int f)
//{
//    /* S为公式对应的子句集。若其满足，返回TURE；否则返回FALSE. */
//    int l,t=cnf->root->head->literal;
//    int ret=f;//回溯层数
//    #if DEBUG2
//    printf("递归:%d, 选%d\n",f,cnf->root->head->literal);
//    #endif // DEBUG2
//
//    LearnClauseList *lcp;
//    ClauseList *Cp;
//    LiteralNode *Lp;
//    ChangeStack st;
//    st.floor=f;
//    st.next=NULL;
//    while((Cp=locateUnitClause(cnf))!=NULL) {//单子句传播
//        boolarrayAssign(cnf,Cp->head->literal,f);//记录到数组
//        simplySingleClause(cnf, Cp->head->literal, &st);//化简单子句并保存修改
//
//        #if DEBUG
//        printClause(cnf);
//        getchar();
//        #endif // DEBUG
//        if(isHaveEmptyClause(cnf)==TRUE){//如果有空子句
//            #if DEBUG2
//            //printf("回溯:%d, 选%d\n",f,cnf->root->head->literal);
//            #endif // DEBUG2
//            //空子句在置顶
//
//            Cp=cnf->root;
//            reduceChange(cnf, &st,-1, &ret);//还原到初始集
//            /*如果取值不成立，可能要添加学习子句并非时序回溯...*/
//
//            #if DEBUG2
//            //printClause(cnf);
//            //getchar();
//            printf("跳至%d\n",ret);
//            #endif // DEBUG2
//
//            /*LearnClauseList *lcp;
//            for(lcp=cnf->learn_root;lcp;lcp=lcp->next){
//                if(lcp->isInStack==FALSE){
//                    for(Lp=lcp->clause->rmv;Lp && cnf->floorarray[ABS(Lp->literal)]==f;Lp=lcp->clause->rmv){
//                        backLiteral(lcp->clause,Lp);
//                    }
//                    if(Lp==NULL){
//                        lcp->isInStack=TRUE;
//                    }
//                }
//            }*/
//            #if DEBUG
//            printf("回溯isHaveEmptyClause\n");
//
//            #endif // DEBUG
//            //赋值回溯
//            int j;
//            for(j=1;j<=cnf->literals;j++){
//                if(cnf->floorarray[j]==f){
//                    cnf->boolarray[j]=NOTSURE;
//                    cnf->floorarray[j]=-1;
//                }
//            }
//            #if DEBUG2
//            {
//                printf("回跳1\n");
//                printf("floor:\n");
//                int j;
//                for(j=1;j<=cnf->literals;j++){
//                    printf("%d:%d ",j,cnf->floorarray[j]*cnf->boolarray[j]);
//                }
//                printf("\n");
//                printfLearnClause(cnf);
//                //printClause(cnf);
//                //getchar();
//            }
//            #endif // DEBUG2
//            return ret-f;
//            return FALSE;
//        }
//        else if(isHaveClause(cnf)==FALSE){//如果没有子句
//            reduceChange(cnf, &st,-1, NULL);//还原到初始集
//
//            #if DEBUG
//            printf("回溯isHaveClause\n");
//            printClause(cnf);
//            #endif // DEBUG
//            return TRUE;
//        }
//    }//while
//    l=combineStrategy(cnf);//综合策略选出变元
//    /* VSIDS */
//    //boolarrayAssign(cnf,l);
//    addClause(cnf,1,&l);
//    saveChange(&st,SPLIT,cnf->root,NULL);//保存插入操作
//    int s=DPLLRec(cnf,f+1);
//    if(s==TRUE){
//        //PrintChangeStack(&st);
//        reduceChange(cnf,&st,-1,NULL);
//        #if DEBUG
//        printf("回溯l\n");
//        printClause(cnf);
//        #endif // DEBUG
//        return TRUE;
//    }
//    //按返回值多层回溯
//    if(s<FALSE){
//        reduceChange(cnf,&st,-1,NULL);
//        //赋值回溯
//        int j;
//        for(j=1;j<=cnf->literals;j++){
//            if(cnf->floorarray[j]==f){
//                cnf->boolarray[j]=NOTSURE;
//                cnf->floorarray[j]=-1;
//            }
//        }
///*
//        LearnClauseList *lcp;
//        for(lcp=cnf->learn_root;lcp;lcp=lcp->next){
//            if(lcp->isInStack==FALSE){
//                for(Lp=lcp->clause->rmv;Lp && cnf->floorarray[ABS(Lp->literal)]==f;Lp=lcp->clause->rmv){
//                    backLiteral(lcp->clause,Lp);
//                }
//                if(Lp==NULL){
//                    lcp->isInStack=TRUE;
//                }
//            }
//        }*/
//        #if DEBUG2
//        {
//            int j;
//            printf("正在回跳%d\n",f);
//            printf("floor:\n");
//            for(j=1;j<=cnf->literals;j++){
//                printf("%d:%d ",j,cnf->floorarray[j]*cnf->boolarray[j]);
//            }
//            printf("\n");
//            printfLearnClause(cnf);
//            //printClause(cnf);
//            //getchar();
//        }
//        #endif // DEBUG2
//        return s+1;
//    }
//    reduceChange(cnf,&st,1,NULL);//撤销插入的操作,bug是LITERAL所指的clause指针在递归时已经被释放，回溯的clause地址不是原先的地址
//    //printClause(cnf);
//
//    l=0-l;//取反
//    //boolarrayAssign(cnf,l);
//    addClause(cnf,1,&l);
//    saveChange(&st,SPLIT,cnf->root,NULL);//保存插入操作
//
//
//
//    s=DPLLRec(cnf,f+1);
//    //PrintChangeStack(&st);
//    reduceChange(cnf,&st,-1,NULL);//还原到初始集
//
//    if(s<=FALSE){
//        //赋值回溯
//        int j;
//        for(j=1;j<=cnf->literals;j++){
//            if(cnf->floorarray[j]==f){
//                cnf->boolarray[j]=NOTSURE;
//                cnf->floorarray[j]=-1;
//            }
//        }
//    }
//    #if DEBUG2
//    printf("回跳2\n");
//    printfLearnClause(cnf);
//    //printf("回溯-l\n");
//    //printClause(cnf);
//    #endif // DEBUG
//    return (s==FALSE || s==TRUE)?s:s+1;
//}
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
    do{
        count++;
        if(reset==TRUE){
            reset=FALSE;
            /*如果重启动，可以把那些没有移除变元的、很长的学习子句删除*/
            LearnClauseList *lcp=cnf->learn_root,*lcq=NULL;
            while(lcp){
                if(lcp->clause->rmv==NULL && lcp->clause->length>LEARNLENGTH_MAX){
                    lcq=lcp->next;
                    deleteLearnClause(cnf,lcp);
                    lcp=lcq;
                }
                else{
                    lcp=lcp->next;
                }
            }
        }
        #if DEBUG2
        //
        //printf("化简前\n");
        //printClause(cnf);
        #endif // DEBUG2
        while((Cp=locateUnitClause(cnf))!=NULL) {//单子句传播
            boolarrayAssign(cnf,Cp->head->literal,f);//记录到数组
            simplySingleClause(cnf, Cp->head->literal, &st);//化简单子句并保存修改
            if(isHaveEmptyClause(cnf)==TRUE){//如果有空子句
                #if DEBUG2
                //printf("化简后\n");
                //printClause(cnf);
                #endif // DEBUG2
                //空子句在置顶
                int *a=(int*)malloc(sizeof(int)*cnf->literals);
                reduceChange(cnf, &st,-1, a);//还原到初始集
                /*如果取值不成立，可能要添加学习子句并非时序回溯...*/
                ret=createLearnClause(cnf,a+1,a[0],a[a[0]+1]);
                free(a);
                backLearnClause(cnf,f);
                //deleteRepeatLearnClause(cnf, cnf->learn_root);
                #if DEBUG2
                //getchar();
                printf("跳至%d\n",ret);
                #endif // DEBUG2
                #if DEBUG
                printf("回溯isHaveEmptyClause\n");
                #endif // DEBUG
                //赋值回溯
                backAssign(cnf,f);
                if(count>MAXC){
                    printf("重启动...\n");
                    reset=TRUE;
                    //学习子句的回溯
                    return 0-f;
                }
                #if DEBUG2
                {
                    printf("回跳\n");
                    printf("floor:\n");
                    int j;
                    for(j=1;j<=cnf->literals;j++){
                        printf("%d:%d ",j,cnf->floorarray[j]*cnf->boolarray[j]);
                    }
                    printf("\n");
                    printfLearnClause(cnf);
                    //printClause(cnf);
                    //getchar();
                }
                #endif // DEBUG2
                return ret-f;
            }
            else if(isHaveClause(cnf)==FALSE){//如果没有子句
                reduceChange(cnf, &st,-1, NULL);//还原到初始集
                //学习子句的回溯
                backLearnClause(cnf, f);
                return TRUE;
            }
        }//while
        l=combineStrategy(cnf);//综合策略选出变元
        /* VSIDS */
        addClause(cnf,1,&l);
        saveChange(&st,SPLIT,cnf->root,NULL);//保存插入操作
        printf("递归:%d, 选%d\n",f,l);
        s=DPLLRec(cnf,f+1);
        reduceChange(cnf,&st,1,NULL);
    }while(s==FALSE);
    if(s==TRUE){
        reduceChange(cnf,&st,-1,NULL);
        //学习子句的回溯
        backLearnClause(cnf,f);
        return TRUE;
    }
    //按返回值多层回溯
    else{
        reduceChange(cnf,&st,-1,NULL);
        //学习子句的回溯
        backLearnClause(cnf,f);
        //赋值回溯
        backAssign(cnf,f);
        #if DEBUG2
        {
            int j;
            printf("正在回跳%d\n",f);
            printf("floor:\n");
            for(j=1;j<=cnf->literals;j++){
                printf("%d:%d ",j,cnf->floorarray[j]*cnf->boolarray[j]);
            }
            printf("\n");
            printfLearnClause(cnf);
            //printClause(cnf);
            //getchar();
        }
        #endif // DEBUG2
        return s+1;
    }
}
boolean DPLLRec2(CNF *cnf)
{
    /* S为公式对应的子句集。若其满足，返回TURE；否则返回FALSE. */
    int l;
    ClauseList *Cp;
    ChangeStack st;
    st.next=NULL;
    while((Cp=locateUnitClause(cnf))!=NULL) {//单子句传播
        boolarrayAssign(cnf,Cp->head->literal,-1);//记录到数组
        simplySingleClause(cnf, Cp->head->literal, &st);//化简单子句
        if(isHaveEmptyClause(cnf)==TRUE){//如果有空子句
            reduceChange(cnf,&st,-1,NULL);
            return FALSE;
        }
        else if(isHaveClause(cnf)==FALSE){//如果没有子句
            reduceChange(cnf,&st,-1,NULL);
            return TRUE;
        }

    }//while
    l=combineStrategy(cnf);//综合策略选出变元
    boolarrayAssign(cnf,l,-1);//记录到数组
    addClause(cnf,1,&l);//添加到子句集中
    saveChange(&st,SPLIT,cnf->root,NULL);
    int s=DPLLRec2(cnf);//递归
    if(s==TRUE){//如果是真
        reduceChange(cnf,&st,-1,NULL);
        return TRUE;
    }
    reduceChange(cnf,&st,1,NULL);
    l=-l;//取假值
    boolarrayAssign(cnf,l,-1);//记录到数组
    addClause(cnf,1,&l);//添加到子句集中
    saveChange(&st,SPLIT,cnf->root,NULL);
    s=DPLLRec2(cnf);//递归
    reduceChange(cnf,&st,-1,NULL);
    return s;
}
boolean DPLLUnRec();//非递归

status simplySingleClause(CNF *cnf, int literal, ChangeStack *head)
{
    LiteralNode *Lp;
    ClauseList *Cp=cnf->root;
    ClauseList *Cq;
    while(Cp){
        switch(evaluateClause(Cp,literal))
        {
        case TRUE:
            removeClause(cnf, Cp);
            saveChange(head,CLAUSE,Cp,NULL);
            Cp=Cp->next;
            //printClause(cnf);
            break;
        case NOTSURE:
            Lp=removeLiteral(Cp,-literal);
            saveChange(head,LITERAL,Cp,Lp);
            //如果为空语句,把空语句置顶返回
            //如果为单语句,置顶
            //如果为双语句,置顶
            //上述不需要
            /*if(Cp->length<=2){
                Cq=Cp->next;
                removeClause(cnf,Cp);
                insertClauseTop(cnf,Cp);
                if(Cp->length==0){
                    return ERROR;
                }
                else{
                    Cp=Cq;
                }
            }
            else{
                Cp=Cp->next;
            }*/
            Cp=Cp->next;
            //Lp->next=NULL;
            //printClause(cnf);
            break;
        default:
            Cp=Cp->next;
            break;
        }
    }
    return OK;
}
status simplifyClause();//化简子句
int chooseStrategy_MaxOccurrence(CNF *cnf)
{
    int i;
    int max=0;
    int index=0;
    ClauseList *Cp;
    LiteralNode *Lp;
    int *visitedplus=(int*)malloc((cnf->literals+1)*sizeof(int));
    int *visitedminus=(int*)malloc((cnf->literals+1)*sizeof(int));
    for(i=1;i<=cnf->literals;i++){
        visitedplus[i]=visitedminus[i]=0;
    }
    for(Cp=cnf->root;Cp;Cp=Cp->next){
        for(Lp=Cp->head;Lp;Lp=Lp->next){
            if(Lp->literal>0){
                visitedplus[Lp->literal]++;
            }
            else{
                visitedminus[0-Lp->literal]++;
            }
        }
    }
    for(i=1;i<=cnf->literals;i++)
        if(max<visitedplus[i]+visitedminus[i]){
            max=visitedplus[i]+visitedminus[i];
            index = i;
        }
//        else if(max==visitedplus[i]+visitedminus[i]){
//            if(rand()%2==0){
//                index = i;//取概率不平等,取消
//            }
//        }
    index=(visitedplus[index]<visitedminus[index])?-index:index;
    free(visitedplus);
    free(visitedminus);
    return index;
}
int chooseStrategy_MaxOccurrenceTwoLiteral(CNF *cnf)
{
    int i;
    int max=0;
    int index=0;
    ClauseList *Cp;
    LiteralNode *Lp;
    int *visitedplus=(int*)malloc((cnf->literals+1)*sizeof(int));
    int *visitedminus=(int*)malloc((cnf->literals+1)*sizeof(int));
    for(i=1;i<=cnf->literals;i++){
        visitedplus[i]=visitedminus[i]=0;
    }
    for(Cp=cnf->root;Cp && Cp->length<=2;Cp=Cp->next){
        //在化简单子句时已经把length<=2的子句全部置顶了,但是在第一次递归时可能并没有
        for(Lp=Cp->head;Lp;Lp=Lp->next){
            if(Lp->literal>0){
                visitedplus[Lp->literal]++;
            }
            else{
                visitedminus[-Lp->literal]++;
            }
        }
    }
    for(i=1;i<=cnf->literals;i++){
        if(max<visitedplus[i]+visitedminus[i]){
            max=visitedplus[i]+visitedminus[i];
            index = i;
        }
    }
    index=(visitedplus[index]<visitedminus[index])?-index:index;
    free(visitedplus);
    free(visitedminus);
    return index;
}
int VSIDSStrategy(CNF *cnf)
{
    int i;
    static int count=0;
    int max=0;
    int index=0;
    ClauseList *Cp;
    LiteralNode *Lp;
    for(Cp=cnf->root;Cp;Cp=Cp->next){
        //在化简单子句时已经把length<=2的子句全部置顶了,但是在第一次递归时可能并没有
        for(Lp=Cp->head;Lp;Lp=Lp->next){
            if(Lp->literal>0){
                cnf->countarray[Lp->literal]++;
            }
            else{
                cnf->countarray[-Lp->literal]++;
            }
        }
    }
    for(i=1;i<=cnf->literals;i++){
        if(max<cnf->countarray[i] && cnf->boolarray[i]==NOTSURE){
            max=cnf->countarray[i];
            index = i;
        }
    }
    count++;
    if(count>VSIDSCOUNT){
        count=0;
        for(i=1;i<=cnf->literals;i++){
            cnf->countarray[i]/=VSIDS;
        }
    }
    index=(count%2)?index:-index;
    return index;
}
int combineStrategy(CNF *cnf)
{
    /*int index=0;
    index=chooseStrategy_MaxOccurrenceTwoLiteral(cnf);
    if(index!=0){
        return index;
    }
    return chooseStrategy_MaxOccurrence(cnf);*/
    return VSIDSStrategy(cnf);
    return cnf->root->head->literal;
}
status saveChange(ChangeStack *head, int tag, ClauseList* Cp, LiteralNode *Lp)
{
    //head是不含数据的头节点指针
    if(head==NULL){
        return FALSE;
    }
    ChangeStack *newChange = (ChangeStack*)malloc(sizeof(ChangeStack));
    if(!newChange){
        return ERROR;
    }
    newChange->tag = tag;
    switch(tag)
    {
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
    status *a=NULL;
    LiteralNode *Lp;
    ChangeStack *Sp=head->next;
    status flag=FALSE;
    status isLearn=FALSE;
    while(Sp && time!=0){
        //PrintChangeStack(head);
        switch(Sp->tag)
        {
        case CLAUSE:
            insertClauseTop(cnf,Sp->Cp);
            break;
        case LITERAL:
            //子句变元遵循规律：DIVIDE后的变元决策层为降序
            //撤销本层的变元删除操作只需要将本决策层的变元放在DIVIDE前即可
            backLiteral(Sp->Cp,Sp->Lp);
            if(learnarray && flag==FALSE){
                //将冲突的变元加入a[]_0
                if(Sp->Cp->length==1){
                    flag=TRUE;
                    a=(status*)malloc(sizeof(status)*cnf->literals+1);
                    learnarray[i]=Sp->Cp->head->literal;
                    i++;
                    learnarray[i]=0-Sp->Cp->head->literal;
                    i++;
                    #if DEBUG2
                    printf("冲突：Cp=%p,l=%d\n",Sp->Cp,a[0]);
                    #endif // DEBUG2
                }
            }
            break;
        case SPLIT:
            deleteClause(cnf, Sp->Cp);
            break;
        default:
            break;
        }

        if(learnarray && isLearn==FALSE){

            if(Sp->Cp->length==1){
                //是否元素在a[]_0内（元素的值取反后与a[]_0比较）
                for(j=1;j<i;j++){
                    if(Sp->Cp->head->literal==-learnarray[j]){
                        break;
                    }
                }
                //该子句C的DIVIDE后的所有变元都加入a[]_0
                if(i!=j){
                    //访问该顶点
                    a[j]=TRUE;
                    int t=j;
                    //是否全部访问过
                    for(j=2;j<i;j++){
                        if(a[j]!=TRUE){
                            break;
                        }
                    }
                    if(j==i && i!=3 && isLearn==FALSE){
                        isLearn=TRUE;
                        learnarray[0]=i-1;
                        learnarray[i]=-learnarray[t];
                    }
                    else{

                        //printf("子句%p, %d: ",Sp->Cp,Sp->Cp->head->literal);

                        for(Lp=Sp->Cp->rmv;Lp;Lp=Lp->next){

                            //printf("%d ",Lp->literal);
                            //删去重复的变元
                            for(j=1;j<i;j++){
                                if(learnarray[j]==Lp->literal){
                                    break;
                                }
                            }
                            if(j==i){
                                learnarray[i]=Lp->literal;
                                a[i]=FALSE;
                                if(cnf->floorarray[ABS(learnarray[i])]<head->floor){
                                    a[i]=TRUE;
                                }
                                i++;
                            }
                        }
                    }
                    //printf("\n");
                }
                //如果是过长的学习子句，那么先加入a[]，再删除（不可行,因为会产生短的但是隐含前提错误的子句）
            }
        }
        time--;
        head->next=head->next->next;
        free(Sp);
        Sp=head->next;
    }
    if(learnarray){
        free(a);
    }
    return OK;
}
int createLearnClause(CNF *cnf, int *a, int i, int X)
{
    int j,l;
    int *copy=(int*)malloc(sizeof(int)*i);
    for(j=0;j<i;j++){
        copy[j]=a[j];
    }
    int F=0,toF=0;

    /*printf("学习前a=");
    for(j=0;j<i;j++){
        printf("%d ",a[j]);
    }
    printf("\n");
    printf("floor:\n");
    for(j=1;j<=cnf->literals;j++){
        printf("%d:%d ",j,cnf->floorarray[j]*cnf->boolarray[j]);
    }
    printf("\n");*/

    //由后往前减少赋值次数
    for(j=i-1;j>=0;j--){
        if(F<cnf->floorarray[ABS(copy[j])]){
            F=cnf->floorarray[ABS(copy[j])];
        }
    }
    //3.将所有撤销完成后，得到a[]_1，删去a[]_1中本层所有的变量赋值（决策以及推倒出）的变元，得到a[]_2
    for(l=0,j=0;j<i;j++){
        if(cnf->floorarray[ABS(copy[j])]!=F && cnf->floorarray[ABS(copy[j])]!=-1){
            copy[l]=copy[j];
            l++;
        }
    }
    //4.得到需要返回的决策层，即a[]_2中变元i的floorarray[i]的最大值toF
    for(j=0;j<l;j++){
        if(toF<cnf->floorarray[ABS(copy[j])]){
            toF=cnf->floorarray[ABS(copy[j])];
        }
    }
    // **如果学习子句长度过长，只生成决策单子句，在回溯的时候将其删除
    int L=l+1;//长学习子句的原长
    /*if(L>LEARNLENGTH_MAX){
        l=0;
    }*/

    //排序变元，使得变元决策层为降序
    //插入排序
    for(i=1;i<l;i++){
        for(j=i-1;j>=0;j--){
            if(cnf->floorarray[ABS(copy[j+1])]>cnf->floorarray[ABS(copy[j])]){
                int tmp=copy[j+1];
                copy[j+1]=copy[j];
                copy[j]=tmp;
            }
        }
    }
    //5.加入本层的决策X得到a[]
    copy[l]=-X;
    l++;
    for(j=l-1;j>0;j--){
        int tmp=copy[j-1];
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
    //学习子句的变元有赋值
    while(cnf->root->head){
        removeLiteral(cnf->root,cnf->root->head->literal);
    }
/*
    printf("学习%p a = ",cnf->root);
    for(j=0;j<l;j++){
        printf("%d,%d ",copy[j],cnf->floorarray[ABS(copy[j])]);
    }
    printf("L=%d\n",L);
*/
    free(copy);
    return toF;
}
status backLearnClause(CNF *cnf, int floor)
{
    LiteralNode *Lp;
    LearnClauseList *lcp=cnf->learn_root,*lcq=NULL;
    while(lcp){
        if(lcp->isInStack==FALSE){
            for(Lp=lcp->clause->rmv;Lp && cnf->floorarray[ABS(Lp->literal)]==floor;Lp=lcp->clause->rmv){
                backLiteral(lcp->clause,Lp);
            }
            if(Lp==NULL){
                //printf("删前\n");
                //printfLearnClause(cnf);

                if(lcp->clause->length>LEARNLENGTH_MAX){
                    if(lcq==NULL){
                        cnf->learn_root=lcp->next;
                        deleteClause(cnf,lcp->clause);
                        free(lcp);
                        lcp=cnf->learn_root;
                        continue;
                    }
                    else{
                        lcq->next=lcp->next;
                        deleteClause(cnf,lcp->clause);
                        free(lcp);
                        lcp=lcq;
                    }

                }
                else
                    lcp->isInStack=TRUE;
                //printf("删后\n");
                //printfLearnClause(cnf);
            }
        }
        lcq=lcp;
        lcp=lcp->next;
    }
    return OK;
}
status deleteRepeatLearnClause(CNF *cnf, LearnClauseList *Lcp)
{
    //boolean flag=FALSE;
    LiteralNode *Lp1,*Lp2;
    LearnClauseList *lcp=cnf->learn_root;
    //printfLearnClause(cnf);

    lcp=Lcp->next;
    if(lcp){
        Lp1=lcp->clause->head;
        if(Lp1 && Lp1->literal==Lcp->clause->head->literal){
            for(Lp1=lcp->clause->rmv,Lp2=cnf->learn_root->clause->rmv;Lp1 && Lp2;Lp1=Lp1->next,Lp2=Lp2->next){
                if(Lp1->literal!=Lp2->literal){
                    break;
                }
            }
            if(Lp1==NULL && Lp2==NULL){
                LearnClauseList *tmp=lcp->next;
                deleteLearnClause(cnf,lcp);
                lcp=tmp;
            }
            else{
                lcp=lcp->next;
            }
        }
    }

/*
    lcp=Lcp->next;
    while(lcp){
        flag=FALSE;
        for(Lp1=lcp->clause->head;Lp1 && lcp->isInStack==FALSE;Lp1=Lp1->next){
            //对于已经加入到回溯栈的子句不能删
            if(Lp1->literal==Lcp->clause->head->literal){
                flag=TRUE;
                for(Lp1=lcp->clause->rmv,Lp2=cnf->learn_root->clause->rmv;Lp1 && Lp2;Lp1=Lp1->next,Lp2=Lp2->next){
                    if(Lp1->literal!=Lp2->literal){
                        break;
                    }
                }
                if(Lp1==NULL && Lp2==NULL){
                    LearnClauseList *tmp=lcp->next;
                    deleteLearnClause(cnf,lcp);
                    lcp=tmp;
                }
                else{
                    lcp=lcp->next;
                }
                break;
            }
        }
        if(flag==FALSE){
            lcp=lcp->next;
        }
    }*/
    //printfLearnClause(cnf);
    //printClause(cnf);
    //getchar();
    return OK;
}
status backAssign(CNF *cnf, int floor)
{
    int j;
    for(j=1;j<=cnf->literals;j++){
        if(cnf->floorarray[j]==floor){
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
    for(Cp=cnf->root,i=1;Cp;Cp=Cp->next,i++){
        printf("第%d个子句: ",i);
        for(Lp=Cp->head,flag=0;Lp;Lp=Lp->next){
            b=cnf->boolarray[(ABS(Lp->literal))];
            if(EVALUE(Lp->literal)==b){
                printf("√");
                flag=1;
                break;
            }
            else if(b==NOTSURE){
                printf("%dNOTsure ",ABS(Lp->literal));
            }
            else if(EVALUE(Lp->literal)==-b){
                continue;
            }
        }
        if(!flag){
            printf("x");
        }
        if(i%4==0){
            printf("\n");
        }
        else{
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
    for(p=head->next;p;p=p->next){
        switch(p->tag)
        {
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
