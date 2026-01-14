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
CPPFLAGS =
LDFLAGS =
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# =========================
# macOS (Apple Silicon)
# =========================
ifeq ($(UNAME_S),Darwin)
    CXXFLAGS += -arch arm64
    SFML_PATH = /opt/homebrew/Cellar/sfml/3.0.2
    CPPFLAGS += -I$(SFML_PATH)/include
    LDFLAGS  += -L$(SFML_PATH)/lib
endif

# =========================
# Linux (Codespaces)
# =========================
ifeq ($(UNAME_S),Linux)
    # SFML installed via apt (standard paths)
endif

# =========================
# Project files
# =========================
OBJ = main.o Player.o Pnj.o TileMap.o MessageBox.o Inventory.o Item.o Zone.o
TARGET = game

# =========================
# Rules
# =========================
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS) $(LIBS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c main.cpp

Player.o: lib/Player.cpp lib/Player.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c lib/Player.cpp -o Player.o

Pnj.o: lib/Pnj.cpp lib/Pnj.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c lib/Pnj.cpp -o Pnj.o

TileMap.o: lib/TileMap.cpp lib/TileMap.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c lib/TileMap.cpp -o TileMap.o

MessageBox.o: lib/MessageBox.cpp lib/MessageBox.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c lib/MessageBox.cpp -o MessageBox.o

Inventory.o: lib/Inventory.cpp lib/Inventory.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c lib/Inventory.cpp -o Inventory.o

Item.o: lib/Item.cpp lib/Item.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c lib/Item.cpp -o Item.o

Zone.o: lib/Zone.cpp lib/Zone.hpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c lib/Zone.cpp -o Zone.o

# =========================
# Clean
# =========================
clean:
	rm -f $(OBJ) $(TARGET)
