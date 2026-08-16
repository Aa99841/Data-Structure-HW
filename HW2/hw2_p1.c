#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int ban;
int slot;
int output[100][2];
int searchcount;

void search(long **hash,long num){
    int bucket = num % ban;
    for(int j = 0 ; j < ban ; j++){
        int pos = (bucket + j) % ban;
        for(int i = 0 ; i < slot ; i++){
            if(hash[pos][i] == num){
                output[searchcount][0] = pos;
                output[searchcount][1] = i;
                searchcount ++;
                return;
            }
        }
    }
}

void insert(long **hash, long num){
    int bucket = num % ban;
    for(int j = 0 ; j < ban ; j++){
        int pos = (bucket + j) % ban;
        for(int i = 0 ; i < slot ; i++){
            if(hash[pos][i] == -1){
                hash[pos][i] = num;
                return;
            }
        }
    }
}

void delet(long **hash,long num){
    int bucket = num % ban;
    for(int j = 0 ; j < ban ; j++){
        int pos = (bucket + j) % ban;
        for(int i = 0 ; i < slot ; i++){
            if(hash[pos][i] == num){
                hash[pos][i] = -1;
                return;
            }
        }
    }
}


int main(){
    searchcount = 0;

    scanf("bucket %d\n",&ban);
    scanf("slot %d\n",&slot);

    long **hash = (long **)malloc(100 * sizeof(*hash));
    for (int i = 0; i < 100; i++) {
        hash[i] = (long *)malloc(20 * sizeof(long));
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 20; j++) {
            hash[i][j] = -1;
        }
    }

    while(1){
        char str[10000];
        scanf("%s",str);
        if(strcmp(str,"exit")==0){
            break;
        }else if(strcmp(str,"insert")==0){
            long innum;
            scanf("%ld",&innum);
            insert(hash,innum);
        }else if(strcmp(str,"delete")==0){
            long deletnum;
            scanf("%ld",&deletnum);
            delet(hash,deletnum);
        }else if(strcmp(str,"search")==0){
            long searchNum;
            scanf("%ld",&searchNum);
            search(hash,searchNum);
        }
    }

    for(int k = 0 ; k < searchcount ; k++){
        printf("%d %d\n",output[k][0],output[k][1]); 
    }
}