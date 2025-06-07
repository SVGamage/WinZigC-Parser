# WinZigC Parser

A modular recursive descent parser for the WinZigC programming language implemented in C++. This parser features a clean separation of concerns with individual modules for lexical analysis, syntax parsing, and AST construction.

## Overview

WinZigC is a Pascal-like programming language that supports:

- Variable declarations and type definitions
- Functions with parameters and local scope
- Control structures (if/then/else, while, for, case, repeat/until, loop/pool)
- Built-in functions (succ, pred, chr, ord, eof)
- Input/output operations (read, output)
- Swap operations (`:=:`)
- Comments (both line `#` and block `{...}`)
- Basic data types (integer, boolean, char)
- Arithmetic and logical expressions

## Architecture

The parser is built with a modular architecture for maintainability and extensibility:

### Core Components

- **Lexer**: Tokenizes WinZigC source code into a stream of tokens
- **Parser**: Implements recursive descent parsing to build an AST
- **AST Builder**: Constructs and manages the Abstract Syntax Tree
- **Main Interface**: Command-line interface and file handling

### File Structure

```
WinZigC-Parser/
├── main.cpp                 # Entry point and CLI
├── token.h                  # Token definitions and types
├── ast_node.h              # AST node class declaration
├── ast_node.cpp            # AST node implementation
├── lexer.h                 # Lexer class declaration
├── lexer.cpp               # Lexical analyzer implementation
├── parser.h                # Parser class declaration
├── parser.cpp              # Recursive descent parser implementation
├── winzigc.cpp             # Original monolithic implementation (reference)
├── Makefile                # Multi-file build configuration
├── run_tests.sh           # Comprehensive test runner
├── simple_test.winzig     # Basic test program
└── winzig_test_programs/  # Complete test suite (15 programs)
    ├── winzig_01 ... winzig_15    # Test programs
    └── winzig_01.tree ... winzig_15.tree  # Expected outputs
```

## Module Dependencies

The modular design follows a clear dependency hierarchy:

```
token.h
  ↓
ast_node.h → ast_node.cpp
  ↓
lexer.h → lexer.cpp
  ↓
parser.h → parser.cpp
  ↓
main.cpp
```

This structure enables:

- **Incremental compilation**: Only modified modules need recompilation
- **Independent testing**: Each module can be tested in isolation
- **Code reusability**: Components can be used in other projects
- **Maintainability**: Clear separation of concerns

## Build System

### Prerequisites

- C++ compiler with C++11 support (g++, clang++)
- Make utility
- POSIX-compatible shell for test runner

### Compilation

```bash
# Build the complete project
make

# Clean build artifacts
make clean

# Rebuild everything
make clean && make
```

The Makefile supports:

- **Parallel compilation**: Uses object files for faster incremental builds
- **Dependency tracking**: Automatically handles header dependencies
- **Cross-platform compatibility**: Works on Linux, macOS, and Windows (with appropriate toolchain)

## Usage

The parser takes a WinZigC source file and generates an Abstract Syntax Tree representation:

```bash
./winzigc -ast <filename>
```

### Command Line Options

- `-ast <filename>`: Parse the specified file and output the AST tree structure

### Example Usage

```bash
# Parse a simple test program
./winzigc -ast simple_test.winzig

# Parse comprehensive test programs
./winzigc -ast winzig_test_programs/winzig_01
./winzigc -ast winzig_test_programs/winzig_15
```

### Sample Program

Here's a simple WinZigC program (`simple_test.winzig`):

```pascal
program simple:
var x: integer;
begin
    x := 5;
    output(x)
end simple.
```

**Generated AST Output:**

```
program(7)
. <identifier>(1)
. . simple(0)
. consts(0)
. types(0)
. dclns(1)
. . var(2)
. . . <identifier>(1)
. . . . x(0)
. . . <identifier>(1)
. . . . integer(0)
. subprogs(0)
. block(2)
. . assign(2)
. . . <identifier>(1)
. . . . x(0)
. . . <integer>(1)
. . . . 5(0)
. . output(1)
. . . integer(1)
. . . . <identifier>(1)
. . . . . x(0)
. <identifier>(1)
. . simple(0)
```

## Module Overview

### 1. Token System (`token.h`)

Defines the complete token vocabulary for WinZigC:

- **TokenType enum**: All language tokens (keywords, operators, literals, punctuation)
- **Token struct**: Token representation with type and value
- **Comprehensive coverage**: 50+ token types including all WinZigC constructs

### 2. AST Node (`ast_node.h/cpp`)

Manages the Abstract Syntax Tree structure:

- **Tree construction**: Dynamic node creation with parent-child relationships
- **Tree traversal**: Formatted output with indentation levels
- **Memory management**: Proper cleanup of dynamically allocated nodes
- **Debugging support**: Clear tree visualization for analysis

### 3. Lexical Analyzer (`lexer.h/cpp`)

Converts source code into tokens:

- **Keyword recognition**: Complete WinZigC keyword table
- **Multi-character operators**: Support for `:=`, `:=:`, `<=`, `>=`, `<>`, `..`
- **String/character literals**: Proper handling of quoted strings and character constants
- **Comment processing**: Both line (`#`) and block (`{...}`) comments
- **Number parsing**: Integer literal recognition
- **Identifier scanning**: Variable and function name tokenization

### 4. Parser (`parser.h/cpp`)

Implements recursive descent parsing:

- **Complete grammar coverage**: All WinZigC language constructs
- **Expression parsing**: Operator precedence and associativity
- **Error recovery**: Basic error detection and reporting
- **AST construction**: Builds semantic tree structure during parsing
- **30+ parsing methods**: Each grammar rule has dedicated parsing function

### 5. Main Interface (`main.cpp`)

Provides command-line interface:

- **File handling**: Input file reading and validation
- **Error reporting**: User-friendly error messages
- **Memory cleanup**: Proper AST deletion after use
- **Exit codes**: Standard success/failure reporting

## Language Grammar

The parser implements the complete WinZigC grammar specification:

### Program Structure

```
Program → 'program' Name ':' Consts Types Dclns SubProgs Body Name '.'
```

### Declarations

- **Constants**: `const name = value [, name = value]*;`
- **Types**: `type name = (literal_list) [; name = (literal_list)]*;`
- **Variables**: `var name_list : type [; name_list : type]*;`

### Functions

```
Function → 'function' Name '(' Params ')' ':' Name ';'
           Consts Types Dclns Body Name ';'
```

### Statements

- **Assignment**: `variable := expression`
- **Swap**: `variable :=: variable`
- **Output**: `output(expression [, expression]*)`
- **Read**: `read(variable [, variable]*)`
- **Control flow**: if/then/else, while/do, for, case/of, repeat/until, loop/pool
- **Function calls**: `name(argument_list)`

### Expressions

- **Operators**: `+`, `-`, `*`, `/`, `mod`, `and`, `or`, `not`
- **Comparisons**: `=`, `<>`, `<`, `<=`, `>`, `>=`
- **Built-ins**: `succ(expr)`, `pred(expr)`, `chr(expr)`, `ord(expr)`, `eof`
- **Precedence**: Proper operator precedence and associativity

## Testing

### Comprehensive Test Suite

The project includes a robust testing framework with 15 comprehensive test programs:

```bash
# Run all tests with detailed reporting
./run_tests.sh
```

**Sample Output:**

```
WinZigC Parser Test Runner
==========================

Testing winzig_01...
✓ PASS: winzig_01 matches expected output

Testing winzig_02...
✓ PASS: winzig_02 matches expected output

...

==========================
Test Summary:
Total tests: 15
Passed: 15
Failed: 0
All tests passed!
```

### Test Coverage

The test programs validate:

1. **Basic Programs** - Simple variable declarations and assignments
2. **Functions** - Function definitions, calls, parameters, and recursion
3. **Control Flow** - All control structures (if, while, for, case, repeat, loop)
4. **Expressions** - Complex arithmetic and logical expressions
5. **Data Types** - Integer, boolean, character, and user-defined types
6. **I/O Operations** - Read and output statements
7. **Advanced Features** - Nested scopes, complex case statements, swap operations

### Individual Test Execution

```bash
# Test specific programs
./winzigc -ast winzig_test_programs/winzig_01
./winzigc -ast winzig_test_programs/winzig_05  # Function tests
./winzigc -ast winzig_test_programs/winzig_10  # Control flow tests
```

### Custom Testing

```bash
# Test your own WinZigC programs
./winzigc -ast your_program.winzig

# Compare with expected output
./winzigc -ast your_program.winzig > actual.tree
diff actual.tree expected.tree
```

## Development

### Modular Development Benefits

The refactored architecture enables efficient development:

- **Isolated Changes**: Modify lexer, parser, or AST independently
- **Unit Testing**: Test individual components in isolation
- **Debugging**: Easier to trace issues to specific modules
- **Performance**: Incremental compilation reduces build times

### Code Organization

```cpp
// token.h - Clean token definitions
enum TokenType { TOK_PROGRAM, TOK_IDENTIFIER, ... };
struct Token { TokenType type; std::string value; };

// ast_node.h/cpp - Simple tree structure
class ASTNode {
    std::string label;
    std::vector<ASTNode*> children;
public:
    void addChild(ASTNode* child);
    void print(int depth, bool isRoot);
};

// lexer.h/cpp - Tokenization logic
class Lexer {
    std::map<std::string, TokenType> keywords;
public:
    Token nextToken();
};

// parser.h/cpp - Parsing logic
class Parser {
    Lexer lexer;
    Token currentToken;
public:
    ASTNode* parseProgram();
    ASTNode* parseExpression();
    // ... 30+ parsing methods
};
```

### Memory Management

- **RAII Principles**: Automatic resource management where possible
- **Clear Ownership**: AST nodes own their children
- **Cleanup Handling**: Proper deletion in main() and destructors
- **Exception Safety**: Basic exception handling for file operations

### Code Style

- **Modern C++11**: STL containers, range-based loops where appropriate
- **Consistent Naming**: Clear, descriptive function and variable names
- **Header Guards**: Proper include protection in all headers
- **Namespace Usage**: `std::` prefixes instead of `using namespace std`
- **Documentation**: Clear comments explaining complex parsing logic

## Performance Characteristics

### Compilation Performance

- **Incremental Builds**: Only changed modules recompile
- **Parallel Compilation**: Make can build object files in parallel
- **Header Dependencies**: Minimal includes reduce compilation overhead

### Runtime Performance

- **Single Pass**: One-pass lexing and parsing
- **Linear Complexity**: O(n) parsing time for most constructs
- **Memory Efficient**: AST nodes allocated only as needed
- **Fast I/O**: Efficient file reading and output generation

## Error Handling

### Syntax Error Detection

- **Token Mismatch**: Reports expected vs. actual tokens
- **Grammar Violations**: Detects violations of language grammar
- **File Access**: Handles missing or unreadable input files
- **Graceful Degradation**: Continues parsing when possible

### Error Examples

```bash
$ ./winzigc -ast nonexistent.winzig
Error: Cannot open file nonexistent.winzig

$ ./winzigc -ast malformed.winzig
Parse error (detailed error messages in development)
```

## Extending the Parser

### Adding New Language Features

1. **Extend TokenType** in `token.h`
2. **Update Lexer** in `lexer.cpp` for new tokens
3. **Add Parsing Methods** in `parser.h/cpp`
4. **Update Grammar** documentation

### Adding New AST Node Types

1. **Extend ASTNode** if needed for special node types
2. **Update Print Methods** for new node visualization
3. **Add Construction** in appropriate parser methods

## Educational Value

This parser serves as an excellent learning resource for:

- **Compiler Construction**: Complete lexer and parser implementation
- **Language Design**: Understanding of grammar and syntax rules
- **Software Architecture**: Modular design principles
- **C++ Programming**: Modern C++ practices and STL usage
- **Testing**: Comprehensive test-driven development approach

## Documentation References

- **`project docs/WinZigC_Grammar.pdf`**: Complete grammar specification
- **`project docs/WinZigC_Lex.pdf`**: Lexical analysis rules
- **`project docs/Project.pdf`**: Overall project requirements
- **Source Code**: Extensive inline documentation

## License

Educational project for compiler construction coursework.

## Future Enhancements

Potential improvements for advanced study:

- **Semantic Analysis**: Type checking and scope validation
- **Code Generation**: Backend for executable code production
- **Optimization**: AST optimization passes
- **IDE Integration**: Language server protocol support
- **Debugging**: Interactive parser debugging tools
