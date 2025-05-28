# Makefile pour la bibliothèque RaylibCPP

CXX = g++
CFLAGS = -fPIC -O2 -Wall -std=c++17
LDFLAGS = -shared

# Chemins
SRC_DIR = .
BUILD_DIR = ../../build/libs
TARGET = libraylibcpp.so

# Sources
SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(SRC_DIR)/window/*.cpp) \
       $(wildcard $(SRC_DIR)/shape/*.cpp) \
       $(wildcard $(SRC_DIR)/texture/*.cpp) \
       $(wildcard $(SRC_DIR)/font/*.cpp) \
       $(wildcard $(SRC_DIR)/input/*.cpp) \
       $(wildcard $(SRC_DIR)/audio/*.cpp) \
       $(wildcard $(SRC_DIR)/utils/*.cpp)

# Objets
OBJS = $(SRCS:.cpp=.o)

# Includes
INCLUDES = -I../../src/Shared

# Règle par défaut
all: $(TARGET)

# Création des objets
%.o: %.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Création de la bibliothèque partagée
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
