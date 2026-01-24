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
# Project files
# =========================
LIB_DIR = lib
ALL_LIB_DIRS = $(shell find $(LIB_DIR) -type d)
VPATH = . $(ALL_LIB_DIRS)
SRC = main.cpp $(shell find $(LIB_DIR) -name "*.cpp")
OBJ = $(addprefix $(BUILD_DIR)/, $(notdir $(SRC:.cpp=.o)))
BIN_DIR = bin
TARGET = $(BIN_DIR)/PokemonYunOdd
BUILD_DIR = build

ifeq ($(OS_NAME),Windows)
    TARGET := $(TARGET).exe
endif

# =========================
# Common flags
# =========================
CXXFLAGS = -Wall -std=c++17
CPPFLAGS = -I. $(addprefix -I, $(ALL_LIB_DIRS))
LDFLAGS =
LIBS = 

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

$(BUILD_DIR)/%.o: %.cpp ; @mkdir -p $(BUILD_DIR) ; $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

git:
	@echo "--- 🛠️  Préparation du commit ---"
	@read -p "Nom de la branche : " branch; \
	read -p "Message du commit : " msg; \
	if [ -z "$$branch" ] || [ -z "$$msg" ]; then \
		echo "❌ ERREUR : Champs vides, annulation."; \
		exit 1; \
	fi; \
	git checkout -b $$branch || git checkout $$branch && \
	git add . && \
	git commit -m "$$msg" && \
	git push origin $$branch && \
	git checkout dev && \
	git pull origin dev && \
	git merge $$branch --no-edit && \
	git push origin dev && \
	echo "--- 🗑️  Nettoyage distant et local ---" && \
	git push origin --delete $$branch && \
	git branch -d $$branch && \
	echo "✅ Terminé ! Tu es sur dev."