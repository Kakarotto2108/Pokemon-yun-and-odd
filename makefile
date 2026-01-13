CXX = g++
# Pour un nouvel iMac M-series.
ARCH_FLAG = -arch arm64
CXXFLAGS = -Wall -std=c++17 $(ARCH_FLAG)

# >>> UTILISATION DU CHEMIN DIRECT VERS LA VERSION 3.0.2 DANS LE CELLAR <<<
# Ceci contourne tout problème de symlink ou de configuration zsh.
SFML_PATH = /opt/homebrew/Cellar/sfml/3.0.2
CPPFLAGS = -I$(SFML_PATH)/include
LDFLAGS = -L$(SFML_PATH)/lib

# Les fichiers objets
OBJ = main.o Player.o Pnj.o TileMap.o MessageBox.o Inventory.o Item.o Zone.o
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
TARGET = game

all: $(TARGET)

# Règle de liaison (linking) : utilise LDFLAGS
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Règle de compilation (main.o) : utilise CPPFLAGS
main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c main.cpp

# Les autres règles de compilation : utilisent CPPFLAGS
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
	
clean:
	rm -f $(OBJ) $(TARGET)