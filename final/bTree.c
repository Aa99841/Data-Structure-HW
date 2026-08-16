#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 定義 2-3-4 樹的最大鍵數和最小鍵數
#define MAX_KEYS 3  // 最大鍵數（B-Tree 階數 t=2 時，2t-1=3）
#define MIN_KEYS (MAX_KEYS / 2) // 最少鍵數，t=2 時至少要有 1 個鍵

// 節點結構
typedef struct BTreeNode {
    int keys[MAX_KEYS + 1];           // 儲存鍵值，多一格預留插入溢位
    struct BTreeNode *children[MAX_KEYS + 2]; // 子節點指標，多一格預留分裂用
    int key_count;                    // 節點內的鍵數
    bool is_leaf;                     // 是否為葉子節點
} BTreeNode;

// 建立一個新的節點
BTreeNode* create_node(bool is_leaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->key_count = 0; // 初始鍵數為 0
    node->is_leaf = is_leaf;
    for (int i = 0; i < MAX_KEYS + 2; i++) { // 初始化所有子節點為 NULL
        node->children[i] = NULL;
    }
    return node;
}

// 分裂子節點
void split_child(BTreeNode *parent, int index, BTreeNode *child) {
    BTreeNode *right = create_node(child->is_leaf); // 建立新的右節點

    int mid = (MAX_KEYS + 1) / 2; // 中間位置
    int pivot = child->keys[mid - 1]; // 中間鍵值

    // 將右半部分鍵值搬移到新的右節點
    right->key_count = child->key_count - mid;
    for (int i = 0; i < right->key_count; i++) {
        right->keys[i] = child->keys[mid + i];
    }

    // 如果不是葉子節點，搬移對應的子節點指標
    if (!child->is_leaf) {
        for (int i = 0; i <= right->key_count; i++) {
            right->children[i] = child->children[mid + i];
        }
    }

    // 調整左節點的鍵數
    child->key_count = mid - 1;

    // 將父節點中鍵值和子節點指標右移，以插入中間鍵和新節點
    for (int i = parent->key_count; i > index; i--) {
        parent->keys[i] = parent->keys[i - 1];
        parent->children[i + 1] = parent->children[i];
    }
    parent->keys[index] = pivot;
    parent->children[index + 1] = right;
    parent->key_count++;
}

// 插入鍵值到非滿節點
void insert_non_full(BTreeNode *node, int key) {
    int i = node->key_count - 1;

    if (node->is_leaf) { // 葉子節點處理
        // 找到適當的位置插入鍵值
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->key_count++;
    } else { // 非葉子節點處理
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++; // 找到子節點索引

        // 如果子節點已滿，先分裂
        if (node->children[i]->key_count == MAX_KEYS) {
            split_child(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++; // 根據分裂後的結構調整索引
            }
        }
        insert_non_full(node->children[i], key);
    }
}

// 插入鍵值
BTreeNode* insert(BTreeNode *root, int key) {
    if (root->key_count == MAX_KEYS) { // 如果根節點已滿，需創建新根
        BTreeNode *new_root = create_node(false); // 新的根節點
        new_root->children[0] = root; // 將舊根設為子節點
        split_child(new_root, 0, root); // 分裂舊根

        int i = 0;
        if (key > new_root->keys[0]) { // 插入到適當的子節點
            i++;
        }
        insert_non_full(new_root->children[i], key);
        return new_root;
    } else { // 根節點未滿，直接插入
        insert_non_full(root, key);
        return root;
    }
}

// 按層級列印 B-Tree（Level-order traversal）
void print_levels(BTreeNode *root) {
    if (!root) return;

    BTreeNode* queue[100]; // 隊列，用於 BFS
    int level[100];
    int front = 0, rear = 0;

    queue[rear] = root; // 根節點入隊
    level[rear++] = 1; // 設定層級

    int current_level = 1;
    printf("B-Tree Level Order Traversal:\n");
    printf("Level 1: ");

    while (front < rear) {
        BTreeNode* node = queue[front];
        int node_level = level[front++];

        if (node_level > current_level) { // 換行處理
            current_level = node_level;
            printf("\nLevel %d: ", current_level);
        }

        // 列印當前節點的鍵值
        printf("[");
        for (int i = 0; i < node->key_count; i++) {
            printf("%d", node->keys[i]);
            if (i < node->key_count - 1) {
                printf(" ");
            }
        }
        printf("] ");

        // 子節點入隊
        if (!node->is_leaf) {
            for (int i = 0; i <= node->key_count; i++) {
                if (node->children[i]) {
                    queue[rear] = node->children[i];
                    level[rear++] = node_level + 1;
                }
            }
        }
    }
    printf("\n");
}

// 搜尋 B-Tree 的鍵值
bool search(BTreeNode *root, int key) {
    int i = 0;

    // 找到大於等於 key 的第一個鍵
    while (i < root->key_count && key > root->keys[i]) {
        i++;
    }

    // 如果鍵值在當前節點中，返回 true
    if (i < root->key_count && key == root->keys[i]) {
        printf("Key %d found at node: [", key);
        for (int j = 0; j < root->key_count; j++) {
            printf("%d", root->keys[j]);
            if (j < root->key_count - 1) {
                printf(" ");
            }
        }
        printf("]\n");
        return true;
    }

    // 如果該節點是葉子，找不到鍵值
    if (root->is_leaf) {
        return false;
    }

    // 向子節點遞迴搜尋
    return search(root->children[i], key);
}

int main() {
    BTreeNode *root = create_node(true); // 建立一個空的根節點（葉子節點）

    int keys[] = {8, 4, 2, 5, 10, 9, 6, 7}; // 要插入的鍵值
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++) {
        root = insert(root, keys[i]); // 將鍵值插入 B-Tree
    }

    print_levels(root); // 列印 B-Tree 的層級結構

    int search_keys[] = {5, 7, 1, 10}; // 要搜尋的鍵值
    int m = sizeof(search_keys) / sizeof(search_keys[0]);

    for (int i = 0; i < m; i++) {
        if (!search(root, search_keys[i])) { // 搜尋鍵值，若未找到則列印訊息
            printf("Key %d not found\n", search_keys[i]);
        }
    }

    return 0;
}
