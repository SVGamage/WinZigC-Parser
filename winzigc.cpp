#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <map>
#include <sstream>

using namespace std;

class ASTNode {
public:
    string nodeType;
    vector<ASTNode*> children;
    string value;
    
    ASTNode(const string& type, const string& val = "") 
        : nodeType(type), value(val) {}
    
    void addChild(ASTNode* child) {
        if (child) children.push_back(child);
    }
    
    void print(int depth = 0, bool isLast = false) const {
        // Print indentation
        for (int i = 0; i < depth; i++) {
            cout << ". ";
        }
        
        // Print node type and child count
        cout << nodeType << "(" << children.size() << ")";
        
        // Print value if it's a leaf node
        if (children.empty() && !value.empty()) {
            cout << "\n";
            for (int i = 0; i <= depth; i++) {
                cout << ". ";
            }
            cout << value << "(0)";
        }
        
        // Print children
        for (size_t i = 0; i < children.size(); i++) {
            cout << "\n";
            children[i]->print(depth + 1, false);
        }
        
        // Only add final newline if this is not the root node or not the last element
        if (depth == 0 && isLast) {
            // Don't add newline for the root node when it's the last
        } else if (depth > 0) {
            // For non-root nodes, newlines are handled by the parent
        }
    }
};

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
    string value;
    int line;
    int column;
    
    Token(TokenType t = TOK_UNKNOWN, const string& v = "", int l = 1, int c = 1)
        : type(t), value(v), line(l), column(c) {}
};

class Lexer {
private:
    string input;
    size_t pos;
    int line;
    int column;
    map<string, TokenType> keywords;
    
    void initKeywords() {
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
    
    char peek(int offset = 0) {
        if (pos + offset >= input.length()) return '\0';
        return input[pos + offset];
    }
    
    char advance() {
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
    
    void skipWhitespace() {
        while (pos < input.length() && isspace(peek()) && peek() != '\n') {
            advance();
        }
    }
    
    void skipComment() {
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
    
    Token readIdentifier() {
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
    
    Token readNumber() {
        string value;
        int startLine = line, startCol = column;
        
        while (pos < input.length() && isdigit(peek())) {
            value += advance();
        }
        
        return Token(TOK_INTEGER, value, startLine, startCol);
    }
    
    Token readChar() {
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
    
    Token readString() {
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
    
public:
    Lexer(const string& text) : input(text), pos(0), line(1), column(1) {
        initKeywords();
    }
    
    Token nextToken() {
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
};

class Parser {
private:
    Lexer lexer;
    Token currentToken;
    
    void advance() {
        do {
            currentToken = lexer.nextToken();
        } while (currentToken.type == TOK_NEWLINE); // Skip newlines
    }
    
    bool match(TokenType type) {
        return currentToken.type == type;
    }
    
    bool consume(TokenType type) {
        if (match(type)) {
            advance();
            return true;
        }
        return false;
    }
    
    ASTNode* createIdentifierNode(const string& name) {
        ASTNode* node = new ASTNode("<identifier>");
        node->addChild(new ASTNode(name));
        return node;
    }
    
    ASTNode* createIntegerNode(const string& value) {
        ASTNode* node = new ASTNode("<integer>");
        node->addChild(new ASTNode(value));
        return node;
    }
    
    ASTNode* createCharNode(const string& value) {
        ASTNode* node = new ASTNode("<char>");
        node->addChild(new ASTNode(value));
        return node;
    }
    
    ASTNode* createStringNode(const string& value) {
        ASTNode* node = new ASTNode("<string>");
        node->addChild(new ASTNode(value));
        return node;
    }

public:
    Parser(const string& input) : lexer(input) {
        advance(); // Get first token
    }
    
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
        string name = currentToken.value;
        advance();
        return createIdentifierNode(name);
    } else if (match(TOK_INTEGER_TYPE)) {
        string name = currentToken.value;
        advance();
        return createIdentifierNode(name);
    } else if (match(TOK_BOOLEAN)) {
        string name = currentToken.value;
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
        string value = currentToken.value;
        advance();
        return createIntegerNode(value);
    } else if (match(TOK_CHAR)) {
        string value = currentToken.value;
        advance();
        return createCharNode(value);
    } else if (match(TOK_IDENTIFIER)) {
        return parseName();
    } else if (match(TOK_TRUE)) {
        string value = currentToken.value;
        advance();
        return createIdentifierNode(value);
    } else if (match(TOK_FALSE)) {
        string value = currentToken.value;
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
        
        string op;
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
        string op;
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
        string op;
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
        string value = currentToken.value;
        advance();
        return createIntegerNode(value);
    }
    
    if (match(TOK_CHAR)) {
        string value = currentToken.value;
        advance();
        return createCharNode(value);
    }
    
    if (match(TOK_STRING)) {
        string value = currentToken.value;
        advance();
        return createStringNode(value);
    }
    
    if (match(TOK_TRUE) || match(TOK_FALSE)) {
        string value = currentToken.value;
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
        string name = currentToken.value;
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
        string name = currentToken.value;
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
        string name = currentToken.value;
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
        string value = currentToken.value;
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

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " -ast <filename>" << endl;
        return 1;
    }
    
    string flag = argv[1];
    string filename = argv[2];
    
    if (flag != "-ast") {
        cerr << "Only -ast flag is supported" << endl;
        return 1;
    }
    
    // Read input file
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return 1;
    }
    
    string input;
    string line;
    while (getline(file, line)) {
        input += line + "\n";
    }
    file.close();
    
    try {
        // Parse the input
        Parser parser(input);
        ASTNode* ast = parser.parseProgram();
        
        if (ast) {
            // Print the AST
            ast->print(0, true);
            cout << endl; // Add final newline to match expected output
        } else {
            cerr << "Parse error" << endl;
            return 1;
        }
        
        // Clean up memory (in practice, you'd want proper RAII)
        delete ast;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}