#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Heap{
    int* heapArray;
    int num;
    int capacity;
};

// 調整堆，維持最小堆性質（從下往上）
void heapifyUp(struct Heap* heap, int idx) {
    while (idx > 0 && heap->heapArray[idx] < heap->heapArray[(idx - 1) / 2]) {
        // 交換當前節點與父節點
        int temp = heap->heapArray[idx];
        heap->heapArray[idx] = heap->heapArray[(idx - 1) / 2];
        heap->heapArray[(idx - 1) / 2] = temp;
        // 更新當前節點為父節點位置
        idx = (idx - 1) / 2;
    }
}

// 調整堆，維持最小堆性質（從上往下）
void heapifyDown(struct Heap* heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left <= heap->num && heap->heapArray[left] < heap->heapArray[smallest]) {
        smallest = left;
    }
    if (right <= heap->num && heap->heapArray[right] < heap->heapArray[smallest]) {
        smallest = right;
    }
    if (smallest != idx) {
        // 交換當前節點與最小的子節點
        int temp = heap->heapArray[idx];
        heap->heapArray[idx] = heap->heapArray[smallest];
        heap->heapArray[smallest] = temp;
        // 遞迴調整下方的子堆
        heapifyDown(heap, smallest);
    }
}

void Delet(struct Heap* heap , int whereDelet) {
    heap->heapArray[whereDelet] = heap->heapArray[heap->num];  // 用最後一個節點替換
    heap->num--;  // 減少堆大小
    heapifyDown(heap, whereDelet);  // 從被刪除位置開始重新調整
}

void Insert(struct Heap* heap , int insertNum) {
    // 如果堆容量不夠，擴大容量
    if (heap->num + 1 == heap->capacity) {
        heap->capacity *= 2;
        heap->heapArray = (int*) realloc(heap->heapArray, heap->capacity * sizeof(int));
    }

    // 插入新元素
    heap->num++;
    heap->heapArray[heap->num] = insertNum;
    heapifyUp(heap, heap->num);  // 從最後位置開始向上調整
}

int Exit(struct Heap* heap , int findNum) {
    for (int i = 0; i <= heap->num; i++) {
        if (heap->heapArray[i] == findNum) {
            return i;
        }
    }
    return -1;
}

int main() {
    struct Heap* heap = (struct Heap*)malloc(sizeof(struct Heap));
    heap->num = -1;
    heap->capacity = 10;
    heap->heapArray = (int*)malloc(heap->capacity * sizeof(int));

    // 讀取輸入
    char commands[100][100];
    int lineCount = 0;
    char command[10];
    int value;

    while (scanf("%s %d",command,&value) != "\n") {
        command[strcspn(commands[lineCount], "\n")] = '\0';
        //sscanf(commands[lineCount], "%s %d", command, &value);
        if (strcmp(command, "insert") == 0) {
            if (Exit(heap, value) == -1) {
                Insert(heap, value);
            }
        } else if (strcmp(command, "delete") == 0) {
            int whereDelet = Exit(heap, value);
            if (whereDelet != -1) {
                Delet(heap, whereDelet);
            }
        }
        lineCount++;
    }
    /*
    for (int i = 0; i < lineCount; i++) {
        sscanf(commands[i], "%s %d", command, &value);
        if (strcmp(command, "insert") == 0) {
            if (Exit(heap, value) == -1) {
                Insert(heap, value);
            }
        } else if (strcmp(command, "delete") == 0) {
            int whereDelet = Exit(heap, value);
            if (whereDelet != -1) {
                Delet(heap, whereDelet);
            }
        }
    }
    */

    // 輸出堆內容
    for (int j = 0; j <= heap->num; j++) {
        if (j != heap->num) {
            printf("%d ", heap->heapArray[j]);
        } else {
            printf("%d", heap->heapArray[j]);
        }
    }

    // 釋放內存
    free(heap->heapArray);
    free(heap);
}
