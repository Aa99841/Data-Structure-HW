#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Heap{
    int* heapArray;
    int num;
};

void Delet(struct Heap* heap , int deletNum,int whereDelet){
    int k = whereDelet;
    int temp = -1;
    heap->heapArray[k] = heap->heapArray[heap->num];
    //heap->heapArray[heap->num] = -1;
    //int swith = heap->heapArray[k];
    int smallPosition = 0;
    heap->num--;
    //printf("2:%d\n",k);
        while(k<=heap->num){
            smallPosition = k;
            if(heap->heapArray[smallPosition] > heap->heapArray[k*2+1]){
                if(k*2+1 > heap->num){
                    break;
                }
                smallPosition = k*2+1;
            }
            if(heap->heapArray[smallPosition] > heap->heapArray[k*2+2]){
                if(k*2+2 > heap->num){
                    break;
                }
                smallPosition = k*2+2;
            }
            if(smallPosition == k){
                break;
            }
            temp = heap->heapArray[k];
                //printf("%d\n",temp);
            heap->heapArray[k] = heap->heapArray[smallPosition];
                //printf("%d\n",heap->heapArray[k]);
            heap->heapArray[smallPosition] = temp;
                //printf("%d\n",heap->heapArray[(k+1)*2]);
            k = smallPosition;
                //printf("%d\n",k);
        }
}

void Insert(struct Heap* heap , int insertNum){
    heap->num++;
    heap->heapArray[heap->num] = insertNum;
    int k = 0;
    int changeNum = heap->num;
    int up = (changeNum+1)/2-1;
    if(heap->num == k){
        return;
    }
    //printf("1:%d\n",heap->num);
    while(insertNum < heap->heapArray[up]){
        k = heap->heapArray[up];
        heap->heapArray[up] = insertNum;
        heap->heapArray[changeNum] = k;
        changeNum = up;
        up = (changeNum+1)/2-1;
        if(k < 0 || up < 0){
            //printf("%d\n",(changeNum+1)*2-1);
            break;
        }
    }
}

int Exit(struct Heap* heap , int findNum){
    for(int i = 0;i<=heap->num;i++){
        if(heap->heapArray[i] == findNum){
            return i;
        }
    }
    return -1;
}

int Empty(struct Heap* heap){
    return heap->num == -1;
}



int main() {
    struct Heap* heap = (struct Heap*)malloc(sizeof(struct Heap));
    heap->num = -1;
    heap->heapArray = (int*)malloc(10*sizeof(int));
/*
Input:
insert 50 
insert 30 
insert 40 
insert 20 
insert 35 
delete 20 
insert 25 
insert 45 
delete 25 
Output:
30 35 40 50 45 
*/

    //printf("hello");
    char commands[100][100];
    //memset(commands, -1, sizeof(commands));

    int lineCount = 0;
    while (fgets(commands[lineCount], 100, stdin)) {
        if (strcmp(commands[lineCount], "\n") == EOF) {
            break;
        }
        commands[lineCount][strcspn(commands[lineCount], "\n")] = '\0';
        lineCount++;
    }


    char command[10];
    int value;

    for(int i=0 ; i<lineCount ; i++){
        if(strcmp(commands[i], "-1")!=0){
            sscanf(commands[i], "%s %d ", command, &value);
        if(strcmp(command,"insert") == 0){
            //input = strtok(commands[i]," ");
            //Insert(heap,value);
            
            if(Exit(heap, value)==-1){
                Insert(heap,value);
            }
        }else if(strcmp(command,"delete") == 0){
            //printf("%d:2",value);
            //input = strtok(commands[i]," ");
            int whereDelet = Exit(heap,value);
            //printf("%d\n",whereDelet);
            //Delet(heap,value,whereDelet);
            
            if(whereDelet != -1){
                //printf("%d\n",value);
                Delet(heap,value,whereDelet);
            }
        }
        }
    }
    int length = heap->num+1;
    //printf("%d\n",heap->num);
    for (int j=0; j<length; j++){
        if(j!=length-1){
            printf("%d ",heap->heapArray[j]);
        }else{
            printf("%d",heap->heapArray[j]);
        }
    }

}