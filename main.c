#include "9cc.h"

Token *token;
char *user_input;
LVar *locals;

int main(int argc,char **argv){
    if (argc != 2){
        fprintf(stderr,"引数の個数が正しくありません");
        return 1;
    }

    user_input = argv[1];
    locals = calloc(1,sizeof(LVar));
    locals->next = NULL;
    locals->offset = 0;
    token = tokenize(argv[1]);
    program();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    printf("    push rbp\n");
    printf("    mov rbp, rsp\n");
    printf("    sub rsp, 1000\n");

    for (int i = 0;code[i];i++){
        gen(code[i]);

        printf("    pop rax\n");
    }
    
    printf("    mov rsp, rbp\n");
    printf("    pop rbp\n");
    printf("    ret\n");
    return 0;
}
