# WinZigC Parser

# EXECUTION INSTRUCTIONS

1. BUILDING THE PROJECT

---

```make```

2. RUNNING THE PARSER

```

./winzigc -ast winzig_test_programs/winzig_01 > tree.01
./winzigc -ast winzig_test_programs/winzig_02 > tree.02
./winzigc -ast winzig_test_programs/winzig_03 > tree.03
...and so on for all test cases

```

3. VERIFYING OUTPUT

```

diff tree.01 winzig_test_programs/winzig_01.tree
diff tree.02 winzig_test_programs/winzig_02.tree
diff tree.03 winzig_test_programs/winzig_03.tree
...and so on for all test cases

```

4. RUN ALL TEST CASES

```

make test

```

5. CLEAN THE BUILD

```

make clean

```
# PROJECT STRUCTURE

```
├── app/                    # Source files
│   ├── main.cpp           # Main entry point
│   ├── lexer.cpp          # Lexical analyzer
│   ├── parser.cpp         # Parser implementation
│   └── ast_node.cpp       # AST node implementation
├── header/                # Header files
│   ├── lexer.h            # Lexer interface
│   ├── parser.h           # Parser interface
│   ├── ast_node.h         # AST node definition
│   └── token.h            # Token definitions
├── build/                 # Compiled object files
├── winzig_test_programs/  # Test cases and expected outputs
├── Makefile              # Build configuration
└── README.md             # This file
```

# ADDITIONAL MAKE TARGETS

- `make help` - Show available make targets
- `make structure` - Display project file structure
- `make clean-tests` - Remove test output files only
