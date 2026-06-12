#ifndef IMGUIANDROID_INITHACKS_H
#define IMGUIANDROID_INITHACKS_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/input.h>
#include <string>
#include <vector>

#include "ImGui/imgui.h"

// Global EGL state
struct {
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    EGLConfig config;
    int width;
    int height;
    int screenWidth;
    int screenHeight;
    bool setup;
} egl;

// Configuration structure for the mod menu
struct sConfig {
    struct sESP {
        bool Esp = false;
        bool ESPLine = false;
        bool ESPBox = false;
        bool EspObject = false; // Used for Health Bar
        
        float BoxSize = 0;
        float density = -1;
        
        float LineSize = 1.0f;
        float BoxSizeValue = 1.0f;
    } ESPMenu;

    struct sFeat {
        bool GodMode = false;
        bool UnlimitedAll = false;
    } FEATMenu;

    struct sChams {
        bool Chams = false;
        bool Shading = false;
        bool Wireframe = false;
        bool Glow = false;
        bool Outline = false;
        bool Rainbow = false;
    } CHAMSMenu;
};

// Global instance of config
extern sConfig settings;

// Existing Chams variables
extern const char* Chams[];
extern int chamsint;
extern ImVec4 visibleColor;
extern ImVec4 inWallColor;
extern bool enableRainbow;
extern bool enableRainbowWall;
extern std::vector<std::string> shaders;
extern std::string mineShader;

// ESP Color variables (defined in Main.cpp)
extern ImVec4 espBoxColor;
extern ImVec4 espLineColor;
extern ImVec4 espHealthColor;

#endif //IMGUIANDROID_INITHACKS_H
