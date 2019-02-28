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
    /* SΪ��ʽ��Ӧ���Ӿ伯���������㣬����TURE�����򷵻�FALSE. */
    int s,l;
    int count=0;
    int ret=f;//���ݲ���
    ClauseList *Cp;
    ChangeStack st;
    st.floor=f;
    st.next=NULL;
    //VSIDS��ÿ����������һ������������������ĳ���Ӿ��г��֣�����ԭʼ�����е��Ӿ��ѧϰ�Ӿ䣬�����ֶ�Ӧ�ļ������ͼ�1
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
            /*��������������԰���Щû���Ƴ���Ԫ�ġ��ܳ���ѧϰ�Ӿ�ɾ��*/
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
        while((Cp=locateUnitClause(cnf))!=NULL) {//���Ӿ䴫��
            boolarrayAssign(cnf,Cp->head->literal,f);//��¼������
            simplifySingleClause_2(cnf, Cp, &st);//�����Ӿ䲢�����޸�
            if(isHaveEmptyClause(cnf)==TRUE) { //����п��Ӿ�
                if(clock()-startTime>=OUTTIME) {
                    printf("��ʱ >%dms\n",OUTTIME);
                    return -1-f;
                }
                int *a=(int*)malloc(sizeof(int)*(cnf->literals+3));
                reduceChange(cnf, &st,-1, a);//��ԭ����ʼ��
                /*���ȡֵ������������Ҫ���ѧϰ�Ӿ䲢��ʱ�����...*/
                ret=a[0];
                free(a);
                backLearnClause(cnf,f);
                //��ֵ����
                backAssign(cnf,f);
                if(count>MAXC) {
                    MAXC++;
                    //printf("������...\n");
                    reset=TRUE;
                    //ѧϰ�Ӿ�Ļ���
                    return 0-f;
                }
                return ret-f;
            } else if(isHaveClause(cnf)==FALSE) { //���û���Ӿ�
                reduceChange(cnf, &st,-1, NULL);//��ԭ����ʼ��
                //ѧϰ�Ӿ�Ļ���
                backLearnClause(cnf, f);
                return TRUE;
            }
        }//while
        /* VSIDS */
        l=combineStrategy(cnf,mom);//�ۺϲ���ѡ����Ԫ

        addClause(cnf,1,&l);
        saveChange(&st,SPLIT,cnf->root,NULL);//����������

        //printf("�ݹ�:%d, ѡ%d\n",f,l);

        s=DPLLRec(cnf,f+1);
        reduceChange(cnf,&st,1,NULL);
    } while(s==FALSE);
    if(s==TRUE) {
        reduceChange(cnf,&st,-1,NULL);
        //ѧϰ�Ӿ�Ļ���
        backLearnClause(cnf,f);
        return TRUE;
    }
    //������ֵ������
    else {
        reduceChange(cnf,&st,-1,NULL);
        //ѧϰ�Ӿ�Ļ���
        backLearnClause(cnf,f);
        //��ֵ����
        backAssign(cnf,f);
        return s+1;
    }
}
boolean DPLLRec2(CNF *cnf, const int f)
{
    /* SΪ��ʽ��Ӧ���Ӿ伯���������㣬����TURE�����򷵻�FALSE. */
    int l;
    ClauseList *Cp;
    ChangeStack st;
    st.next=NULL;
    if(f==0) {
        startTime=clock();
    }
    while((Cp=locateUnitClause(cnf))!=NULL) {//���Ӿ䴫��
        boolarrayAssign(cnf,Cp->head->literal,f);//��¼������
        simplySingleClause(cnf, Cp->head->literal, &st);//�����Ӿ�
        if(isHaveEmptyClause(cnf)==TRUE) { //����п��Ӿ�
            reduceChange(cnf,&st,-1,NULL);
            backAssign(cnf,f);
            if(clock()-startTime>=OUTTIME) {
                printf("��ʱ >%dms\n",OUTTIME);
                return NOTSURE;
            }
            return FALSE;
        } else if(isHaveClause(cnf)==FALSE) { //���û���Ӿ�
            reduceChange(cnf,&st,-1,NULL);
            return TRUE;
        }
    }//while
    l=combineStrategy(cnf,mom);//�ۺϲ���ѡ����Ԫ
    boolarrayAssign(cnf,l,f);//��¼������
    addClause(cnf,1,&l);//��ӵ��Ӿ伯��
    saveChange(&st,SPLIT,cnf->root,NULL);
    int s=DPLLRec2(cnf, f+1);//�ݹ�
    if(s==TRUE) { //�������
        reduceChange(cnf,&st,-1,NULL);
        return TRUE;
    }
    if(s==NOTSURE) {
        reduceChange(cnf,&st,-1,NULL);
        backAssign(cnf,f);
        return NOTSURE;
    }
    reduceChange(cnf,&st,1,NULL);
    l=-l;//ȡ��ֵ
    boolarrayAssign(cnf,l,f);//��¼������
    addClause(cnf,1,&l);//��ӵ��Ӿ伯��
    saveChange(&st,SPLIT,cnf->root,NULL);
    s=DPLLRec2(cnf, f+1);//�ݹ�
    reduceChange(cnf,&st,-1,NULL);
    if(s==FALSE) {
        backAssign(cnf,f);
    }
    return s;
}
boolean DPLLUnRec();//�ǵݹ�

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
            //���Ϊ�����,�ѿ�����ö�����
            //���Ϊ�����,�ö�
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
    //���ڲ���ĵ��Ӿ�û�м�����������������ɾ��
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
                //���Ϊ�����,�ѿ�����ö�����
                //���Ϊ�����,�ö�
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
    //head�ǲ������ݵ�ͷ�ڵ�ָ��
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
    //learnarray���:��0��Ԫ��Ϊ���ȣ���1����ʼ�Ǳ�Ԫ,��i+1��ΪΨһ�̺���
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
            //�Ӿ��Ԫ��ѭ���ɣ�DIVIDE��ı�Ԫ���߲�Ϊ����
            //��������ı�Ԫɾ������ֻ��Ҫ�������߲�ı�Ԫ����DIVIDEǰ����
            backLiteral(Sp->Cp,Sp->Lp);
            if(learnarray && flag==FALSE) {
                //����ͻ�ı�Ԫ����a[]_0
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
                //�Ƿ�Ԫ����a[]_0�ڣ�Ԫ�ص�ֵȡ������a[]_0�Ƚϣ�
                //������learnarray[i]=0-Sp->Cp->head->literal
                learnarray[i]=0-Sp->Cp->head->literal;
                for(j=1; Sp->Cp->head->literal!=-learnarray[j]; j++) {
                    ;
                }
                if(j!=i) {
                    //���ʸö���
                    G[j]=TRUE;
                    int t=j;
                    //�Ƿ�ȫ�����ʹ�
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
                            //ɾȥ�ظ��ı�Ԫ
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
    //�ɺ���ǰ���ٸ�ֵ����
    for(j=i-1; j>=0; j--) {
        if(F<cnf->floorarray[ABS(copy[j])]) {
            F=cnf->floorarray[ABS(copy[j])];
        }
    }
    //3.�����г�����ɺ󣬵õ�a[]_1��ɾȥa[]_1�б������еı�����ֵ�������Լ��Ƶ������ı�Ԫ���õ�a[]_2
    for(l=0,j=0; j<i; j++) {
        if(cnf->floorarray[ABS(copy[j])]!=F) {
            copy[l]=copy[j];
            l++;
        }
    }
    //4.�õ���Ҫ���صľ��߲㣬��a[]_2�б�Ԫi��floorarray[i]�����ֵtoF
    for(j=0; j<l; j++) {
        if(toF<cnf->floorarray[ABS(copy[j])]) {
            toF=cnf->floorarray[ABS(copy[j])];
        }
    }
    // **���ѧϰ�Ӿ䳤�ȹ�����ֻ���ɾ��ߵ��Ӿ䣬�ڻ��ݵ�ʱ����ɾ��
    int L=l+1;//��ѧϰ�Ӿ��ԭ��

    //�����Ԫ��ʹ�ñ�Ԫ���߲�Ϊ����
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

    //5.���뱾��ľ���X�õ�a[]
    copy[l]=-X;
    l++;
    int tmp;
    for(j=l-1; j>0; j--) {
        tmp=copy[j-1];
        copy[j-1]=copy[j];
        copy[j]=tmp;
    }

    //6.����ѧϰ�Ӿ�Cl => a[]�õ�����̺���ϵ
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

    //���ѧϰ�Ӿ������
    addLIndex(cnf,lcp->clause);

    //ѧϰ�Ӿ�ı�Ԫ�и�ֵ
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
    printf("���ڼ��: \n");
    for(Cp=cnf->root,i=1; Cp; Cp=Cp->next,i++) {
        printf("��%d���Ӿ�: ",i);
        for(Lp=Cp->head,flag=0; Lp; Lp=Lp->next) {
            b=cnf->boolarray[(ABS(Lp->literal))];
            if(EVALUE(Lp->literal)==b) {
                printf("��");
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
    printf("��ӡChangeջ\n");
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
