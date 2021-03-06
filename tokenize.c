#include "zincc.h"

// Input string
static char *current_input;

// Reports an error and exit.
void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

// Reports an error location and exit.
static void verror_at(char *loc, char *fmt, va_list ap) {
	int pos = loc - current_input;
	fprintf(stderr, "%s\n", current_input);
	fprintf(stderr, "%*s", pos, ""); // print pos spaces.
	fprintf(stderr, "^ ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

static void error_at(char *loc, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	verror_at(loc, fmt, ap);
}

void error_tok(Token *tok, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	verror_at(tok->loc, fmt, ap);
}

// Consumes the current token if it matches `s`.
bool equal(Token *tok, char *s) {
	return strlen(s) == tok->len &&
		     !strncmp(tok->loc, s, tok->len);
}

// Ensure that the current token is `s`.
Token *skip(Token *tok, char *s) {
	if (!equal(tok, s))
		error("expected '%s'", s);
	return tok->next;
}

// Create a new token and add it as the next token of `cur`.
static Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->loc = str;
	tok->len = len;
	cur->next = tok;
	return tok;
}

static bool startswith(char *p, char *q) {
	return strncmp(p, q, strlen(q)) == 0;
}

static bool is_alpha(char c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

static bool is_alnum(char c) {
	return is_alpha(c) || ('0' <= c && c <= '9');
}

// Tokenize `current_input` and returns new tokens.
Token *tokenize(char *p) {
	current_input = p;
	Token head;
	Token *cur = &head;

	while (*p) {
		// Skip whitespace characters.
		if (isspace(*p)) {
			p++;
			continue;
		}

		// Keywords
		if (startswith(p, "return") && !is_alnum(p[6])) {
			cur = new_token(TK_RESERVED, cur, p, 6);
			p += 6;
			continue;
		}

		// Identifier
		if ('a' <= *p && *p <= 'z') {
			cur = new_token(TK_IDENT, cur, p++, 1);
			continue;
		}

		// Multi-letter punctuators
		if (startswith(p, "==") || startswith(p, "!=") ||
				startswith(p, "<=") || startswith(p, ">=")) {
				cur = new_token(TK_RESERVED, cur, p, 2);
				p += 2;
				continue;
		}

		// Single-letter punctuators
		if (ispunct(*p)) {
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		// Integer literal
		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p, 0);
			char *q = p;
			cur->val = strtoul(p, &p, 10);
			cur->len = p - q;
			continue;
		}

		error_at(p, "invalid token");
	}

	new_token(TK_EOF, cur, p, 0);
	return head.next;
}
