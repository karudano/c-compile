#include "9cc.h"

void error_at(char *loc,char *fmt,...){
    va_list ap;
    va_start(ap,fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n",user_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr,"^ ");
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    exit(1);
}

bool consume(char *op){
    if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(op,token->str,token->len)) return false;
    token = token->next;
    return true;
}

Token *consume_ident(){
    if (token->kind != TK_IDENT) return NULL;
    Token *t = token;
    token = token->next;
    return t;    
}

void expect(char *op){
    if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(op,token->str,token->len)) error_at(token->str,"'%s'ではありません",op);
    token = token->next;
}

int expect_number(){
    if (token->kind != TK_NUM && token->kind != TK_IDENT){
        error_at(token->str,"数ではありません");
    }
    int val = token->val;
    token = token->next;
    return val;
}



bool at_eof(){
    return token->kind == TK_EOF;
}
