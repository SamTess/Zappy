/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Parser for handling special markers and text processing
*/

#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <memory>
#include "Text3DCore.hpp"
#include "Text3DHelper.hpp"

namespace raylibcpp {

class Text3DParser {
public:
    static bool isWaveMarker(const std::string& text, int pos);
    static int skipWaveMarkers(const std::string& text, int pos);
    static void renderWaveText(const ::Font& font, const std::string& text,
        Vector3 position, float fontSize, float fontSpacing,
        float lineSpacing, bool backface,
        const WaveTextConfigPtr& config, float time, Color tint);

private:
    static void processWaveCharacter(const ::Font& font, int codepoint, int index,
        const RenderStatePtr& renderState,
        float fontSize, float fontSpacing, float lineSpacing,
        float scale, bool backface, Color tint,
        bool waveActive, const WaveTextConfigPtr& config,
        float time);
    static void handleWaveNewline(const RenderStatePtr& renderState,
        float fontSize, float lineSpacing);
};

}  // namespace raylibcpp
