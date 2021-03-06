#include "clause.h"

status createCNF(CNF *cnf, int literals, int clauses)
{
    cnf->literals = literals;
    cnf->clauses = clauses;
    cnf->root = NULL;
    cnf->learn_root = NULL;
    cnf->boolarray = (boolean *)malloc((cnf->literals + 1) * sizeof(boolean));
    cnf->floorarray = (int *)malloc((cnf->literals + 1) * sizeof(int));
    cnf->countarray = (int *)malloc((cnf->literals * 2 + 1) * sizeof(int));
    cnf->lindex = NULL;
    if (cnf->boolarray && cnf->floorarray) {
        clearCnfAnswer(cnf);
        return OK;
    } else {
        return ERROR;
    }
}
status destroyCNF(CNF *cnf)
{
    if (!cnf) {
        return ERROR;
    }
    deleteAllClause(cnf);
    deleteALLIndex(cnf);
    free(cnf->boolarray);
    free(cnf->floorarray);
    free(cnf->countarray);
    free(cnf->lindex);
    free(cnf);
    return OK;
}
status clearCnfAnswer(CNF *cnf)
{
    LearnClauseList *Lcp = cnf->learn_root;
    while (Lcp) {
        deleteClauseLIndex(cnf, Lcp->clause);
        deleteLearnClause(cnf, Lcp);
        Lcp = cnf->learn_root;
    }
    int i;
    for (i = 0; i <= cnf->literals; i++) {
        cnf->boolarray[i] = NOTSURE;
        cnf->floorarray[i] = -1;
        cnf->countarray[i] = cnf->countarray[i + cnf->literals] = 0;
    }

    return OK;
}
status addClause(CNF *cnf, int n, int *clause)
{
    int i;
    ClauseList *newClause = (ClauseList *)malloc(sizeof(ClauseList));
    if (!newClause || n <= 0) {
        return ERROR;
    }
    newClause->head = NULL;
    newClause->rmv = NULL;
    newClause->length = 0;
    newClause->weight = 1;
    newClause->isremoved = FALSE;
    for (i = 0; i < n; i++) {
        if (!addLiteral(newClause, clause[i])) {
            return ERROR;
        }
    }
    if (cnf->root) {
        cnf->root->prev = newClause;
    }
    newClause->next = cnf->root;
    newClause->prev = NULL;

    cnf->root = newClause;
    cnf->clauses++;

    return OK;
}
status createLIndex(CNF *cnf)
{
    if (cnf->clauses == 0 || cnf->literals == 0) {
        return FALSE;
    }
    if (cnf->lindex != NULL) {
        deleteALLIndex(cnf);
    } else {
        cnf->lindex = (LiteralIndex *)malloc((cnf->literals + 1) * sizeof(LiteralIndex));
    }
    int i;
    ClauseList *Cp;
    for (i = 1; i <= cnf->literals; i++) {
        cnf->lindex[i].next = NULL;
    }
    for (Cp = cnf->root; Cp; Cp = Cp->next) {
        addLIndex(cnf, Cp);
    }
    return OK;
}
status addLIndex(CNF *cnf, ClauseList *Cp)
{
    if (cnf->lindex == NULL) {
        return FALSE;
    }
    LiteralNode *Lp;
    LiteralIndex *li;
    for (Lp = Cp->head; Lp; Lp = Lp->next) {
        li = (LiteralIndex *)malloc(sizeof(LiteralIndex));
        li->Lp = Lp;
        li->Cp = Cp;
        li->next = cnf->lindex[ABS(Lp->literal)].next;
        cnf->lindex[ABS(Lp->literal)].next = li;
    }
    return OK;
}
status insertClauseTop(CNF *cnf, ClauseList *Cp)
{
    if (cnf->root) {
        cnf->root->prev = Cp;
    }
    Cp->isremoved = FALSE;
    Cp->next = cnf->root;
    Cp->prev = NULL;
    cnf->root = Cp;
    cnf->clauses++;
    return OK;
}
ClauseList *removeClause(CNF *cnf, ClauseList *Cp)
{
    ClauseList *p = Cp->prev;
    if (Cp->next) {
        Cp->next->prev = p;
    }
    if (p) {
        p->next = Cp->next;
    } else {
        cnf->root = Cp->next;
    }
    Cp->isremoved = TRUE;
    cnf->clauses--;
    return Cp;
}
status deleteClause(CNF *cnf, ClauseList *Cp)
{
    LiteralNode *lp;
    ClauseList *p = Cp->prev;
    if (Cp->next) {
        Cp->next->prev = p;
    }
    if (p) {
        p->next = Cp->next;
    } else {
        cnf->root = Cp->next;
    }
    for (lp = Cp->head; lp; lp = Cp->head) {
        Cp->head = lp->next;
        free(lp);
    }
    for (lp = Cp->rmv; lp; lp = Cp->rmv) {
        Cp->rmv = lp->next;
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
    for (p = cnf->root; p; p = cnf->root) {
        deleteClause(cnf, p);
    }
    for (lcp = cnf->learn_root; lcp; lcp = cnf->learn_root) {
        cnf->learn_root = lcp->next;
        free(lcp);
    }
    return OK;
}
status addLiteral(ClauseList *Cp, int literal)
{
    LiteralNode *p = (LiteralNode *)malloc(sizeof(LiteralNode));
    if (!p) {
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
    LearnClauseList *lcq = cnf->learn_root;
    if (Lcp == cnf->learn_root) {
        cnf->learn_root = Lcp->next;
    } else {
        for (lcq = cnf->learn_root; lcq; lcq = lcq->next) {
            if (lcq->next == Lcp) {
                lcq->next = Lcp->next;
                break;
            }
        }
    }
    deleteClause(cnf, Lcp->clause);
    free(Lcp);
    return OK;
}
LiteralNode *removeLiteral(ClauseList *Cp, int literal)
{
    LiteralNode *p, *q = NULL;
    for (p = Cp->head; p; p = p->next) {
        if (p->literal == literal) {
            Cp->length--;
            if (!q) {
                Cp->head = p->next;
            } else {
                q->next = p->next;
            }
            p->next = Cp->rmv;
            Cp->rmv = p;
            return p;
        }
        q = p;
    }
    return NULL;
}
status backLiteral(ClauseList *Cp, LiteralNode *Lp)
{
    LiteralNode *Lq = NULL;
    if (Cp->rmv == Lp) {
        Cp->rmv = Lp->next;
    } else {
        for (Lq = Cp->rmv; Lq && Lq->next != Lp; Lq = Lq->next) {
            ;
        }
        if (Lq == NULL) {
            return ERROR;
        }
        Lq->next = Lp->next;
    }
    Lp->next = Cp->head;
    Cp->head = Lp;
    Cp->length++;
    return OK;
}
status deleteLiteral(ClauseList *Cp, int literal)
{
    LiteralNode *p, *q = NULL;
    for (p = Cp->head; p; p = p->next) {
        if (p->literal == literal) {
            Cp->length--;
            if (!q) {
                Cp->head = p->next;
            } else {
                q->next = p->next;
            }
            free(p);
            return OK;
        }
        q = p;
    }
    return ERROR;
}
status deleteClauseLIndex(CNF *cnf, ClauseList *Cp)
{
    LiteralNode *Lp;
    LiteralIndex *li;
    LiteralIndex *tmp;
    for (Lp = Cp->head; Lp; Lp = Lp->next) {
        for (li = &(cnf->lindex[ABS(Lp->literal)]); li->next->Cp != Cp; li = li->next) {
            ;
        }
        tmp = li->next;
        li->next = tmp->next;
        free(tmp);
    }
    for (Lp = Cp->rmv; Lp; Lp = Lp->next) {
        for (li = &(cnf->lindex[ABS(Lp->literal)]); li->next->Cp != Cp; li = li->next) {
            ;
        }
        tmp = li->next;
        li->next = tmp->next;
        free(tmp);
    }
    return OK;
}
status deleteALLIndex(CNF *cnf)
{
    int i;
    if (cnf->lindex == NULL) {
        return OK;
    }
    for (i = 1; i <= cnf->literals; i++) {
        while (cnf->lindex[i].next) {
            LiteralIndex *li = cnf->lindex[i].next;
            cnf->lindex[i].next = li->next;
            free(li);
        }
    }
    return OK;
}
status boolarrayAssign(CNF *cnf, int literal, int f)
{
    cnf->boolarray[ABS(literal)] = EVALUE(literal);
    cnf->floorarray[ABS(literal)] = f;
    return OK;
}
status printClause(CNF *cnf)
{
    printf("------------------------------------------------\n");
    printf("literals: %d\n", cnf->literals);
    printf("clauses: %d\n", cnf->clauses);
    printf("boolarray: 0x%p\n", cnf->boolarray);
    int i;
    for (i = 1; i <= cnf->literals; i++) {
        printf("%d:%d, ", i, cnf->boolarray[i]);
        if (i % 10 == 0) {
            printf("\n");
        }
    }
    printf("floorarray: 0x%p\n", cnf->floorarray);
    for (i = 1; i <= cnf->literals; i++) {
        printf("%d:%d, ", i, cnf->floorarray[i]);
        if (i % 10 == 0) {
            printf("\n");
        }
    }
    ClauseList *Cp;
    LiteralNode *Lp;
    printf("\n");

    printf("clause:\n");
    for (Cp = cnf->root, i = 1; Cp; Cp = Cp->next, i++) {
        printf("%d:%p length: %d prev:%p next:%p\n", i, Cp, Cp->length, Cp->prev, Cp->next);
        for (Lp = Cp->head; Lp; Lp = Lp->next) {
            printf("%d ", Lp->literal);
        }
        if (Cp->rmv) {
            printf("|| ");
            for (Lp = Cp->rmv; Lp; Lp = Lp->next) {
                printf("%d ", Lp->literal);
            }
        }

        printf("\n");
    }
    return OK;
}
status printLearnClause(CNF *cnf)
{
    printf("------------------------------------------------\n");
    printf("learn clause:\n");
    int i;
    LearnClauseList *lcp;
    ClauseList *Cp;
    LiteralNode *Lp;
    for (lcp = cnf->learn_root, i = 1; lcp; lcp = lcp->next, i++) {
        Cp = lcp->clause;
        printf("%d:%p length: %d isInStack:%s ���߲�:%d\n", i, Cp, Cp->length, (lcp->isInStack == TRUE) ? "TRUE" : "FALSE", lcp->floor);
        for (Lp = Cp->head; Lp; Lp = Lp->next) {
            printf("%d ", Lp->literal);
        }
        if (Cp->rmv) {
            printf("|| ");
            for (Lp = Cp->rmv; Lp; Lp = Lp->next) {
                printf("%d ", Lp->literal);
            }
        }
        printf("\n");
    }
    return OK;
}
status printLIndex(CNF *cnf)
{
    int i;
    LiteralIndex *li;
    printf("������\n");
    for (i = 1; i <= cnf->literals; i++) {
        printf("%d: ", i);
        for (li = cnf->lindex[i].next; li; li = li->next) {
            printf("%p ", li->Cp);
        }
        printf("\n");
    }
    return OK;
}
CNF *copyCNF(CNF *cnf)
{
    int i;
    ClauseList *Cp, *Cpp;
    LiteralNode *Lp, *Lpp;
    CNF *newCNF = (CNF *)malloc(sizeof(CNF));
    if (!newCNF) {
        return NULL;
    }
    newCNF->clauses = cnf->clauses;
    newCNF->literals = cnf->literals;
    newCNF->boolarray = (boolean *)malloc(cnf->literals * sizeof(boolean));
    newCNF->floorarray = (int *)malloc(cnf->literals * sizeof(boolean));
    newCNF->root = NULL;
    newCNF->learn_root = NULL;
    if (!newCNF) {
        return NULL;
    }
    for (i = 1; i <= newCNF->literals; i++) {
        newCNF->boolarray[i] = cnf->boolarray[i];
        newCNF->floorarray[i] = cnf->floorarray[i];
    }
    for (Cp = cnf->root; Cp; Cp = Cp->next) {
        Cpp = (ClauseList *)malloc(sizeof(ClauseList));
        if (!Cpp) {
            return NULL;
        }
        Cpp->length = Cp->length;
        Cpp->head = NULL;
        Cpp->rmv = NULL;
        for (Lp = Cp->head; Lp; Lp = Lp->next) {
            Lpp = (LiteralNode *)malloc(sizeof(LiteralNode));
            if (!Lpp) {
                return NULL;
            }
            Lpp->literal = Lp->literal;
            Lpp->next = Cpp->head;
            Cpp->head = Lpp;
        }
        for (Lp = Cp->rmv; Lp; Lp = Lp->next) {
            Lpp = (LiteralNode *)malloc(sizeof(LiteralNode));
            if (!Lpp) {
                return NULL;
            }
            Lpp->literal = Lp->literal;
            Lpp->next = Cpp->head;
            Cpp->rmv = Lpp;
        }
        Cpp->prev = NULL;
        Cpp->next = newCNF->root;
        if (Cpp->next) {
            Cpp->next->prev = Cpp;
        }
        newCNF->root = Cpp;
    }
    return newCNF;
}
status pasteCNF(CNF *oldCNF, CNF *newCNF)
{
    deleteAllClause(oldCNF);
    oldCNF->literals = newCNF->literals;
    oldCNF->clauses = newCNF->clauses;
    oldCNF->root = newCNF->root;
    oldCNF->learn_root = newCNF->learn_root;
    return OK;
}
boolean isClauseEmpty(CNF *cnf)
{
    return (cnf->clauses) ? FALSE : TRUE;
}
boolean isUnitClause(ClauseList *Cp)
{
    return (Cp->length == 1) ? TRUE : FALSE;
}
ClauseList *locateUnitClause(CNF *cnf)
{
    ClauseList *p;
    for (p = cnf->root; p; p = p->next) {
        if (isUnitClause(p) == TRUE) {
            return p;
        }
    }
    return NULL;
}
boolean isHaveEmptyClause(CNF *cnf)
{
    ClauseList *p;
    for (p = cnf->root; p; p = p->next) {
        if (!(p->length)) {
            return TRUE;
        }
    }
    return FALSE;
}
boolean isHaveClause(CNF *cnf)
{
    return (cnf->clauses) ? TRUE : FALSE;
}
boolean evaluateClause(ClauseList *Cp, int literal)
{
    int flag = NOTCONTAIN;
    LiteralNode *p;
    for (p = Cp->head; p; p = p->next) {
        if (literal == p->literal) {
            return TRUE;
        } else if (-literal == p->literal) {
            flag = NOTSURE;
        }
    }
    return flag;
}
