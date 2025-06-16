#include<stdio.h>
#include<stdlib.h>
typedef int ElemType;
typedef struct AVLNode{
    ElemType data;
    int bf;
    struct AVLNode *lchild,*rchild;
}*AVLTree,AVLNode;
typedef enum{FALSE,TRUE}Boolean;
void L_Rotate(AVLTree *T){
    AVLTree rc;
    rc = (*T)->rchild;
    (*T)->rchild = rc->lchild;
    rc->lchild = *T;
    *T = rc;
}

void R_Rotate(AVLTree *T){
    AVLTree lc;
    lc = (*T)->lchild;
    (*T)->lchild = lc->rchild;
    lc->rchild = (*T);
    *T = lc;
}

void LeftBalance(AVLTree *T){
    AVLTree root = *T;
    AVLTree lc = root->lchild;
    int bf = lc->bf;
    switch(bf){
        case 1: 
            R_Rotate(T);
            root->bf = lc->bf = 0;
            break;
        case -1: 
            AVLTree rc = lc->rchild;
            if(rc->bf == 0){    
                L_Rotate(&root->lchild);
                R_Rotate(T);
                root->bf = 0;
                lc->bf = 0;
                rc->bf = 0;
            }else if(rc->bf == 1){ 
                L_Rotate(&root->lchild);
                R_Rotate(T);
                root->bf = -1;
                lc->bf = 0;
                rc->bf = 0;
                break;
            }else if(rc->bf == -1){
                L_Rotate(&root->lchild);
                R_Rotate(T);
                root->bf = 0;
                lc->bf = 1;
                rc->bf = 0;
            }
    }
}

void RightBalance(AVLTree *T){
    AVLTree root = *T;
    AVLTree rc = root->rchild;
    int bf = rc->bf;
    switch(bf){
        case -1: 
            L_Rotate(T);
            root->bf = rc->bf = 0;
            break;
        case 1:
            AVLTree lc = rc->lchild;
            if(lc->bf == 0){    
                R_Rotate(&root->rchild);
                L_Rotate(T);
                root->bf = 0;
                rc->bf = 0;
                lc->bf = 0;
            }else if(lc->bf == -1){ 
                R_Rotate(&root->rchild);
                L_Rotate(T);
                root->bf = 1;
                rc->bf = 0;
                lc->bf = 0;
            }else if(lc->bf == 1){
                R_Rotate(&root->rchild);
                L_Rotate(T);
                root->bf = 0;
                rc->bf = -1;
                lc->bf = 0;
            }
    }
}

int InsertAVL(AVLTree *T,ElemType e,Boolean *taller){
    if(!(*T)){
        *T = (AVLTree)malloc(sizeof(AVLNode));
        if(!(*T)) return 0; 
        (*T)->data = e;
        (*T)->lchild = (*T)->rchild = NULL;
        *taller = TRUE; 
        (*T)->bf = 0; 
        return 1; 
    }else if(e == (*T)->data){
        *taller = FALSE; 
        return 0;
    }else if(e < (*T)->data){
        if(!InsertAVL(&((*T)->lchild), e, taller)) return 0;
        if(*taller){
            switch((*T)->bf){
                case -1:
                    (*T)->bf = 0;
                    *taller = FALSE;
                    break;
                case 0:
                    (*T)->bf = 1;
                    *taller = TRUE;
                    break;
                case 1:
                    LeftBalance(T);
                    *taller = FALSE;
                    break;
            }
        }
    }else{
        if(!InsertAVL(&((*T)->rchild), e, taller)) return 0;
        if(*taller){
            switch((*T)->bf){
                case 1:
                    (*T)->bf = 0;
                    *taller = FALSE;
                    break;
                case 0:
                    (*T)->bf = -1;
                    *taller = TRUE;
                    break;
                case -1:
                    RightBalance(T);
                    *taller = FALSE;
                    break;
            }
        }
    }
    return 1;
}

void PriTree(AVLTree T,int level){
    if(T){
        for(int i = 0; i < level; i++) printf("-");
        if(T->data < 0) printf("(%d)\n", T->data);
        else  printf("%d\n", T->data);
        PriTree(T->lchild, level+1);
        PriTree(T->rchild, level+1);
    }
}

int SearchAVL(AVLTree T, ElemType e){
    if(!T) return 0; 
    if(e == T->data) return 1; 
    else if(e < T->data) return SearchAVL(T->lchild, e);
    else return SearchAVL(T->rchild, e);
}

int DealShorter(AVLTree *T, Boolean *shorter,int dir){
    if(!(*T)) return 0; 
    if(dir == 0){ //左子树变短
        switch((*T)->bf){
            case 1:
                (*T)->bf = 0;
                *shorter = TRUE;
                break;
            case 0:
                (*T)->bf = -1;
                *shorter = FALSE;
                break;
            case -1:
                AVLTree rc = (*T)->rchild;
                AVLTree root = *T;
                switch (rc->bf){
                    case 0:
                        L_Rotate(T);
                        root->bf = -1;
                        rc->bf = 1;
                    case -1:
                        L_Rotate(T);
                        root->bf = 0;
                        rc->bf = 0;
                        *shorter = TRUE;
                        break;
                    case 1:
                        int bf = rc->lchild->bf;
                        R_Rotate(&((*T)->rchild));
                        L_Rotate(T);
                        if(bf == 0){
                            root->bf = 0;
                            rc->bf = 0;
                        }else if(bf == 1){
                            rc->bf = -1;
                            root->bf = 0;
                        }else if(bf == -1){
                            rc->bf = 0;
                            root->bf = 1;
                        }
                        *shorter = TRUE;
                        break; 
                }
        }
    }else{ //右子树变短
        switch((*T)->bf){
            case -1:
                (*T)->bf = 0;
                *shorter = TRUE;
                break;
            case 0:
                (*T)->bf = 1;
                *shorter = FALSE;
                break;
            case 1:
                AVLTree lc = (*T)->lchild;
                AVLTree root = *T;
                switch (lc->bf){
                    case 0:
                        R_Rotate(T);
                        root->bf = 1;
                        lc->bf = -1;
                        *shorter = FALSE;
                        break;
                    case 1:
                        R_Rotate(T);
                        root->bf = 0;
                        lc->bf = 0;
                        *shorter = TRUE;
                        break;
                    case -1:
                        int bf = lc->rchild->bf;
                        L_Rotate(&((*T)->lchild));
                        R_Rotate(T);
                        if(bf == 0){
                            lc->bf = 0;
                            root->bf = 0;
                        }else if(bf == 1){
                            lc->bf = 0;
                            root->bf = -1;
                        }else if(bf == -1){
                            lc->bf = 1;
                            root->bf = 0;
                        }
                        *shorter = TRUE;
                        break;
                    }
                }
                
    }
}


int DeleteAVL(AVLTree *T, ElemType e, Boolean *shorter){
    if(!(*T)) return 0; 
    if(e == (*T)->data){
        if(!(*T)->lchild ||!(*T)->rchild){
            *T = (*T)->lchild ? (*T)->lchild : (*T)->rchild;
            *shorter = TRUE;
        }else{
            AVLTree pre = (*T)->lchild;
            while(pre->rchild) pre = pre->rchild;  //找前驱节点
            (*T)->data = pre->data;  
            if(!DeleteAVL(&((*T)->lchild), pre->data, shorter)) return 0;
            if(*shorter){
                DealShorter(T, shorter, 0); //左子树变短
            }
        }
    }else if(e < (*T)->data){
        if(!DeleteAVL(&((*T)->lchild), e, shorter)) return 0;
        if(*shorter){
            DealShorter(T, shorter, 0); //左子树变短
        }
    }else{
        if(!DeleteAVL(&((*T)->rchild), e, shorter)) return 0;
        if(*shorter){
            DealShorter(T, shorter, 1); //右子树变短
        }
    }
    return 1;           
}

typedef struct {
    ElemType *base;
    int size;
    int length;
} Sqlist;
int InitList(Sqlist *L){
    L->base = (ElemType *)malloc(100 * sizeof(ElemType));
    if(!L->base) return 0;
    L->size = 100;
    L->length = 0;
    return 1;
}
int InsertList(Sqlist *L, ElemType e){
    if(L->length >= L->size) L->base = (ElemType *)realloc(L->base, (L->size + 30) * sizeof(ElemType));
    if(!L->base) return 0;
    L->base[L->length++] = e;
    return 1;
}

void SortList(Sqlist *L,int part1,int part2){
    ElemType *temp = (ElemType *)malloc((part2 + part1) * sizeof(ElemType));
    if(!temp) return;
    int i=0,j=0;
    while(i < part1 && j < part2){
        if(L->base[i] < L->base[j + part1]){
            temp[i + j] = L->base[i];
            i++;
        }else{
            temp[i + j] = L->base[j + part1];
            j++;
        }
    }
    while(i < part1){
        temp[i + j] = L->base[i];
        i++;
    }   
    while(j < part2){
        temp[i + j] = L->base[j + part1];
        j++;
    }   
    for(int k = 0; k < part1 + part2; k++){
        L->base[k] = temp[k];
    }
    free(temp);
}

int CreateAVL(Sqlist *L, AVLTree *T,int low,int high){
    if(low > high) return 0; 
    int mid = (low + high) / 2;
    AVLTree node = (AVLTree)malloc(sizeof(AVLNode));
    if(!node) return 0;
    node->data = L->base[mid];
    node->lchild = node->rchild = NULL;
    int leftHeight = 0, rightHeight = 0;
    *T = node;
    leftHeight = CreateAVL(L, &((*T)->lchild), low, mid - 1);
    rightHeight = CreateAVL(L, &((*T)->rchild), mid + 1, high);
    if(leftHeight > rightHeight){
        (*T)->bf = 1; 
        return leftHeight + 1; 
    }else if(leftHeight < rightHeight){
        (*T)->bf = -1; 
        return rightHeight + 1; 
    }else{
        (*T)->bf = 0; 
        return leftHeight + 1; 
    }
}

void TraverseAVL(AVLTree T, Sqlist *L){
    if(!T) return;
    TraverseAVL(T->lchild, L);
    InsertList(L, T->data);
    TraverseAVL(T->rchild, L);
}

AVLTree MergeAVL(AVLTree *T1,AVLTree *T2){
    if(!(*T1) && !(*T2)) return NULL;
    if(!(*T1)) return *T2;
    if(!(*T2)) return *T1;
    AVLTree T = NULL;
    Sqlist L;
    if(!InitList(&L)) return 0;
    int part1 = 0, part2 = 0;
    TraverseAVL(*T1, &L);
    part1 = L.length;
    TraverseAVL(*T2, &L);
    part2 = L.length - part1;
    SortList(&L, part1, part2);
    if(!CreateAVL(&L, &T, 0, L.length - 1)){
        free(L.base);
        return NULL;
    }
    free(L.base);
    return T;
} //这种方式应该是达到了O(n)的时间复杂度
 
int DivAVL(AVLTree T, AVLTree *T1, AVLTree *T2, ElemType e){
    if(!T) return 0; 
    Sqlist L;
    if(!InitList(&L)) return 0;
    TraverseAVL(T, &L);
    int i;
    for(i = 0; i < L.length; i++){
        if(L.base[i] >= e) break;
    }
    if(i == L.length || L.base[i] != e) {
        free(L.base);
        return 0;
    }
    CreateAVL(&L, T1, 0, i);
    CreateAVL(&L, T2, i + 1, L.length - 1);   
    free(L.base);
    return 1;
}



int main(){
    AVLTree T = NULL;
    Boolean taller;
    printf("Please give the type of operation:\n");
    printf("1. Search\n");
    printf("2. Insert\n");
    printf("3. Delete\n");
    printf("4. Print\n");
    printf("5. DivTree\n");
    printf("6. Exit\n");

    printf("=============================\n");
    int choice;
    while(1){
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if(choice == 2){
            ElemType e;
            printf("Enter the element to insert: ");
            scanf("%d", &e);
            if(InsertAVL(&T, e, &taller)){
                printf("Inserted %d successfully.\n", e);
            } else {
                printf("Insertion failed for %d.\n", e);
            }
        } else if(choice == 4){
            PriTree(T, 0);
        } else if(choice == 1){
            ElemType e;
            printf("Enter the element to search: ");
            scanf("%d", &e);
            if(SearchAVL(T, e)){
                printf("Element %d found in the AVL tree.\n", e);
            } else {
                printf("Element %d not found in the AVL tree.\n", e);
            }
        } else if(choice == 3){
            ElemType e;
            printf("Enter the element to delete: ");
            scanf("%d", &e);
            Boolean shorter = FALSE;
            if(DeleteAVL(&T, e, &shorter)){
                printf("Deleted %d successfully.\n", e);
            } else {
                printf("Deletion failed for %d.\n", e);
            }
        } else if(choice == 5){
            AVLTree T1 = NULL, T2 = NULL;
            ElemType e;
            printf("Enter the element to divide the tree: ");
            scanf("%d", &e);
            if(DivAVL(T, &T1, &T2, e)){
                printf("Divided the tree at %d successfully.\n", e);
                printf("Left Tree:\n");
                PriTree(T1, 0);
                printf("Right Tree:\n");
                PriTree(T2, 0);
            } else {
                printf("Division failed for %d.\n", e);
            }
        }
        else if(choice == 6){
            printf("Exiting the program.\n");
            printf("=============================\n");
            break;
        } else {    
            printf("Invalid choice. Please try again.\n");
        }
        printf("=============================\n");
    }
    printf("AVL Tree:\n");
    PriTree(T, 0);
    return 0;
}
