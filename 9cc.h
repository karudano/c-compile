#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TK_RESERVED,
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
    ND_LVAR,
} NodeKind;

typedef struct Token Token;
typedef struct Node Node;

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
    int val;
    int offset;
};

extern Token *token;
extern char *user_input;

//fuction
void error_at(char *loc,char *fmt,...);
bool consume(char *op);
Token *consume_ident();
void expect(char *op);
int expect_number();
bool at_eof();

Token *new_token(TokenKind kind,Token* cur,char* str,int len);
int check(char* p,char* q);
Token *tokenize(char* p);

Node *new_node(NodeKind kind,Node* lhs,Node* rhs);
Node *new_node_num(int val);
Node *code[100];
void program();
Node *stmt(),*expr(),*assign(),*equality(),*relational(),*add(),*mul(),*primary(),*unary();

void gen(Node* node);
void gen_lval(Node *node);
