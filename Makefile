CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = winzigc
SOURCES = winzigc.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean