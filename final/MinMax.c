#include <stdio.h>

int heap[1000];
int tail = 0;

void insert(int key) {
    heap[++tail] = key;

    int layer = 1;
    while (tail >> layer)
        layer++;

    int min_layer = layer % 2, pt = tail;
    while (pt > 1) {
        if (min_layer ^ (heap[pt] < heap[pt >> 1])) {
            heap[pt] ^= heap[pt >> 1] ^= heap[pt] ^= heap[pt >> 1];
            pt >>= 1;
            min_layer = !min_layer;
        } else if (pt >= 4 && (min_layer ^ (heap[pt] > heap[pt >> 2]))) {
            heap[pt] ^= heap[pt >> 2] ^= heap[pt] ^= heap[pt >> 2];
            pt >>= 2;
        } else {
            break;
        }
    }
}

void deleteMin() {
    heap[1] = heap[tail--];
    int pt = 1;
    while ((pt << 1) <= tail) {
        int target = pt << 1;
        if (target < tail && heap[target + 1] > heap[target])
            target++;

        if (heap[pt] > heap[target]) {
            heap[pt] ^= heap[target] ^= heap[pt] ^= heap[target];
            break;
        }

        if ((pt << 2) > tail)
            break;

        target = pt << 2;
        for (int i = pt << 2; i <= tail && i < (pt << 2) + 4; i++) {
            if (heap[i] < heap[target])
                target = i;
        }

        if (heap[pt] < heap[target])
            break;

        heap[pt] ^= heap[target] ^= heap[pt] ^= heap[target];
        if (heap[target] > heap[target >> 1])
            heap[target] ^= heap[target >> 1] ^= heap[target] ^=
                heap[target >> 1];
        pt = target;
    }
}

int main() {
    char tmp[10];
    int d;
    while (1) {
        scanf("%s", tmp);
        if (tmp[0] == 'e')
            break;

        if (tmp[0] == 'i') {
            scanf("%d", &d);
            insert(d);
        }

        if (tmp[0] == 'd') {
            deleteMin();
        }
    }

    for (int i = 1; i <= tail; i++) {
        printf("%d ", heap[i]);
    }
}