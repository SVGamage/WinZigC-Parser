#include "lexer.h"
#include <cctype>

using namespace std;

Lexer::Lexer(const string& text) : input(text), pos(0), line(1), column(1) {
    initKeywords();
}

void Lexer::initKeywords() {
    keywords["program"] = TOK_PROGRAM;
    keywords["var"] = TOK_VAR;
    keywords["const"] = TOK_CONST;
    keywords["type"] = TOK_TYPE;
    keywords["function"] = TOK_FUNCTION;
    keywords["return"] = TOK_RETURN;
    keywords["begin"] = TOK_BEGIN;
    keywords["end"] = TOK_END;
    keywords["if"] = TOK_IF;
    keywords["then"] = TOK_THEN;
    keywords["else"] = TOK_ELSE;
    keywords["while"] = TOK_WHILE;
    keywords["do"] = TOK_DO;
    keywords["case"] = TOK_CASE;
    keywords["of"] = TOK_OF;
    keywords["otherwise"] = TOK_OTHERWISE;
    keywords["repeat"] = TOK_REPEAT;
    keywords["until"] = TOK_UNTIL;
    keywords["for"] = TOK_FOR;
    keywords["loop"] = TOK_LOOP;
    keywords["pool"] = TOK_POOL;
    keywords["exit"] = TOK_EXIT;
    keywords["read"] = TOK_READ;
    keywords["output"] = TOK_OUTPUT;
    keywords["and"] = TOK_AND;
    keywords["or"] = TOK_OR;
    keywords["not"] = TOK_NOT;
    keywords["mod"] = TOK_MOD;
    keywords["succ"] = TOK_SUCC;
    keywords["pred"] = TOK_PRED;
    keywords["chr"] = TOK_CHR;
    keywords["ord"] = TOK_ORD;
    keywords["eof"] = TOK_EOF_KW;
    keywords["true"] = TOK_TRUE;
    keywords["false"] = TOK_FALSE;
    keywords["boolean"] = TOK_BOOLEAN;
    keywords["integer"] = TOK_INTEGER_TYPE;
}

char Lexer::peek(int offset) {
    if (pos + offset >= input.length()) return '\0';
    return input[pos + offset];
}

char Lexer::advance() {
    if (pos >= input.length()) return '\0';
    char c = input[pos++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

void Lexer::skipWhitespace() {
    while (pos < input.length() && isspace(peek()) && peek() != '\n') {
        advance();
    }
}

void Lexer::skipComment() {
    if (peek() == '#') {
        // Line comment
        while (pos < input.length() && peek() != '\n') {
            advance();
        }
    } else if (peek() == '{') {
        // Block comment
        advance(); // skip '{'
        while (pos < input.length()) {
            if (peek() == '}') {
                advance(); // skip '}'
                break;
            }
            advance();
        }
    }
}

Token Lexer::readIdentifier() {
    string value;
    int startLine = line, startCol = column;
    
    while (pos < input.length() && (isalnum(peek()) || peek() == '_')) {
        value += advance();
    }
    
    TokenType type = TOK_IDENTIFIER;
    if (keywords.find(value) != keywords.end()) {
        type = keywords[value];
    }
    
    return Token(type, value, startLine, startCol);
}

Token Lexer::readNumber() {
    string value;
    int startLine = line, startCol = column;
    
    while (pos < input.length() && isdigit(peek())) {
        value += advance();
    }
    
    return Token(TOK_INTEGER, value, startLine, startCol);
}

Token Lexer::readChar() {
    int startLine = line, startCol = column;
    advance(); // skip opening '
    
    if (pos >= input.length()) {
        return Token(TOK_UNKNOWN, "", startLine, startCol);
    }
    
    char c = advance();
    string value = "'";
    value += c;
    value += "'";
    
    if (pos < input.length() && peek() == '\'') {
        advance(); // skip closing '
    }
    
    return Token(TOK_CHAR, value, startLine, startCol);
}

Token Lexer::readString() {
    string value;
    int startLine = line, startCol = column;
    
    advance(); // skip opening "
    value += '"';
    
    while (pos < input.length() && peek() != '"') {
        value += advance();
    }
    
    if (pos < input.length() && peek() == '"') {
        value += advance(); // include closing "
    }
    
    return Token(TOK_STRING, value, startLine, startCol);
}

Token Lexer::nextToken() {
    while (pos < input.length()) {
        skipWhitespace();
        
        if (pos >= input.length()) break;
        
        char c = peek();
        
        // Handle comments
        if (c == '#' || c == '{') {
            skipComment();
            continue;
        }
        
        // Handle newlines
        if (c == '\n') {
            Token tok(TOK_NEWLINE, "\\n", line, column);
            advance();
            return tok;
        }
        
        // Handle identifiers and keywords
        if (isalpha(c) || c == '_') {
            return readIdentifier();
        }
        
        // Handle numbers
        if (isdigit(c)) {
            return readNumber();
        }
        
        // Handle characters
        if (c == '\'') {
            return readChar();
        }
        
        // Handle strings
        if (c == '"') {
            return readString();
        }
        
        // Handle two-character operators
        if (c == ':' && peek(1) == '=') {
            if (peek(2) == ':') {
                advance(); advance(); advance();
                return Token(TOK_SWAP, ":=:", line, column - 3);
            } else {
                advance(); advance();
                return Token(TOK_ASSIGN, ":=", line, column - 2);
            }
        }
        
        if (c == '<' && peek(1) == '=') {
            advance(); advance();
            return Token(TOK_LESS_EQUAL, "<=", line, column - 2);
        }
        
        if (c == '>' && peek(1) == '=') {
            advance(); advance();
            return Token(TOK_GREATER_EQUAL, ">=", line, column - 2);
        }
        
        if (c == '<' && peek(1) == '>') {
            advance(); advance();
            return Token(TOK_NOT_EQUAL, "<>", line, column - 2);
        }
        
        if (c == '.' && peek(1) == '.') {
            advance(); advance();
            return Token(TOK_DOTS, "..", line, column - 2);
        }
        
        // Handle single-character operators and punctuation
        Token tok(TOK_UNKNOWN, string(1, c), line, column);
        advance();
        
        switch (c) {
            case '+': tok.type = TOK_PLUS; break;
            case '-': tok.type = TOK_MINUS; break;
            case '*': tok.type = TOK_MULTIPLY; break;
            case '/': tok.type = TOK_DIVIDE; break;
            case '=': tok.type = TOK_EQUAL; break;
            case '<': tok.type = TOK_LESS; break;
            case '>': tok.type = TOK_GREATER; break;
            case ';': tok.type = TOK_SEMICOLON; break;
            case ':': tok.type = TOK_COLON; break;
            case ',': tok.type = TOK_COMMA; break;
            case '.': tok.type = TOK_DOT; break;
            case '(': tok.type = TOK_LPAREN; break;
            case ')': tok.type = TOK_RPAREN; break;
            case '{': tok.type = TOK_LBRACE; break;
            case '}': tok.type = TOK_RBRACE; break;
            default: tok.type = TOK_UNKNOWN; break;
        }
        
        return tok;
    }
    
    return Token(TOK_EOF, "", line, column);
}
