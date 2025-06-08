#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
    // Literals
    TOK_IDENTIFIER, TOK_INTEGER, TOK_CHAR, TOK_STRING,
    
    // Keywords
    TOK_PROGRAM, TOK_VAR, TOK_CONST, TOK_TYPE, TOK_FUNCTION, TOK_RETURN,
    TOK_BEGIN, TOK_END, TOK_IF, TOK_THEN, TOK_ELSE, TOK_WHILE, TOK_DO,
    TOK_CASE, TOK_OF, TOK_OTHERWISE, TOK_REPEAT, TOK_UNTIL, TOK_FOR,
    TOK_LOOP, TOK_POOL, TOK_EXIT, TOK_READ, TOK_OUTPUT,
    TOK_AND, TOK_OR, TOK_NOT, TOK_MOD, TOK_SUCC, TOK_PRED, TOK_CHR, TOK_ORD,
    TOK_EOF_KW, TOK_TRUE, TOK_FALSE, TOK_BOOLEAN, TOK_INTEGER_TYPE,
    
    // Operators
    TOK_ASSIGN, TOK_SWAP, TOK_PLUS, TOK_MINUS, TOK_MULTIPLY, TOK_DIVIDE,
    TOK_EQUAL, TOK_NOT_EQUAL, TOK_LESS, TOK_LESS_EQUAL, TOK_GREATER,
    TOK_GREATER_EQUAL, TOK_DOTS,
    
    // Punctuation
    TOK_SEMICOLON, TOK_COLON, TOK_COMMA, TOK_DOT, TOK_LPAREN, TOK_RPAREN,
    TOK_LBRACE, TOK_RBRACE,
    
    // Special
    TOK_EOF, TOK_NEWLINE, TOK_UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    int column;
    
    Token(TokenType t = TOK_UNKNOWN, const std::string& v = "", int l = 1, int c = 1)
        : type(t), value(v), line(l), column(c) {}
};

#endif // TOKEN_H
