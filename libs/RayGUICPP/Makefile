# Compilation des librairies partagées Raylib et Raygui

# Répertoire courant = libs/RayGUICPP

CXX = g++
CC = gcc
CFLAGS = -fPIC -O2 -Wall
LDFLAGS = -shared

# Raylib (suppose que raylib est installé sur le système)
# Pour une compilation custom, adapter le chemin des sources raylib

# Raygui
RAYGUI_SRC = raygui-4.0/src/raygui.h
RAYGUI_SO = libraygui.so
RAYGUI_IMPL = raygui_impl.cpp
RAYGUI_CPP = RayGui.cpp
RAYGUI_ENCAP = include/RayGuiEncap.hpp

all: $(RAYGUI_SO)

$(RAYGUI_SO): $(RAYGUI_IMPL) $(RAYGUI_CPP) $(RAYGUI_SRC) $(RAYGUI_ENCAP)
	$(CXX) $(CFLAGS) -DRAYGUI_IMPLEMENTATION -Iraygui-4.0/src -c $(RAYGUI_IMPL) -o raygui_impl.o
	$(CXX) $(CFLAGS) -I../../src/Shared -c $(RAYGUI_CPP) -o RayGui.o
	$(CXX) $(LDFLAGS) -o $@ raygui_impl.o RayGui.o -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm -f *.o *.so

.PHONY: all clean
