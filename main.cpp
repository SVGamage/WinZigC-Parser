#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " -ast <filename>" << std::endl;
        return 1;
    }
    
    std::string flag = argv[1];
    std::string filename = argv[2];
    
    if (flag != "-ast") {
        std::cerr << "Only -ast flag is supported" << std::endl;
        return 1;
    }
    
    // Read input file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return 1;
    }
    
    std::string input;
    std::string line;
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
            std::cout << std::endl; // Add final newline to match expected output
        } else {
            std::cerr << "Parse error" << std::endl;
            return 1;
        }
        
        // Clean up memory (in practice, you'd want proper RAII)
        delete ast;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
