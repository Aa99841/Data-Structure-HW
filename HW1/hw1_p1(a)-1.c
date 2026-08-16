#include <stdio.h>
#include <string.h>
//#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <ctype.h>

struct Stack{
   int top; // top在array中的位置
   //int length; // 輸入(出)的長度
   char** prefix; //存輸出字串的位置
};

int Empty(struct Stack* stack){// 0是false,1是true
   if(stack->top != -1){
      return 0;
   }else{
      return 1;
   }
}

char* Pop(struct Stack* stack){
    if(Empty(stack)){
        return NULL ;
    }
    else{
        char* data = stack->prefix[stack->top];
        stack->top--;
        return data;
    }
}

void Push(struct Stack* stack, char* data){
   stack->top++;
   stack->prefix[stack->top] = data;
}

int main(){
   char* input = NULL;
   scanf("%c",&input);
   input[strcspn(input, "\n")] = 0;
   int length = strlen(input);
   struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
   stack->top = -1;
   stack->prefix = (char**)malloc(length * sizeof(char*));

   for(int i=0 ; i<length ; i++){
      if(input[i] == "+" || input[i] == "-" || input[i] == "*" || input[i] == "/"){
         char* data1 = pop(stack);
         char* data2 = pop(stack);
         char* new = strcat(data1,input[i]);
         new = strcat(new,data2);
         push(stack,new);
      }else{
         push(stack,input[i]);
      }
   }

   return 0;
}
