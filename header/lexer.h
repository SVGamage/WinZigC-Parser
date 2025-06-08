#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <map>
#include "token.h"

class Lexer {
private:
    std::string input;
    size_t pos;
    int line;
    int column;
    std::map<std::string, TokenType> keywords;
    
    void initKeywords();
    char peek(int offset = 0);
    char advance();
    void skipWhitespace();
    void skipComment();
    Token readIdentifier();
    Token readNumber();
    Token readChar();
    Token readString();
    
public:
    Lexer(const std::string& text);
    Token nextToken();
};

#endif // LEXER_H
