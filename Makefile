CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = winzigc
SOURCES = main.cpp ast_node.cpp lexer.cpp parser.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: clean