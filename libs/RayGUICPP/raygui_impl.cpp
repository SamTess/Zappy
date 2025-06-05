/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** RayGUI encapsulation
*/

// Définir les macros avant d'inclure raygui.h pour éviter les conflits
#define RAYLIB_VECTOR2_DEFINED
#define RAYLIB_VECTOR3_DEFINED
#define RAYLIB_COLOR_DEFINED
#define RAYLIB_RECTANGLE_DEFINED

// Définir RAYGUI_IMPLEMENTATION pour générer le code d'implémentation
#define RAYGUI_IMPLEMENTATION
#include "raygui-4.0/src/raygui.h"

// Inclure l'adaptateur après pour avoir les fonctions de conversion
#include "../TypeAdapter.hpp"
