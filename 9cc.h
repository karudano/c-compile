#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TK_RESERVED,
    TK_RETURN,
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef enum{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
    ND_EQ,
    ND_NE,
    ND_LT, // <
    ND_LE, // <=
    ND_ASSIGN, // =
    ND_LVAR,// left value
    ND_RETURN, // return
    ND_IF,// if
    ND_ELSE, //else
    ND_WHILE, //while
} NodeKind;

typedef struct Token Token;
typedef struct Node Node;
typedef struct LVar LVar;

struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;

    // if statement
    Node *cond;
    Node *then;
    Node *els;

    int val;
    int offset;
};

struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
};

extern Token *token;
extern char *user_input;
extern LVar *locals;
extern long serial;

//fuction
void error_at(char *loc,char *fmt,...);
bool consume(char *op);
bool consume_kind(NodeKind a);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();

Token *new_token(TokenKind kind,Token* cur,char* str,int len);
int check(char* p,char* q);
bool issalpha(char *p);
bool is_alnum(char c);
Token *tokenize(char* p);

Node *new_node(NodeKind kind,Node* lhs,Node* rhs);
Node *new_node_num(int val);
Node *code[100];
LVar *find_lvar(Token *tok);
void program();
Node *stmt(),*expr(),*assign(),*equality(),*relational(),*add(),*mul(),*primary(),*unary();

void gen(Node* node);
void gen_lval(Node *node);
