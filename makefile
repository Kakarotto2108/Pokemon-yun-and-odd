# =========================
# Compiler
# =========================
CXX = g++

# =========================
# OS detection
# =========================
UNAME_S := $(shell uname -s)
ifeq ($(OS),Windows_NT)
    OS_NAME := Windows
else
    OS_NAME := $(UNAME_S)
endif

# =========================
# Common flags
# =========================
CXXFLAGS = -Wall -std=c++17
CPPFLAGS = -Ilib
LDFLAGS =
LIBS = 

# =========================
# Project files
# =========================
SRC = main.cpp $(wildcard lib/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = game
ifeq ($(OS_NAME),Windows)
    TARGET := game.exe
endif

# =========================
# OS-specific flags
# =========================
ifeq ($(OS_NAME),Darwin)
    CXXFLAGS += -arch arm64
    SFML_PATH = /opt/homebrew/Cellar/sfml@2/2.6.2_1
    CPPFLAGS += -I$(SFML_PATH)/include
    LDFLAGS  += -L$(SFML_PATH)/lib
    LIBS = -lsfml-graphics -lsfml-window -lsfml-system
endif

ifeq ($(OS_NAME),Linux)
    # Utilise pkg-config pour SFML
    CPPFLAGS += $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system)
    LDFLAGS  += $(shell pkg-config --libs sfml-graphics sfml-window sfml-system)
endif

ifeq ($(OS_NAME),Windows)
    CXXFLAGS += -IC:/SFML-2.5.1/include -Ilib -DSFML_DYNAMIC
    LDFLAGS  += -LC:/SFML-2.5.1/lib
    LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
           -lwinmm -lgdi32 -lfreetype -lopengl32 -lflac -lvorbisenc \
           -lvorbisfile -lvorbis -logg -lws2_32
endif

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
