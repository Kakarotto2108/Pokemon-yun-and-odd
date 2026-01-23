# =========================
# Compiler
# =========================
CXX = g++

# =========================
# OS detection
# =========================
UNAME_S := $(shell uname -s)

# =========================
# Common flags
# =========================
CXXFLAGS = -Wall -std=c++17
CPPFLAGS = -Ilib
LDFLAGS =
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# =========================
# macOS SFML 2
# =========================
ifeq ($(UNAME_S),Darwin)
    CXXFLAGS += -arch arm64
    SFML_PATH = /opt/homebrew/Cellar/sfml@2/2.6.2_1
    CPPFLAGS += -I$(SFML_PATH)/include
    LDFLAGS  += -L$(SFML_PATH)/lib
endif

# =========================
# Project files
# =========================
SRC = main.cpp $(wildcard lib/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = game

# =========================
# Rules
# =========================
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

rebuild: clean all

clean:
	rm -f $(OBJ) $(TARGET)