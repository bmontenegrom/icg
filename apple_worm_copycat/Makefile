CXX = g++
CXXFLAGS = -std=c++11 -Wall -I./include
LDFLAGS = -lSDL2 -lSDL2_ttf

# Detect operating system
ifeq ($(OS),Windows_NT)
    CXXFLAGS += -I"C:\SDL2\include" -I"C:\SDL2\include\SDL2" -I"C:\freeimage\include"
    LDFLAGS += -L"C:\SDL2\lib" -L"C:\freeimage\lib" -lSDL2main -lopengl32 -lglu32 -lfreeimage
    TARGET_EXTENSION = .exe
else
    CXXFLAGS += -I/opt/homebrew/include/SDL2 -I/opt/homebrew/include -I/opt/homebrew/include/GL
    LDFLAGS += -L/opt/homebrew/lib -framework OpenGL -framework GLUT -lfreeimage
    TARGET_EXTENSION =
endif

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/apple_worm$(TARGET_EXTENSION)

.PHONY: all clean directories

all: directories $(TARGET)

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) 
