#include "9cc.h"

Node *new_node(NodeKind kind,Node *lhs,Node *rhs){
    Node *node = calloc(1,sizeof(Node));
    node -> kind = kind;
    node -> lhs = lhs;
    node -> rhs = rhs;
    return node;
}

Node *new_node_num(int val){
    Node *node = calloc(1,sizeof(Node));
    node -> kind = ND_NUM;
    node -> val = val;
    return node;
}

LVar *find_lvar(Token *tok){
    for (LVar *var = locals;var;var = var->next){
        if (var->len == tok->len && !memcmp(tok->str,var->name,var->len)){
            return var;
        }
    }
    return NULL;
}

void program(){
    int i = 0;
    while(!at_eof()){
        code[i++] = stmt();
    }
    code[i] = NULL;
}
Node *stmt(){
    Node *node;
    if (token->kind == TK_RETURN){
        token = token->next;
        node = calloc(1,sizeof(Node));
        node->kind = ND_RETURN;
        node->lhs = expr();
        expect(";");
    }
    else if (token->kind == TK_IF){
        token = token->next;
        node = calloc(1,sizeof(Node));
        node->kind = ND_IF;
        expect("(");
        node->cond = expr();
        expect(")");
        node->then = stmt();
        if (consume_kind(TK_ELSE)){
            node->els = stmt();
        }
        else{
            node->els = NULL;
        }
    }
    else if (token->kind == TK_WHILE){
        token = token->next;
        node = calloc(1,sizeof(Node));
        node->kind = ND_WHILE;
        expect("(");
        node->cond = expr();
        expect(")");
        node->then = stmt();
    }
    else if (token->kind == TK_FOR){
        token = token->next;
        node = calloc(1,sizeof(Node));
        node->kind = ND_FOR;
        expect("(");
        if (!consume(";")){
            node->fst = expr();
            expect(";");
        }
        if (!consume(";")){
            node->cond = expr();
            expect(";");
        }
        if (!consume(")")){
            node->nxt = expr();
            expect(")");
        }
        node->then = stmt();
    }
    else if (consume("{")){
        node = calloc(1,sizeof(Node));
        node->kind = ND_BLOCK;
        Node head = {};
        Node *cur = &head;
        while(!consume("}")){
            cur->next = stmt();
            cur = cur->next;
        }
        node->block = head.next;
    }
    else{
        node = expr();
        expect(";");
    }
    return node;
}

Node *expr(){
    return assign();
}

Node *assign(){
    Node *node = equality();
    if (consume("=")){
        node = new_node(ND_ASSIGN,node,assign());
    }
    return node;
}

Node *equality(){
    Node *node = relational();
    while(true){
        if (consume("==")) node = new_node(ND_EQ,node,relational());
        else if (consume("!=")) node = new_node(ND_NE,node,relational());
        else return node;
    }
}

Node *relational(){
    Node *node = add();
    while(true){
        if (consume("<=")) node = new_node(ND_LE,node,add());
        else if (consume(">=")) node = new_node(ND_LE,add(),node);
        else if (consume("<")) node = new_node(ND_LT,node,add());
        else if (consume(">")) node = new_node(ND_LT,add(),node);
        else return node;
    }
}

Node *add(){
    Node *node = mul();
    while(true){
        if (consume("+")) node = new_node(ND_ADD,node,mul());
        else if (consume("-")) node = new_node(ND_SUB,node,mul());
        else return node;
    }
}

Node *mul(){
    Node *node = unary();
    while(true){
        if (consume("*")) node = new_node(ND_MUL,node,unary());
        else if (consume("/")) node = new_node(ND_DIV,node,unary());
        else return node;
    }
}

Node *unary(){
    if (consume("+")) return primary();
    if (consume("-")) return new_node(ND_SUB,new_node_num(0),primary());
    return primary();
}

Node *primary(){
    if (consume("(")){
        Node *node = expr();
        expect(")");
        return node;
    }
    Token *tok = consume_ident();
    if (tok){
        Node *node = calloc(1,sizeof(Node));
        node->kind = ND_LVAR;
        
        LVar *lvar = find_lvar(tok);
        if (lvar){
            node->offset = lvar->offset;
        }
        else{
            lvar = calloc(1,sizeof(LVar));
            lvar->next = locals;
            lvar->name = tok->str;
            lvar->len = tok->len;
            lvar->offset = locals->offset+8;
            node->offset = lvar->offset;
            locals = lvar;
        }
        return node;
    }
    return new_node_num(expect_number());
}