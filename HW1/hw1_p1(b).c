#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


struct Stack {
    int top; // 最上方的元素存在array的位置
    char** postfix; // 輸出字串所存的陣列
};

int Empty(struct Stack* stack) {
    return stack->top == -1;
}

char* Pop(struct Stack* stack) {
    if (Empty(stack)) {
        return NULL;
    } else {
        char* data = stack->postfix[stack->top];
        stack->top--;
        return data;
    }
}

void Push(struct Stack* stack, char* data) {
    stack->top++;
    stack->postfix[stack->top] = data;
}

int compare(char input){
    if(input == '*' || input == '/'){
        return 2;
    }
    if(input == '+' || input == '-'){
        return 1;
    }
}

int main() { // infix -> postfix
    char* input = NULL;
    size_t bufsize = 0;
    int count = 0;
    char** data = NULL;
    int k = 0;

    getline(&input, &bufsize, stdin);
    input[strcspn(input, "\n")] = 0;

    int length = strlen(input);
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = -1;
    stack->postfix = (char**)malloc(length * sizeof(char*));

    char* output = (char*)malloc(length + 1);
    output[0] = '\0';

    for (int i = 0; i <= length ; i++) {
        if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {
            char* operand = (char*)malloc(2);
            operand[0] = input[i];
            operand[1] = '\0';
            int inputPriority = compare(input[i]);
            while (!Empty(stack) && inputPriority <= compare(stack->postfix[stack->top][0])) {
                char* temp = Pop(stack);
                strcat(output,temp);
            }
            Push(stack, operand);
            
        } else {
            char* operand = (char*)malloc(2);
            operand[0] = input[i];
            operand[1] = '\0';
            strcat(output,operand);
        }
    }

    while (!Empty(stack)) {
        char* operation = Pop(stack);
        strcat(output, operation);
        //free(operation);
    }

    printf("%s\n", output);

    free(input);
    free(output);
    free(stack->postfix);
    free(stack);

    return 0;
}
