#include "9cc.h"
long serial = 0;

void gen_lval(Node *node){
    if (node->kind != ND_LVAR) fprintf(stderr,"代入の左辺値が変数ではありません");
    printf("    mov rax, rbp\n");
    printf("    sub rax, %d\n",node->offset);
    printf("    push rax\n");
}

void gen(Node *node){
    switch(node->kind){
        case ND_RETURN:
            gen(node->lhs);
            printf("    pop rax\n");
            printf("    mov rsp, rbp\n");
            printf("    pop rbp\n");
            printf("    ret\n");
            return;
        case ND_IF:
            gen(node->cond);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            if (node->els){
                printf("    je  .Lelse%ld\n",serial);
                gen(node->then);
                printf("    jmp .Lend%ld\n",serial);
                printf(".Lelse%ld:\n",serial);
                gen(node->els);
                printf(".Lend%ld:\n",serial);
                serial++;
            }
            else{
                printf("    je  .Lend%ld\n",serial);
                gen(node->then);
                printf(".Lend%ld:\n",serial);
                serial++;
            }
            return;
        case ND_WHILE:
            printf(".Lbegin%ld:\n",serial);
            gen(node->cond);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je  .Lend%ld\n",serial);
            gen(node->then);
            printf("    jmp .Lbegin%ld\n",serial);
            printf(".Lend%ld:\n",serial);
            serial++;
            return;
        case ND_FOR:
            gen(node->fst);
            printf(".Lbegin%ld:\n",serial);
            gen(node->cond);
            printf("    pop rax\n");
            printf("    cmp rax, 0\n");
            printf("    je  .Lend%ld\n",serial);
            gen(node->then);
            gen(node->nxt);
            printf("    jmp .Lbegin%ld\n",serial);
            printf(".Lend%ld:\n",serial);
            serial++;
            return;
    }

    switch(node -> kind){
        case ND_NUM:
            printf("    push %d\n",node->val);
            return;
        case ND_LVAR:
            gen_lval(node);
            printf("    pop rax\n");
            printf("    mov rax, [rax]\n");
            printf("    push rax\n");
            return;
        case ND_BLOCK:
            for (Node *i = node->block;i;i = i->next){
                gen(i);
            }
            return;
        case ND_ASSIGN:
            gen_lval(node->lhs);
            gen(node->rhs);
            printf("    pop rdi\n");
            printf("    pop rax\n");
            printf("    mov [rax], rdi\n");
            printf("    push rdi\n");
            return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("    pop rdi\n");
    printf("    pop rax\n");

    switch(node -> kind){
        case ND_EQ:
            printf("    cmp rax, rdi\n");
            printf("    sete al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_NE:
            printf("    cmp rax, rdi\n");
            printf("    setne al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LE:
            printf("    cmp rax, rdi\n");
            printf("    setle al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_LT:
            printf("    cmp rax, rdi\n");
            printf("    setl al\n");
            printf("    movzb rax, al\n");
            break;
        case ND_ADD:
            printf("    add rax, rdi\n");
            break;
        case ND_SUB:
            printf("    sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("    imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("    cqo\n");
            printf("    idiv rdi\n");
            break;
    }

    printf("    push rax\n");
}