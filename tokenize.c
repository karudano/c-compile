#include "9cc.h"

Token* new_token(TokenKind kind,Token *cur,char *str,int len){
    Token *tok = calloc(1,sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

int check(char *p,char *q){
    return memcmp(p,q,strlen(q)) == 0;
}

Token *tokenize(char *p){
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while(*p){
        if (isspace(*p)){
            p++;
            continue;
        }

        if (check(p,"==") || check(p,"!=") || check(p,"<=") || check(p,">=")){
            cur = new_token(TK_RESERVED,cur,p,2);
            p += 2;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '<' || *p == '>' || *p == '+' || *p == '(' || *p == ')'){
            cur = new_token(TK_RESERVED,cur,p++,1);
            continue;
        }

        if (isdigit(*p)){
            cur = new_token(TK_NUM,cur,p,0);
            cur->val = strtol(p,&p,10);
            continue;
        }

        error_at(p,"トークナイズできません");
    }
    new_token(TK_EOF,cur,p,0);
    return head.next;
}