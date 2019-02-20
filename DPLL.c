#include "DPLL.h"

#define DEBUG2 0

//boolean DPLLRec(CNF *cnf,const int f)
//{
//    /* SΪ��ʽ��Ӧ���Ӿ伯���������㣬����TURE�����򷵻�FALSE. */
//    int l,t=cnf->root->head->literal;
//    int ret=f;//���ݲ���
//    #if DEBUG2
//    printf("�ݹ�:%d, ѡ%d\n",f,cnf->root->head->literal);
//    #endif // DEBUG2
//
//    LearnClauseList *lcp;
//    ClauseList *Cp;
//    LiteralNode *Lp;
//    ChangeStack st;
//    st.floor=f;
//    st.next=NULL;
//    while((Cp=locateUnitClause(cnf))!=NULL) {//���Ӿ䴫��
//        boolarrayAssign(cnf,Cp->head->literal,f);//��¼������
//        simplySingleClause(cnf, Cp->head->literal, &st);//�����Ӿ䲢�����޸�
//
//        #if DEBUG
//        printClause(cnf);
//        getchar();
//        #endif // DEBUG
//        if(isHaveEmptyClause(cnf)==TRUE){//����п��Ӿ�
//            #if DEBUG2
//            //printf("����:%d, ѡ%d\n",f,cnf->root->head->literal);
//            #endif // DEBUG2
//            //���Ӿ����ö�
//
//            Cp=cnf->root;
//            reduceChange(cnf, &st,-1, &ret);//��ԭ����ʼ��
//            /*���ȡֵ������������Ҫ���ѧϰ�Ӿ䲢��ʱ�����...*/
//
//            #if DEBUG2
//            //printClause(cnf);
//            //getchar();
//            printf("����%d\n",ret);
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
//            printf("����isHaveEmptyClause\n");
//
//            #endif // DEBUG
//            //��ֵ����
//            int j;
//            for(j=1;j<=cnf->literals;j++){
//                if(cnf->floorarray[j]==f){
//                    cnf->boolarray[j]=NOTSURE;
//                    cnf->floorarray[j]=-1;
//                }
//            }
//            #if DEBUG2
//            {
//                printf("����1\n");
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
//        else if(isHaveClause(cnf)==FALSE){//���û���Ӿ�
//            reduceChange(cnf, &st,-1, NULL);//��ԭ����ʼ��
//
//            #if DEBUG
//            printf("����isHaveClause\n");
//            printClause(cnf);
//            #endif // DEBUG
//            return TRUE;
//        }
//    }//while
//    l=combineStrategy(cnf);//�ۺϲ���ѡ����Ԫ
//    /* VSIDS */
//    //boolarrayAssign(cnf,l);
//    addClause(cnf,1,&l);
//    saveChange(&st,SPLIT,cnf->root,NULL);//����������
//    int s=DPLLRec(cnf,f+1);
//    if(s==TRUE){
//        //PrintChangeStack(&st);
//        reduceChange(cnf,&st,-1,NULL);
//        #if DEBUG
//        printf("����l\n");
//        printClause(cnf);
//        #endif // DEBUG
//        return TRUE;
//    }
//    //������ֵ������
//    if(s<FALSE){
//        reduceChange(cnf,&st,-1,NULL);
//        //��ֵ����
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
//            printf("���ڻ���%d\n",f);
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
//    reduceChange(cnf,&st,1,NULL);//��������Ĳ���,bug��LITERAL��ָ��clauseָ���ڵݹ�ʱ�Ѿ����ͷţ����ݵ�clause��ַ����ԭ�ȵĵ�ַ
//    //printClause(cnf);
//
//    l=0-l;//ȡ��
//    //boolarrayAssign(cnf,l);
//    addClause(cnf,1,&l);
//    saveChange(&st,SPLIT,cnf->root,NULL);//����������
//
//
//
//    s=DPLLRec(cnf,f+1);
//    //PrintChangeStack(&st);
//    reduceChange(cnf,&st,-1,NULL);//��ԭ����ʼ��
//
//    if(s<=FALSE){
//        //��ֵ����
//        int j;
//        for(j=1;j<=cnf->literals;j++){
//            if(cnf->floorarray[j]==f){
//                cnf->boolarray[j]=NOTSURE;
//                cnf->floorarray[j]=-1;
//            }
//        }
//    }
//    #if DEBUG2
//    printf("����2\n");
//    printfLearnClause(cnf);
//    //printf("����-l\n");
//    //printClause(cnf);
//    #endif // DEBUG
//    return (s==FALSE || s==TRUE)?s:s+1;
//}
boolean DPLLRec(CNF *cnf,const int f)
{
    /* SΪ��ʽ��Ӧ���Ӿ伯���������㣬����TURE�����򷵻�FALSE. */
    int s,l;
    int ret=f;//���ݲ���
    ClauseList *Cp;
    ChangeStack st;
    st.floor=f;
    st.next=NULL;
    do{
        //printf("�ݹ�:%d, ѡ%d\n",f,cnf->root->head->literal);
        #if DEBUG2
        //
        //printf("����ǰ\n");
        //printClause(cnf);
        #endif // DEBUG2
        while((Cp=locateUnitClause(cnf))!=NULL) {//���Ӿ䴫��
            boolarrayAssign(cnf,Cp->head->literal,f);//��¼������
            simplySingleClause(cnf, Cp->head->literal, &st);//�����Ӿ䲢�����޸�
            if(isHaveEmptyClause(cnf)==TRUE){//����п��Ӿ�
                #if DEBUG2
                //printf("�����\n");
                //printClause(cnf);
                #endif // DEBUG2
                //���Ӿ����ö�
                int *a=(int*)malloc(sizeof(int)*cnf->literals);
                reduceChange(cnf, &st,-1, a);//��ԭ����ʼ��
                /*���ȡֵ������������Ҫ���ѧϰ�Ӿ䲢��ʱ�����...*/
                ret=createLearnClause(cnf,a+1,a[0]);
                free(a);
                backLearnClause(cnf,f);
                deleteRepeatLearnClause(cnf, cnf->learn_root);
                #if DEBUG2
                //getchar();
                printf("����%d\n",ret);
                #endif // DEBUG2
                #if DEBUG
                printf("����isHaveEmptyClause\n");
                #endif // DEBUG
                //��ֵ����
                backAssign(cnf,f);
                #if DEBUG2
                {
                    printf("����\n");
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
            else if(isHaveClause(cnf)==FALSE){//���û���Ӿ�
                reduceChange(cnf, &st,-1, NULL);//��ԭ����ʼ��
                //ѧϰ�Ӿ�Ļ���
                backLearnClause(cnf, f);
                return TRUE;
            }
        }//while
        l=combineStrategy(cnf);//�ۺϲ���ѡ����Ԫ
        /* VSIDS */
        addClause(cnf,1,&l);
        saveChange(&st,SPLIT,cnf->root,NULL);//����������
        s=DPLLRec(cnf,f+1);
        reduceChange(cnf,&st,1,NULL);
    }while(s==FALSE);
    if(s==TRUE){
        reduceChange(cnf,&st,-1,NULL);
        //ѧϰ�Ӿ�Ļ���
        backLearnClause(cnf,f);
        return TRUE;
    }
    //������ֵ������
    else{
        reduceChange(cnf,&st,-1,NULL);
        //ѧϰ�Ӿ�Ļ���
        backLearnClause(cnf,f);
        //��ֵ����
        backAssign(cnf,f);
        #if DEBUG2
        {
            int j;
            printf("���ڻ���%d\n",f);
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
    /* SΪ��ʽ��Ӧ���Ӿ伯���������㣬����TURE�����򷵻�FALSE. */
    int l;
    ClauseList *Cp;
    ChangeStack st;
    st.next=NULL;
    while((Cp=locateUnitClause(cnf))!=NULL) {//���Ӿ䴫��
        boolarrayAssign(cnf,Cp->head->literal,-1);//��¼������
        simplySingleClause(cnf, Cp->head->literal, &st);//�����Ӿ�
        if(isHaveEmptyClause(cnf)==TRUE){//����п��Ӿ�
            reduceChange(cnf,&st,-1,NULL);
            return FALSE;
        }
        else if(isHaveClause(cnf)==FALSE){//���û���Ӿ�
            reduceChange(cnf,&st,-1,NULL);
            return TRUE;
        }

    }//while
    l=combineStrategy(cnf);//�ۺϲ���ѡ����Ԫ
    boolarrayAssign(cnf,l,-1);//��¼������
    addClause(cnf,1,&l);//��ӵ��Ӿ伯��
    saveChange(&st,SPLIT,cnf->root,NULL);
    int s=DPLLRec2(cnf);//�ݹ�
    if(s==TRUE){//�������
        reduceChange(cnf,&st,-1,NULL);
        return TRUE;
    }
    reduceChange(cnf,&st,1,NULL);
    l=-l;//ȡ��ֵ
    boolarrayAssign(cnf,l,-1);//��¼������
    addClause(cnf,1,&l);//��ӵ��Ӿ伯��
    saveChange(&st,SPLIT,cnf->root,NULL);
    s=DPLLRec2(cnf);//�ݹ�
    reduceChange(cnf,&st,-1,NULL);
    return s;
}
boolean DPLLUnRec();//�ǵݹ�

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
            //���Ϊ�����,�ѿ�����ö�����
            //���Ϊ�����,�ö�
            //���Ϊ˫���,�ö�
            if(Cp->length<=2){
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
            }
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
status simplifyClause();//�����Ӿ�
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
//                index = i;//ȡ���ʲ�ƽ��,ȡ��
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
        //�ڻ����Ӿ�ʱ�Ѿ���length<=2���Ӿ�ȫ���ö���,�����ڵ�һ�εݹ�ʱ���ܲ�û��
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
        //�ڻ����Ӿ�ʱ�Ѿ���length<=2���Ӿ�ȫ���ö���,�����ڵ�һ�εݹ�ʱ���ܲ�û��
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
    if(count>10){
        count=0;
        for(i=1;i<=cnf->literals;i++){
            cnf->countarray[i]/=3;
        }
    }
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
    //return VSIDSStrategy(cnf);
    return cnf->root->head->literal;
}
status saveChange(ChangeStack *head, int tag, ClauseList* Cp, LiteralNode *Lp)
{
    //head�ǲ������ݵ�ͷ�ڵ�ָ��
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
        newChange->Cp=Cp;//�����Ӿ�
        break;
    case LITERAL:
        newChange->Cp=Cp;//���������Ӿ��ַ
        newChange->Lp=Lp;//�����Ԫ
        break;
    case SPLIT:
        newChange->Cp=Cp;//�����Ӿ�
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
    //learnarray���:��һ��Ԫ��Ϊ���ȣ��ڶ�����ʼ�Ǳ�Ԫ
    //ѧϰ�Ӿ�ƻ�
    //����a[]:��Ԫx�ڵ�ǰ��ĸ�ֵ״̬X=boolarray[x]*x����a[]
    //��һ�γ����϶��ǳ�ͻ���Ӿ�C��գ����Ե�һ�γ�������Ӿ�C��DIVIDE������б�Ԫ������a[]

    //1.����ͻ�ı�Ԫ����a[]_0

    //2.��i�γ������ȼ����Ӿ��Ƿ���Ԫ�أ������Ӿ��Ԫ��ֻ��һ��(tagΪCLAUSE)����Ԫ����a[]_0�ڣ�Ԫ�ص�ֵȡ������a[]_0�Ƚϣ������Ӿ�C��DIVIDE������б�Ԫ������a[]_0
    //�������Ӿ��Ԫ��ֻ��һ�������ǲ����̺���ϵ
    //����Ԫ����a[]_0�ڡ����Ƿ����ͻ�Ӿ��й�

    //3.�����г�����ɺ󣬵õ�a[]_1��ɾȥa[]_1�б������еı�����ֵ�������Լ��Ƶ������ı�Ԫ���õ�a[]_2
    //���㣺a[]_1�б�Ԫi��floorarray[i]�����ֵF

    //4.�õ���Ҫ���صľ��߲㣬��a[]_2�б�Ԫi��floorarray[i]�����ֵtoF

    //5.���뱾��ľ���X�õ�a[]
    //����X��a[]_1�б�Ԫi��floorarray[i]��ֵΪF��Ϊ���һ��(�ɺ���ǰ����)

    //6.����ѧϰ�Ӿ�Cl => a[]�õ�����̺���ϵ
    int i=1,j;
    LiteralNode *Lp;
    ChangeStack *Sp=head->next;
    while(Sp && time!=0){
        //PrintChangeStack(head);
        switch(Sp->tag)
        {
        case CLAUSE:
            insertClauseTop(cnf,Sp->Cp);
            break;
        case LITERAL:
            //�Ӿ��Ԫ��ѭ���ɣ�DIVIDE��ı�Ԫ���߲�Ϊ����
            //��������ı�Ԫɾ������ֻ��Ҫ�������߲�ı�Ԫ����DIVIDEǰ����
            backLiteral(Sp->Cp,Sp->Lp);
            if(learnarray){
                //����ͻ�ı�Ԫ����a[]_0
                if(Sp->Cp->length==1){
                    learnarray[i]=Sp->Cp->head->literal;
                    i++;
                    learnarray[i]=0-Sp->Cp->head->literal;
                    i++;
                    #if DEBUG2
                    printf("��ͻ��Cp=%p,l=%d\n",Sp->Cp,a[0]);
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

        if(learnarray){
            #if DEBUG2
//            printf("�Ӿ䣺");
//            for(Lp=Sp->Cp->head;Lp;Lp=Lp->next){
//                printf("%d ",Lp->literal);
//            }
//            printf("\n");
            #endif // DEBUG2

            if(Sp->Cp->length==1){
                for(j=1;j<i;j++){
                    if(Sp->Cp->head->literal==-learnarray[j]){
                        break;
                    }
                }
                //��Ԫ����a[]_0�ڣ�Ԫ�ص�ֵȡ������a[]_0�Ƚϣ������Ӿ�C��DIVIDE������б�Ԫ������a[]_0
                if(i!=j){
                    for(Lp=Sp->Cp->rmv;Lp;Lp=Lp->next){
                        //ɾȥ�ظ��ı�Ԫ
                        for(j=1;j<i;j++){
                            if(learnarray[j]==Lp->literal){
                                break;
                            }
                        }
                        if(j==i){
                            learnarray[i]=Lp->literal;
                            i++;
                        }
                    }
                }
                //����ǹ�����ѧϰ�Ӿ䣬��ô�ȼ���a[]����ɾ����������,��Ϊ������̵ĵ�������ǰ�������Ӿ䣩
                /*if(Sp->tag==CLAUSE && Sp->Cp->lcp && Sp->Cp->lcp->countNum>LEARNLENGTH_MAX){
                    printf("ɾ��%p\n",Sp->Cp);
                    deleteLearnClause(cnf,Sp->Cp->lcp);
                }*/
            }
        }
        time--;
        head->next=head->next->next;
        free(Sp);
        Sp=head->next;
    }
    if(learnarray){
        learnarray[0]=i-1;
    }
    return OK;
}
int createLearnClause(CNF *cnf, int *a, int i)
{
    int j,l;
    int X=cnf->root->head->literal;
    int F=0,toF=0;
    #if DEBUG2
    printf("ѧϰǰa=");
    for(j=0;j<i;j++){
        printf("%d ",a[j]);
    }
    printf("\n");
    printf("floor:\n");
    for(j=1;j<=cnf->literals;j++){
        printf("%d:%d ",j,cnf->floorarray[j]*cnf->boolarray[j]);
    }
    printf("\n");
    #endif // DEBUG2
    //�ɺ���ǰ���ٸ�ֵ����
    for(j=i-1;j>=0;j--){
        if(F<cnf->floorarray[ABS(a[j])]){
            F=cnf->floorarray[ABS(a[j])];
        }
    }
    //3.�����г�����ɺ󣬵õ�a[]_1��ɾȥa[]_1�б������еı�����ֵ�������Լ��Ƶ������ı�Ԫ���õ�a[]_2
    for(l=0,j=0;j<i;j++){
        if(cnf->floorarray[ABS(a[j])]!=F && cnf->floorarray[ABS(a[j])]!=-1){
            a[l]=a[j];
            l++;
        }
    }
    //4.�õ���Ҫ���صľ��߲㣬��a[]_2�б�Ԫi��floorarray[i]�����ֵtoF
    for(j=0;j<l;j++){
        if(toF<cnf->floorarray[ABS(a[j])]){
            toF=cnf->floorarray[ABS(a[j])];
        }
    }
    // **���ѧϰ�Ӿ䳤�ȹ�����ֻ���ɾ��ߵ��Ӿ䣬�ڻ��ݵ�ʱ����ɾ��
    int L=l+1;//��ѧϰ�Ӿ��ԭ��
    /*if(L>LEARNLENGTH_MAX){
        l=0;
    }*/

    //�����Ԫ��ʹ�ñ�Ԫ���߲�Ϊ����
    //��������
    for(i=1;i<l;i++){
        for(j=i-1;j>=0;j--){
            if(cnf->floorarray[ABS(a[j+1])]>cnf->floorarray[ABS(a[j])]){
                int tmp=a[j+1];
                a[j+1]=a[j];
                a[j]=tmp;
            }
        }
    }
    //5.���뱾��ľ���X�õ�a[]
    a[l]=-X;
    l++;
    for(j=l-1;j>0;j--){
        int tmp=a[j-1];
        a[j-1]=a[j];
        a[j]=tmp;
    }

    //6.����ѧϰ�Ӿ�Cl => a[]�õ�����̺���ϵ
    addClause(cnf,l,a);

    LearnClauseList *lcp=(LearnClauseList*)malloc(sizeof(LearnClauseList));
    lcp->isInStack=FALSE;
    lcp->floor=F;
    lcp->clause=cnf->root;
    lcp->countNum=L;
    lcp->next=cnf->learn_root;
    cnf->learn_root=lcp;
    //ѧϰ�Ӿ�ı�Ԫ�и�ֵ
    while(cnf->root->head){
        removeLiteral(cnf->root,cnf->root->head->literal);
    }
    #if DEBUG2
    printf("ѧϰ%p a = ",cnf->root);
    for(j=0;j<l;j++){
        printf("%d,%d ",a[j],cnf->floorarray[ABS(a[j])]);
    }
    printf("L=%d\n",L);
    #endif // DEBUG2
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
                //printf("ɾǰ\n");
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
                //printf("ɾ��\n");
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
            //�����Ѿ����뵽����ջ���Ӿ䲻��ɾ
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
    printf("���ڼ��: \n");
    for(Cp=cnf->root,i=1;Cp;Cp=Cp->next,i++){
        printf("��%d���Ӿ�: ",i);
        for(Lp=Cp->head,flag=0;Lp;Lp=Lp->next){
            b=cnf->boolarray[(ABS(Lp->literal))];
            if(EVALUE(Lp->literal)==b){
                printf("��");
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
    printf("��ӡChangeջ\n");
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
