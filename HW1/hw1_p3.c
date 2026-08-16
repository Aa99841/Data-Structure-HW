#include<stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void BFS(int numOfNode, int** nodeCombine){
    bool visit[numOfNode];
    int quene[numOfNode];
    int top = 0;
    int last = 0;
    //int count = 0;
    
    memset(visit, 0, numOfNode * sizeof(bool));
    memset(quene, 0, numOfNode * sizeof(bool));

    //printf("BES\n");

    quene[0] = 1;
    visit[0] = true;
    while(last >= top){
        int currentTop = quene[top];
        printf("%d ",currentTop);

        for(int j = 0; j<numOfNode; j++){
            if(nodeCombine[currentTop-1][j]==1 && !visit[j]){
                last++;
                quene[last] = j + 1;
                visit[j] = true;
            }
        }
        top++;
    }

   return;
}

int main()
{
    int numOfNode;
    int** nodeCombine;

    scanf("%d", &numOfNode);

    nodeCombine = (int**)malloc(numOfNode * sizeof(int*));
    for (int i = 0; i < numOfNode; i++) {
        nodeCombine[i] = (int*)malloc(numOfNode * sizeof(int));
    }

    for (int i = 0; i < numOfNode; i++) {
        for (int j = 0; j < numOfNode; j++) {
            scanf("%d", &nodeCombine[i][j]);
        }
    }

    BFS(numOfNode, nodeCombine);
    return 0;
}
