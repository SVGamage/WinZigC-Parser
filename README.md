# WinZigC Parser

A recursive descent parser for the WinZigC programming language implemented in C++. This parser builds an Abstract Syntax Tree (AST) from WinZigC source code and outputs a tree representation.

## Overview

WinZigC is a Pascal-like programming language that supports:

- Variable declarations and type definitions
- Functions with parameters and local scope
- Control structures (if/then/else, while, for, case, repeat/until, loop/pool)
- Built-in functions (succ, pred, chr, ord)
- Input/output operations
- Comments (both line `#` and block `{...}`)
- Basic data types (integer, boolean, char)

## Features

- **Lexical Analysis**: Complete tokenization of WinZigC source code
- **Syntax Analysis**: Recursive descent parser that builds an AST
- **Error Handling**: Basic error detection and reporting
- **AST Visualization**: Tree representation output for parsed programs
- **Memory Management**: Proper cleanup of dynamically allocated AST nodes

## Project Structure

```
WinZigC-Parser/
├── winzigc.cpp              # Main parser implementation
├── Makefile                 # Build configuration
├── run_tests.sh            # Test runner script
├── simple_test.winzig      # Simple test program
├── expected_output.tree    # Expected AST output
├── temp_output_*.tree      # Generated test outputs
├── project docs/           # Language specification documents
│   ├── Project.pdf
│   ├── WinZigC_Grammar.pdf
│   └── WinZigC_Lex.pdf
└── winzig_test_programs/   # Test programs and expected outputs
    ├── winzig_01           # Test program 1
    ├── winzig_01.tree      # Expected AST for test 1
    ├── ...
    ├── winzig_15           # Test program 15
    └── winzig_15.tree      # Expected AST for test 15
```

## Build Instructions

### Prerequisites

- C++ compiler with C++11 support (g++, clang++)
- Make utility

### Compilation

```bash
make
```

This will compile `winzigc.cpp` into an executable named `winzigc`.

### Clean Build

```bash
make clean
```

## Usage

The parser takes a WinZigC source file and outputs the corresponding AST:

```bash
./winzigc -ast <filename>
```

### Example Usage

```bash
# Parse a simple test program
./winzigc -ast simple_test.winzig

# Parse one of the test programs
./winzigc -ast winzig_test_programs/winzig_01
```

## Example Program

Here's a simple WinZigC program (`simple_test.winzig`):

```pascal
program simple:
var x: integer;
begin
    x := 5;
    output(x)
end simple.
```

This generates an AST showing the program structure with nodes for declarations, statements, and expressions.

## Language Grammar

The parser implements a complete recursive descent parser for the WinZigC grammar, which includes:

### Program Structure

```
Program -> 'program' Name ':' Consts Types Dclns SubProgs Body Name '.'
```

### Declarations

- **Constants**: `const name = value;`
- **Types**: `type name = (literal_list);`
- **Variables**: `var name_list : type;`

### Statements

- **Assignment**: `variable := expression`
- **Swap**: `variable :=: variable`
- **Output**: `output(expression_list)`
- **Read**: `read(variable_list)`
- **Control Structures**: if/then/else, while/do, for loops, case statements
- **Function Calls**: `function_name(argument_list)`

### Expressions

- Arithmetic operators: `+`, `-`, `*`, `/`, `mod`
- Comparison operators: `=`, `<>`, `<`, `<=`, `>`, `>=`
- Logical operators: `and`, `or`, `not`
- Built-in functions: `succ()`, `pred()`, `chr()`, `ord()`, `eof`

## Testing

### Run All Tests

```bash
./run_tests.sh
```

This script:

1. Compiles the parser if needed
2. Runs all test programs in `winzig_test_programs/`
3. Compares output with expected AST files
4. Reports pass/fail status for each test
5. Provides a summary of test results

### Test Programs

The project includes 15 comprehensive test programs that cover:

- Basic arithmetic and expressions
- Function definitions and calls
- Control flow statements
- Variable declarations and scoping
- Type definitions
- Complex nested structures

## Implementation Details

### Architecture

- **Lexer Class**: Tokenizes input source code
- **Parser Class**: Implements recursive descent parsing
- **ASTNode Class**: Represents nodes in the Abstract Syntax Tree

### Key Components

1. **Tokenization**: Converts source text into tokens (identifiers, keywords, operators, literals)
2. **Parsing**: Uses recursive descent to build AST following grammar rules
3. **AST Construction**: Creates tree structure representing program semantics
4. **Output Generation**: Prints formatted tree representation

### Token Types

The lexer recognizes:

- **Literals**: identifiers, integers, characters, strings
- **Keywords**: program, var, const, function, begin, end, if, while, etc.
- **Operators**: :=, :=:, +, -, \*, /, =, <>, <, <=, >, >=
- **Punctuation**: ;, :, ,, ., (, ), {, }

### Error Handling

- Syntax error detection during parsing
- Graceful handling of malformed input
- Error messages with context information

## Development

### Code Style

- Modern C++ with STL containers
- Clear separation of lexical and syntactic analysis
- Consistent naming conventions
- Comprehensive comments

### Memory Management

- Dynamic allocation for AST nodes
- Proper cleanup to prevent memory leaks
- Exception-safe resource handling

## License

This project is part of a compiler construction course assignment.

## Contributing

This is an educational project. For improvements or bug fixes:

1. Ensure all existing tests pass
2. Add test cases for new functionality
3. Follow the existing code style
4. Update documentation as needed

## References

- WinZigC Language Specification (see `project docs/`)
- WinZigC Grammar Definition
- WinZigC Lexical Analysis Specification
