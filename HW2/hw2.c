#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct FNode {
    long key;
    int degree;
    int level;
    struct FNode *right;
    struct FNode *left;
    struct FNode *parent;
    struct FNode *child;
    bool childcut;
} FNode;

typedef struct FHeap {
    FNode *min;
} FHeap;

void cascading(FHeap *F, FNode *del);

void printTree(FNode *node) {
    if (node == NULL) return;

    FNode *current;
    FNode *child;
    FNode *start = node;
    FNode *queue[1000];
    int front = 0, rear = 0;

    queue[rear++] = node;

    while (front != rear) {
        current = queue[front++];

        printf("%ld ", current->key);

        if (current->child != NULL) {
            child = current->child;
            do {
                queue[rear++] = child;
                child = child->right;
            } while (child != current->child);
        }
    }
}

void printHeap(FHeap *F) {
    if (F->min == NULL) {
        //printf("Heap is empty.\n");
        return;
    }

    FNode *current = F->min;
    FNode *trees[100];
    int numTrees = 0;

    do {
        trees[numTrees++] = current;
        current = current->right;
    } while (current != F->min);

    for (int i = 0; i < numTrees ; i++) {
        for (int j = i + 1; j < numTrees; j++) {
            if (trees[i]->degree > trees[j]->degree) {
                FNode *temp = trees[i];
                trees[i] = trees[j];
                trees[j] = temp;
            }
        }
    }

    for (int i = 0; i < numTrees; i++) {
        printTree(trees[i]);
        printf("\n");
    }
}

FNode *creatNode(long key) {
    FNode *n = (FNode*)malloc(sizeof(FNode));
    if (!n) {
        //fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->key = key;
    n->level = 1;
    n->child = NULL;
    n->parent = NULL;
    n->childcut = false;
    n->left = NULL;
    n->right = NULL;
    n->degree = 0;
    return n;
}

FNode *findNode(FNode *n, long key) {
    if (n == NULL) return NULL;

    FNode *start = n;
    do {
        // 如果找到目标键值，直接返回
        if (n->key == key) {
            return n;
        }

        // 如果当前节点有子树，递归查找
        if (n->child != NULL) {
            FNode *childResult = findNode(n->child, key);
            if (childResult != NULL) {
                return childResult;
            }
        }

        // 遍历兄弟节点
        n = n->right;
    } while (n != start);

    return NULL; // 未找到
}


FNode *find(FHeap *F, long key) {
    if (F->min == NULL) {
        return NULL;
    } else {
        return findNode(F->min, key);
    }
}

void insert(FHeap *F, FNode *in) {
    if (F->min == NULL) {
        F->min = in;
        in->left = in;
        in->right = in;
        return;
    }

    FNode *current = F->min;

    do {
        if (in->key < current->key) {
            in->right = current;
            in->left = current->left;
            current->left->right = in;
            current->left = in;

            if (current == F->min) {
                F->min = in;
            }
            return;
        }
        current = current->right;
    } while (current != F->min);

    in->right = F->min;
    in->left = F->min->left;
    F->min->left->right = in;
    F->min->left = in;

    if (in->key < F->min->key) {
        F->min = in;
    }
}

void insertChild(FHeap *F, FNode *in, FNode *p) {
    in->level++;
    p->degree++;

    if (p->child == NULL) {
        p->child = in;
        in->left = in;
        in->right = in;
        return;
    }

    FNode *current = p->child;

    do {
        if (in->key < current->key) {
            in->right = current;
            in->left = current->left;
            current->left->right = in;
            current->left = in;

            if (current == p->child && in->key < p->child->key) {
                p->child = in;
            }
            return;
        }
        current = current->right;
    } while (current != p->child);

    in->right = p->child;
    in->left = p->child->left;
    p->child->left->right = in;
    p->child->left = in;
}

void meld(FHeap *F) {
    FNode *current = F->min;
    FNode *next = F->min->right;
    int degree = 0;
    FNode *maxdegree[90];

    for (int i = 0; i < 90; i++) {
        maxdegree[i] = NULL;
    }

    do {
        degree = current->degree;
        FNode *next = current->right;

        while (maxdegree[degree] != NULL) {
            FNode *child = maxdegree[degree];

            if (current->key > child->key) {
                FNode *tmp = child;
                child = current;
                current = tmp;
            }

            insertChild(F, child, current);
            child->parent = current;

            maxdegree[degree] = NULL;
            degree++;
        }
        maxdegree[degree] = current;
        current = next;

    } while (current != F->min);

    F->min = NULL;
    for (int i = 0; i < 90; i++) {
        if (maxdegree[i] != NULL) {
            if (F->min == NULL) {
                F->min = maxdegree[i];
                F->min->left = F->min->right = F->min;
            } else {
                insert(F, maxdegree[i]);
            }
        }
    }
}

void decrease(FHeap *F, long key, long value) {
    FNode *node = find(F, key);
    if (node == NULL || value >= node->key || value < 0) {
        return;
    }
    FNode *p = node->parent;
    long newkey = key - value;

    if (node->parent == NULL) {
        node->key = newkey;
        node->left->right = node->right;
        node->right->left = node->left;
        insert(F, node);
        return;
    }

    if (node->parent->key < newkey) {
        node->key = newkey;
        return;
    } else {
        node->key = newkey;

        if (p->child == node) {
            if (node->right == node) {
                p->child = NULL;
            } else {
                node->parent->child = node->right;
            }
        }

        node->left->right = node->right;
        node->right->left = node->left;

        p->degree--;

        node->parent = NULL;
        node->childcut = false;

        insert(F, node);
    }

    if (newkey < F->min->key) {
        F->min = node;
    }

    p->childcut = true;
    cascading(F, p);
}

void cascading(FHeap *F, FNode *del) {
    FNode *current = del;
    while (current != NULL && current->childcut) {
        FNode *node = del;
    
        FNode *p = node->parent;
        long newkey = del->key;

        if (node->parent == NULL) {
            node->key = newkey;
            node->left->right = node->right;
            node->right->left = node->left;
            insert(F, node);
            return;
        }
            node->key = newkey;

            if (p->child == node) {
                if (node->right == node) {
                    p->child = NULL;
                } else {
                    node->parent->child = node->right;
                }
            }

            node->left->right = node->right;
            node->right->left = node->left;

            p->degree--;

            node->parent = NULL;
            node->childcut = false;

            insert(F, node);
        

        if (newkey < F->min->key) {
            F->min = node;
        }
        current = current->parent;
    }
    // while (current != NULL) {
    //     FNode *parent = current->parent;

    //     // 如果当前节点的 childcut 为 false，则只标记
    //     if (!current->childcut) {
    //         current->childcut = true;
    //         break;
    //     }

    //     // 如果 childcut 已为 true，将当前节点切断
    //     if (parent != NULL) {
    //         // 从父节点的子链表中移除
    //         if (parent->child == current) {
    //             if (current->right == current) { // 唯一子节点
    //                 parent->child = NULL;
    //             } else {
    //                 parent->child = current->right;
    //             }
    //         }

    //         current->left->right = current->right;
    //         current->right->left = current->left;

    //         parent->degree--; // 减少父节点的度数

    //         // 插入根链表并断开父子关系
    //         insert(F, current);
    //         current->parent = NULL;
    //         current->childcut = false;

    //         current = parent; // 递归向上
    //     } else {
    //         break; // 根节点不再处理
    //     }
    // }
}


void deleteNode(FHeap *F, long key) {
    FNode *del = find(F, key);
    if (del == NULL) {
        return;
    }

    FNode *p = del->parent;

    // 如果是最小节点，更新最小值指针
    if (del == F->min) {
        if (del->right == del) { // 唯一树时
            F->min = NULL;
        } else {
            F->min = del->right;
        }
    }

    // 将 del 的子节点插入到根链表中
    if (del->child != NULL) {
        FNode *n = del->child;
        FNode *start = del->child;

        do {
            FNode *next = n->right;

            n->left->right = n->right; // 从子链表中移除
            n->right->left = n->left;

            insert(F, n); // 插入根链表
            n->parent = NULL; // 断开父节点关系

            n = next;
        } while (n != start);
    }

    // 从父节点的子链表中移除
    if (p != NULL && p->child == del) {
        if (del->right == del) { // 唯一子节点时
            p->child = NULL;
        } else {
            p->child = del->right;
        }
    }

    if (p != NULL) {
        del->left->right = del->right;
        del->right->left = del->left;
        p->degree--;
        p->childcut = true;
        cascading(F, p); // 级联切断
    } else {
        del->left->right = del->right;
        del->right->left = del->left;
    }

    free(del);

    // 如果最小值存在多个根节点，重新 meld
    if (F->min != NULL) {
        meld(F);
    }
}

void deleteMin(FHeap *F) {
    FNode *del = F->min;

    if (del == NULL) {
        return;
    }

    // 如果有子节点，将它们插入根链表
    if (del->child != NULL) {
        FNode *n = del->child;
        FNode *start = del->child;

        do {
            FNode *next = n->right;

            n->left->right = n->right; // 从子链表中移除
            n->right->left = n->left;

            insert(F, n); // 插入根链表
            n->parent = NULL; // 断开父节点关系

            n = next;
        } while (n != start);
    }

    // 从根链表中移除最小值节点
    if (del->right == del) { // 唯一树时
        F->min = NULL;
    } else {
        del->left->right = del->right;
        del->right->left = del->left;
        F->min = del->right;
    }

    free(del);

    // 如果根链表非空，重新 meld
    if (F->min != NULL) {
        meld(F);
    }
}


int main() {
    FHeap *F = (FHeap*)malloc(sizeof(FHeap));
    if (!F) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    F->min = NULL;

    while (1) {
        long key;
        char str[10000];
        scanf("%s", str);
        if (strcmp(str, "exit") == 0) {
            break;
        } else if (strcmp(str, "insert") == 0) {
            scanf("%ld", &key);
            FNode *in = creatNode(key);
            insert(F, in);
        } else if (strcmp(str, "delete") == 0) {
            scanf("%ld", &key);
            deleteNode(F, key);
        }else if (strcmp(str, "decrease") == 0) {
            long value;
            scanf("%ld %ld", &key, &value);
            decrease(F, key, value);
        } else if (strcmp(str, "extract-min") == 0) {
            deleteMin(F);
        }
    }

    printHeap(F);
    free(F);
    return 0;
}