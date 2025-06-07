#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "token.h"
#include "lexer.h"
#include "ast_node.h"

class Parser {
private:
    Lexer lexer;
    Token currentToken;
    
    void advance();
    bool match(TokenType type);
    bool consume(TokenType type);
    
    ASTNode* createIdentifierNode(const std::string& name);
    ASTNode* createIntegerNode(const std::string& value);
    ASTNode* createCharNode(const std::string& value);
    ASTNode* createStringNode(const std::string& value);

public:
    Parser(const std::string& input);
    
    // Forward declarations for parsing functions
    ASTNode* parseProgram();
    ASTNode* parseConsts();
    ASTNode* parseConst();
    ASTNode* parseConstValue();
    ASTNode* parseTypes();
    ASTNode* parseType();
    ASTNode* parseLitList();
    ASTNode* parseDclns();
    ASTNode* parseDcln();
    ASTNode* parseSubProgs();
    ASTNode* parseFcn();
    ASTNode* parseParams();
    ASTNode* parseBody();
    ASTNode* parseStatement();
    ASTNode* parseAssignment();
    ASTNode* parseExpression();
    ASTNode* parseTerm();
    ASTNode* parseFactor();
    ASTNode* parsePrimary();
    ASTNode* parseName();
    ASTNode* parseForStat();
    ASTNode* parseForExp();
    ASTNode* parseOutExp();
    ASTNode* parseCaseclauses();
    ASTNode* parseCaseclause();
    ASTNode* parseCaseExpression();
    ASTNode* parseOtherwiseClause();
};

#endif // PARSER_H
