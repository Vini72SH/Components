# Variáveis
CXX = g++
CXXFLAGS = -Wall -Wextra -Wall -std=c++17 -g
TARGET = test
SRC = test.cpp buffer.cpp
OBJ = $(SRC:.cpp=.o)

# Regras
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
