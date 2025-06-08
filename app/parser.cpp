#include "parser.h"
#include <iostream>

Parser::Parser(const std::string& input) : lexer(input) {
    advance(); // Get first token
}

void Parser::advance() {
    do {
        currentToken = lexer.nextToken();
    } while (currentToken.type == TOK_NEWLINE); // Skip newlines
}

bool Parser::match(TokenType type) {
    return currentToken.type == type;
}

bool Parser::consume(TokenType type) {
    if (match(type)) {
        advance();
        return true;
    }
    return false;
}

ASTNode* Parser::createIdentifierNode(const std::string& name) {
    ASTNode* node = new ASTNode("<identifier>");
    node->addChild(new ASTNode(name));
    return node;
}

ASTNode* Parser::createIntegerNode(const std::string& value) {
    ASTNode* node = new ASTNode("<integer>");
    node->addChild(new ASTNode(value));
    return node;
}

ASTNode* Parser::createCharNode(const std::string& value) {
    ASTNode* node = new ASTNode("<char>");
    node->addChild(new ASTNode(value));
    return node;
}

ASTNode* Parser::createStringNode(const std::string& value) {
    ASTNode* node = new ASTNode("<string>");
    node->addChild(new ASTNode(value));
    return node;
}

ASTNode* Parser::parseProgram() {
    ASTNode* program = new ASTNode("program");
    
    // 'program' Name ':' Consts Types Dclns SubProgs Body Name '.'
    if (!consume(TOK_PROGRAM)) return nullptr;
    
    program->addChild(parseName());
    
    if (!consume(TOK_COLON)) return nullptr;
    
    program->addChild(parseConsts());
    program->addChild(parseTypes());
    program->addChild(parseDclns());
    program->addChild(parseSubProgs());
    program->addChild(parseBody());
    program->addChild(parseName());
    
    consume(TOK_DOT);
    
    return program;
}

ASTNode* Parser::parseName() {
    if (match(TOK_IDENTIFIER)) {
        std::string name = currentToken.value;
        advance();
        return createIdentifierNode(name);
    } else if (match(TOK_INTEGER_TYPE)) {
        std::string name = currentToken.value;
        advance();
        return createIdentifierNode(name);
    } else if (match(TOK_BOOLEAN)) {
        std::string name = currentToken.value;
        advance();
        return createIdentifierNode(name);
    }
    return nullptr;
}

ASTNode* Parser::parseConsts() {
    ASTNode* consts = new ASTNode("consts");
    
    if (match(TOK_CONST)) {
        advance(); // consume 'const'
        
        // Parse const list
        do {
            consts->addChild(parseConst());
        } while (consume(TOK_COMMA));
        
        consume(TOK_SEMICOLON);
    }
    
    return consts;
}

ASTNode* Parser::parseConst() {
    ASTNode* constNode = new ASTNode("const");
    
    constNode->addChild(parseName()); // Name
    
    if (!consume(TOK_EQUAL)) return nullptr;
    
    constNode->addChild(parseConstValue()); // ConstValue
    
    return constNode;
}

ASTNode* Parser::parseConstValue() {
    if (match(TOK_INTEGER)) {
        std::string value = currentToken.value;
        advance();
        return createIntegerNode(value);
    } else if (match(TOK_CHAR)) {
        std::string value = currentToken.value;
        advance();
        return createCharNode(value);
    } else if (match(TOK_IDENTIFIER)) {
        return parseName();
    } else if (match(TOK_TRUE)) {
        std::string value = currentToken.value;
        advance();
        return createIdentifierNode(value);
    } else if (match(TOK_FALSE)) {
        std::string value = currentToken.value;
        advance();
        return createIdentifierNode(value);
    }
    return nullptr;
}

ASTNode* Parser::parseTypes() {
    ASTNode* types = new ASTNode("types");
    
    if (match(TOK_TYPE)) {
        advance(); // consume 'type'
        
        // Parse type list
        do {
            types->addChild(parseType());
            consume(TOK_SEMICOLON);
        } while (match(TOK_IDENTIFIER));
    }
    
    return types;
}

ASTNode* Parser::parseType() {
    ASTNode* type = new ASTNode("type");
    
    type->addChild(parseName()); // Name
    
    if (!consume(TOK_EQUAL)) return nullptr;
    
    type->addChild(parseLitList()); // LitList
    
    return type;
}

ASTNode* Parser::parseLitList() {
    ASTNode* lit = new ASTNode("lit");
    
    if (!consume(TOK_LPAREN)) return nullptr;
    
    // Parse name list
    do {
        lit->addChild(parseName());
    } while (consume(TOK_COMMA));
    
    consume(TOK_RPAREN);
    
    return lit;
}

ASTNode* Parser::parseDclns() {
    ASTNode* dclns = new ASTNode("dclns");
    
    if (match(TOK_VAR)) {
        advance(); // consume 'var'
        
        // Parse declaration list
        do {
            dclns->addChild(parseDcln());
            consume(TOK_SEMICOLON);
        } while (match(TOK_IDENTIFIER) && !match(TOK_BEGIN) && !match(TOK_FUNCTION) && !match(TOK_END));
    }
    
    return dclns;
}

ASTNode* Parser::parseDcln() {
    ASTNode* var = new ASTNode("var");
    
    // Parse name list
    do {
        var->addChild(parseName());
    } while (consume(TOK_COMMA));
    
    if (!consume(TOK_COLON)) return nullptr;
    
    var->addChild(parseName()); // Type name
    
    return var;
}

ASTNode* Parser::parseExpression() {
    ASTNode* left = parseTerm();
    
    while (match(TOK_LESS_EQUAL) || match(TOK_LESS) || match(TOK_GREATER_EQUAL) || 
           match(TOK_GREATER) || match(TOK_EQUAL) || match(TOK_NOT_EQUAL)) {
        
        std::string op;
        switch (currentToken.type) {
            case TOK_LESS_EQUAL: op = "<="; break;
            case TOK_LESS: op = "<"; break;
            case TOK_GREATER_EQUAL: op = ">="; break;
            case TOK_GREATER: op = ">"; break;
            case TOK_EQUAL: op = "="; break;
            case TOK_NOT_EQUAL: op = "<>"; break;
            default: break;
        }
        
        advance();
        ASTNode* right = parseTerm();
        
        ASTNode* opNode = new ASTNode(op);
        opNode->addChild(left);
        opNode->addChild(right);
        left = opNode;
    }
    
    return left;
}

ASTNode* Parser::parseTerm() {
    ASTNode* left = parseFactor();
    
    while (match(TOK_PLUS) || match(TOK_MINUS) || match(TOK_OR)) {
        std::string op;
        switch (currentToken.type) {
            case TOK_PLUS: op = "+"; break;
            case TOK_MINUS: op = "-"; break;
            case TOK_OR: op = "or"; break;
            default: break;
        }
        
        advance();
        ASTNode* right = parseFactor();
        
        ASTNode* opNode = new ASTNode(op);
        opNode->addChild(left);
        opNode->addChild(right);
        left = opNode;
    }
    
    return left;
}

ASTNode* Parser::parseFactor() {
    ASTNode* left = parsePrimary();
    
    while (match(TOK_MULTIPLY) || match(TOK_DIVIDE) || match(TOK_AND) || match(TOK_MOD)) {
        std::string op;
        switch (currentToken.type) {
            case TOK_MULTIPLY: op = "*"; break;
            case TOK_DIVIDE: op = "/"; break;
            case TOK_AND: op = "and"; break;
            case TOK_MOD: op = "mod"; break;
            default: break;
        }
        
        advance();
        ASTNode* right = parsePrimary();
        
        ASTNode* opNode = new ASTNode(op);
        opNode->addChild(left);
        opNode->addChild(right);
        left = opNode;
    }
    
    return left;
}

ASTNode* Parser::parsePrimary() {
    // Handle unary operators
    if (match(TOK_MINUS)) {
        advance();
        ASTNode* unaryMinus = new ASTNode("-");
        unaryMinus->addChild(parsePrimary());
        return unaryMinus;
    }
    
    if (match(TOK_PLUS)) {
        advance();
        return parsePrimary(); // Unary plus is ignored
    }
    
    if (match(TOK_NOT)) {
        advance();
        ASTNode* notNode = new ASTNode("not");
        notNode->addChild(parsePrimary());
        return notNode;
    }
    
    // Handle built-in functions
    if (match(TOK_SUCC)) {
        advance();
        consume(TOK_LPAREN);
        ASTNode* succNode = new ASTNode("succ");
        succNode->addChild(parseExpression());
        consume(TOK_RPAREN);
        return succNode;
    }
    
    if (match(TOK_PRED)) {
        advance();
        consume(TOK_LPAREN);
        ASTNode* predNode = new ASTNode("pred");
        predNode->addChild(parseExpression());
        consume(TOK_RPAREN);
        return predNode;
    }
    
    if (match(TOK_CHR)) {
        advance();
        consume(TOK_LPAREN);
        ASTNode* chrNode = new ASTNode("chr");
        chrNode->addChild(parseExpression());
        consume(TOK_RPAREN);
        return chrNode;
    }
    
    if (match(TOK_ORD)) {
        advance();
        consume(TOK_LPAREN);
        ASTNode* ordNode = new ASTNode("ord");
        ordNode->addChild(parseExpression());
        consume(TOK_RPAREN);
        return ordNode;
    }
    
    if (match(TOK_EOF_KW)) {
        advance();
        return new ASTNode("eof");
    }
    
    // Handle literals
    if (match(TOK_INTEGER)) {
        std::string value = currentToken.value;
        advance();
        return createIntegerNode(value);
    }
    
    if (match(TOK_CHAR)) {
        std::string value = currentToken.value;
        advance();
        return createCharNode(value);
    }
    
    if (match(TOK_STRING)) {
        std::string value = currentToken.value;
        advance();
        return createStringNode(value);
    }
    
    if (match(TOK_TRUE) || match(TOK_FALSE)) {
        std::string value = currentToken.value;
        advance();
        return createIdentifierNode(value);
    }
    
    // Handle parenthesized expressions
    if (match(TOK_LPAREN)) {
        advance();
        ASTNode* expr = parseExpression();
        consume(TOK_RPAREN);
        return expr;
    }
    
    // Handle identifiers and function calls
    if (match(TOK_IDENTIFIER)) {
        std::string name = currentToken.value;
        advance();
        
        // Check for function call
        if (match(TOK_LPAREN)) {
            advance();
            ASTNode* call = new ASTNode("call");
            call->addChild(createIdentifierNode(name));
            
            // Parse argument list
            if (!match(TOK_RPAREN)) {
                do {
                    call->addChild(parseExpression());
                } while (consume(TOK_COMMA));
            }
            
            consume(TOK_RPAREN);
            return call;
        } else {
            return createIdentifierNode(name);
        }
    }
    
    return nullptr;
}

ASTNode* Parser::parseSubProgs() {
    ASTNode* subprogs = new ASTNode("subprogs");
    
    while (match(TOK_FUNCTION)) {
        subprogs->addChild(parseFcn());
    }
    
    return subprogs;
}

ASTNode* Parser::parseFcn() {
    ASTNode* fcn = new ASTNode("fcn");
    
    consume(TOK_FUNCTION); // 'function'
    fcn->addChild(parseName()); // function name
    
    consume(TOK_LPAREN);
    fcn->addChild(parseParams()); // parameters
    consume(TOK_RPAREN);
    
    consume(TOK_COLON);
    fcn->addChild(parseName()); // return type
    
    consume(TOK_SEMICOLON);
    
    fcn->addChild(parseConsts()); // local constants
    fcn->addChild(parseTypes());  // local types
    fcn->addChild(parseDclns());  // local declarations
    fcn->addChild(parseBody());   // function body
    fcn->addChild(parseName());   // function name again
    
    consume(TOK_SEMICOLON);
    
    return fcn;
}

ASTNode* Parser::parseParams() {
    ASTNode* params = new ASTNode("params");
    
    if (!match(TOK_RPAREN)) {
        do {
            params->addChild(parseDcln());
        } while (consume(TOK_SEMICOLON));
    }
    
    return params;
}

ASTNode* Parser::parseBody() {
    ASTNode* block = new ASTNode("block");
    
    consume(TOK_BEGIN);
    
    // Parse statement list
    if (!match(TOK_END)) {
        while (!match(TOK_END)) {
            ASTNode* stmt = parseStatement();
            if (stmt) {
                block->addChild(stmt);
            } else {
                // Add null statement for empty statements (consecutive semicolons)
                block->addChild(new ASTNode("<null>"));
            }
            
            if (!consume(TOK_SEMICOLON)) {
                // If no semicolon, we need to add a null statement at the end
                // because the grammar expects 'Statement list ;'
                if (!match(TOK_END)) {
                    block->addChild(new ASTNode("<null>"));
                }
                break;
            }
            
            // If we consumed a semicolon but are at END, add null statement
            if (match(TOK_END)) {
                block->addChild(new ASTNode("<null>"));
                break;
            }
        }
    }
    
    consume(TOK_END);
    
    return block;
}

ASTNode* Parser::parseStatement() {
    // Assignment or swap
    if (match(TOK_IDENTIFIER)) {
        std::string name = currentToken.value;
        advance();
        
        if (match(TOK_ASSIGN)) {
            advance();
            ASTNode* assign = new ASTNode("assign");
            assign->addChild(createIdentifierNode(name));
            assign->addChild(parseExpression());
            return assign;
        } else if (match(TOK_SWAP)) {
            advance();
            ASTNode* swap = new ASTNode("swap");
            swap->addChild(createIdentifierNode(name));
            swap->addChild(parseName());
            return swap;
        }
        // If neither assignment nor swap, rewind token - this is a simplified approach
        // In a real parser, you'd need proper lookahead
        // For now, we'll just return nullptr to indicate parsing failed for this path
        return nullptr;
    }
    
    // Output statement
    if (match(TOK_OUTPUT)) {
        advance();
        consume(TOK_LPAREN);
        
        ASTNode* output = new ASTNode("output");
        
        do {
            output->addChild(parseOutExp());
        } while (consume(TOK_COMMA));
        
        consume(TOK_RPAREN);
        return output;
    }
    
    // If statement
    if (match(TOK_IF)) {
        advance();
        ASTNode* ifNode = new ASTNode("if");
        
        ifNode->addChild(parseExpression()); // condition
        
        consume(TOK_THEN);
        ifNode->addChild(parseStatement()); // then statement
        
        if (match(TOK_ELSE)) {
            advance();
            ifNode->addChild(parseStatement()); // else statement
        }
        
        return ifNode;
    }
    
    // While statement
    if (match(TOK_WHILE)) {
        advance();
        ASTNode* whileNode = new ASTNode("while");
        
        whileNode->addChild(parseExpression()); // condition
        
        consume(TOK_DO);
        whileNode->addChild(parseStatement()); // body
        
        return whileNode;
    }
    
    // Repeat statement
    if (match(TOK_REPEAT)) {
        advance();
        ASTNode* repeatNode = new ASTNode("repeat");
        
        // Parse statement list
        do {
            repeatNode->addChild(parseStatement());
        } while (consume(TOK_SEMICOLON) && !match(TOK_UNTIL));
        
        consume(TOK_UNTIL);
        repeatNode->addChild(parseExpression()); // condition
        
        return repeatNode;
    }
    
    // For statement
    if (match(TOK_FOR)) {
        advance();
        consume(TOK_LPAREN);
        
        ASTNode* forNode = new ASTNode("for");
        
        forNode->addChild(parseForStat()); // initialization
        consume(TOK_SEMICOLON);
        
        forNode->addChild(parseForExp()); // condition
        consume(TOK_SEMICOLON);
        
        forNode->addChild(parseForStat()); // increment
        consume(TOK_RPAREN);
        
        forNode->addChild(parseStatement()); // body
        
        return forNode;
    }
    
    // Loop statement
    if (match(TOK_LOOP)) {
        advance();
        ASTNode* loopNode = new ASTNode("loop");
        
        do {
            loopNode->addChild(parseStatement());
        } while (consume(TOK_SEMICOLON) && !match(TOK_POOL));
        
        consume(TOK_POOL);
        return loopNode;
    }
    
    // Case statement
    if (match(TOK_CASE)) {
        advance();
        ASTNode* caseNode = new ASTNode("case");
        
        caseNode->addChild(parseExpression()); // case expression
        
        consume(TOK_OF);
        
        // Parse case clauses
        while (!match(TOK_END) && !match(TOK_OTHERWISE)) {
            ASTNode* clause = parseCaseclause();
            if (clause) {
                caseNode->addChild(clause);
            }
            
            // Try to consume semicolon, but don't require it
            consume(TOK_SEMICOLON);
            
            // Safety check - if we haven't advanced, break to avoid infinite loop
            if (!match(TOK_END) && !match(TOK_OTHERWISE) && clause == nullptr) {
                break;
            }
        }
        
        // Parse otherwise clause
        if (match(TOK_OTHERWISE)) {
            caseNode->addChild(parseOtherwiseClause());
        }
        
        consume(TOK_END);
        return caseNode;
    }
    
    // Read statement
    if (match(TOK_READ)) {
        advance();
        consume(TOK_LPAREN);
        
        ASTNode* read = new ASTNode("read");
        
        do {
            read->addChild(parseName());
        } while (consume(TOK_COMMA));
        
        consume(TOK_RPAREN);
        return read;
    }
    
    // Exit statement
    if (match(TOK_EXIT)) {
        advance();
        return new ASTNode("exit");
    }
    
    // Return statement
    if (match(TOK_RETURN)) {
        advance();
        ASTNode* returnNode = new ASTNode("return");
        returnNode->addChild(parseExpression());
        return returnNode;
    }
    
    // Block statement
    if (match(TOK_BEGIN)) {
        return parseBody();
    }
    
    // Empty statement
    return new ASTNode("<null>");
}

ASTNode* Parser::parseForStat() {
    if (match(TOK_IDENTIFIER)) {
        return parseAssignment();
    }
    return new ASTNode("<null>");
}

ASTNode* Parser::parseForExp() {
    if (!match(TOK_SEMICOLON)) {
        return parseExpression();
    }
    return new ASTNode("true");
}

ASTNode* Parser::parseAssignment() {
    if (match(TOK_IDENTIFIER)) {
        std::string name = currentToken.value;
        advance();
        
        if (match(TOK_ASSIGN)) {
            advance();
            ASTNode* assign = new ASTNode("assign");
            assign->addChild(createIdentifierNode(name));
            assign->addChild(parseExpression());
            return assign;
        } else if (match(TOK_SWAP)) {
            advance();
            ASTNode* swap = new ASTNode("swap");
            swap->addChild(createIdentifierNode(name));
            swap->addChild(parseName());
            return swap;
        }
    }
    return nullptr;
}

ASTNode* Parser::parseOutExp() {
    if (match(TOK_STRING)) {
        std::string value = currentToken.value;
        advance();
        ASTNode* stringNode = new ASTNode("string");
        stringNode->addChild(createStringNode(value));
        return stringNode;
    } else {
        ASTNode* integerNode = new ASTNode("integer");
        integerNode->addChild(parseExpression());
        return integerNode;
    }
}

ASTNode* Parser::parseCaseclauses() {
    // This is handled in parseStatement for case
    return nullptr;
}

ASTNode* Parser::parseCaseclause() {
    ASTNode* clause = new ASTNode("case_clause");
    
    clause->addChild(parseCaseExpression());
    
    consume(TOK_COLON);
    
    clause->addChild(parseStatement());
    
    return clause;
}

ASTNode* Parser::parseCaseExpression() {
    ASTNode* left = parseConstValue();
    
    if (match(TOK_DOTS)) {
        advance();
        ASTNode* range = new ASTNode("..");
        range->addChild(left);
        range->addChild(parseConstValue());
        return range;
    }
    
    return left;
}

ASTNode* Parser::parseOtherwiseClause() {
    advance(); // consume 'otherwise'
    ASTNode* otherwise = new ASTNode("otherwise");
    otherwise->addChild(parseStatement());
    return otherwise;
}
