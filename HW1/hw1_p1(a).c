#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Stack {
   int top; // 最上方的元素存在array的位置
   char** prefix; // 輸出字串所存的陣列
};

int Empty(struct Stack* stack) {
   if(stack->top != -1){
      return 0;
   }else{
      return 1;
   }
}

char* Pop(struct Stack* stack) {
   if (Empty(stack)) {
      return NULL;
   } else {
      char* data = stack->prefix[stack->top];
      stack->top--;
      return data;
   }
}

void Push(struct Stack* stack, char* data) {
   stack->top++;
   stack->prefix[stack->top] = data;
}

/**int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}**/

int main() {// prefix -> infix
   char* input = NULL;
   size_t bufsize = 0;
   /*
   char* input = NULL;
   scanf("%c",input);
   input[strcspn(input, "\n")] = 0;
   */
   
   getline(&input, &bufsize, stdin);
   input[strcspn(input, "\n")] = 0;
   
   int length = strlen(input);
   struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
   stack->top = -1;
   stack->prefix = (char**)malloc(length * sizeof(char*));

   for (int i = length - 1; i >= 0; i--) { //從尾開始讀
      if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {
         char* data1 = Pop(stack);
         char* data2 = Pop(stack);
         char* newExpr = (char*)malloc(strlen(data1) + strlen(data2) + 4);
         sprintf(newExpr, "%s%c%s", data1, input[i], data2);

         Push(stack, newExpr);
      } else {
         char* operand = (char*)malloc(2);
         operand[0] = input[i];
         operand[1] = '\0';
         Push(stack, operand);
      }
   }

   char* result = Pop(stack);
   printf("%s\n", result);

   free(input);
   free(result);
   free(stack->prefix);
   free(stack);

   return 0;
}
