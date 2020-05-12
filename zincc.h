#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Tokenizer
//

typedef enum {
	TK_RESERVED, // Keywords or punctuators
	TK_IDENT,    // Identifiers
	TK_NUM,      // Integer literals
	TK_EOF,      // End-of-file markers
} TokenKind;

// Token type
typedef struct Token Token;
struct Token {
	TokenKind kind; // Token kind
	Token *next;    // Next token
	long val;       // If kind is TK_NUM, its value
	char *loc;      // Token location
	int len;        // Token length
};

void error(char *fmt, ...);
void error_tok(Token *tok, char *fmt, ...);
bool equal(Token *tok, char *op);
Token *skip(Token *tok, char *op);
Token *tokenize(char *input);

//
// Parser
//

typedef enum {
	ND_ADD,       // +
	ND_SUB,       // -
	ND_MUL,       // *
	ND_DIV,       // /
	ND_EQ,        // ==
	ND_NE,        // !=
	ND_LT,        // <
	ND_LE,        // <=
	ND_ASSIGN,    // =
	ND_RETURN,    // "return"
	ND_EXPR_STMT, // Expression statement
	ND_VAR,       // Variable
	ND_NUM,       // Integer
} NodeKind;

// AST node type
typedef struct Node Node;
struct Node {
	NodeKind kind; // Node kind
	Node *next;    // Next node
	Node *lhs;     // Left-hand side
	Node *rhs;     // Right-hand side
	char name;     // Used if kind == ND_VAR
	long val;      // Used if kind == ND_NUM
};

Node *parse(Token *tok);

//
// Code generator
//

void codegen(Node *node);
