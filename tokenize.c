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

bool issalpha(char *p){
    return 'a' <= *p && *p <= 'z';
}

bool is_alnum(char c){
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || (c == '_');
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

        if (strncmp(p,"return",6) == 0 && !is_alnum(p[6])){
            cur = new_token(TK_RETURN,cur,p,6);
            p += 6;
            continue;
        }

        if (strncmp(p,"if",2) == 0 && !is_alnum(p[2])){
            cur = new_token(TK_IF,cur,p,2);
            p += 2;
            continue;
        }

        if (strncmp(p,"else",4) == 0 && !is_alnum(p[4])){
            cur = new_token(TK_ELSE,cur,p,4);
            p += 4;
            continue;
        }

        if (check(p,"==") || check(p,"!=") || check(p,"<=") || check(p,">=")){
            cur = new_token(TK_RESERVED,cur,p,2);
            p += 2;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '<' || *p == '>' || *p == '+' || *p == '(' || *p == ')' || *p == '=' || *p == ';'){
            cur = new_token(TK_RESERVED,cur,p++,1);
            continue;
        }

        if (issalpha(p)){
            int len = 0;
            while(issalpha(p)){
                p++;
                len++;
            }
            cur = new_token(TK_IDENT,cur,p-len,len);
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