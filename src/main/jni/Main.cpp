#include "ZYGOD/CallGUI_.h"
#include "style.h"
#include "Unity/Vector2.h"
#include "Unity/Vector3.h"
#include "Unity/Rect.h"
#include "Unity/Unity.h"
#include "Unity/Quaternion.h"

const char* MineLib = OBFUSCATE("libGLESv2.so");

#include "variables.h"
#include "RGB.h"
#include "Chams.h"
// #include "ZYGOD/KittyMemory/MiniIl2Cpp.hpp"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

// Game Package Name
#define targetPackageName OBFUSCATE("com.MA.Polyfield")

// ESP Globals
ImVec4 espBoxColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
ImVec4 espLineColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
ImVec4 espHealthColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);

// Global Settings Instance
sConfig settings;

// Modern ImGui Style
void SetModernDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 0.94f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);

    style.WindowRounding = 7.0f;
    style.FrameRounding = 3.0f;
}

void DrawESP() {
    if (!settings.ESPMenu.Esp) return;

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    float screenWidth = ImGui::GetIO().DisplaySize.x;
    float screenHeight = ImGui::GetIO().DisplaySize.y;

    if (settings.ESPMenu.ESPBox) {
        float boxWidth = 100.0f;
        float boxHeight = 150.0f;
        ImVec2 boxTopLeft = ImVec2(screenWidth / 2 - boxWidth / 2, screenHeight / 2 - boxHeight / 2);
        ImVec2 boxBottomRight = ImVec2(screenWidth / 2 + boxWidth / 2, screenHeight / 2 + boxHeight / 2);
        drawList->AddRect(boxTopLeft, boxBottomRight, ImGui::GetColorU32(espBoxColor), 0.0f, 0, 2.0f);
    }

    if (settings.ESPMenu.ESPLine) {
        ImVec2 screenBottom = ImVec2(screenWidth / 2, screenHeight);
        ImVec2 playerFoot = ImVec2(screenWidth / 2, screenHeight / 2 + 75);
        drawList->AddLine(screenBottom, playerFoot, ImGui::GetColorU32(espLineColor), 2.0f);
    }

    if (settings.ESPMenu.EspObject) {
        float health = 75.0f;
        float barHeight = 100.0f;
        float barWidth = 10.0f;
        ImVec2 barTopLeft = ImVec2(screenWidth / 2 + 60, screenHeight / 2 - barHeight / 2);
        ImVec2 barBottomRight = ImVec2(screenWidth / 2 + 70, screenHeight / 2 + barHeight / 2);
        ImVec2 currentHealthBottom = ImVec2(barTopLeft.x, barBottomRight.y - (barHeight * (health / 100.0f)));
        drawList->AddRectFilled(currentHealthBottom, barBottomRight, ImGui::GetColorU32(espHealthColor));
        drawList->AddRect(barTopLeft, barBottomRight, ImGui::GetColorU32(ImVec4(0.5f, 0.5f, 0.5f, 1.0f)), 0.0f, 0, 1.0f);
    }
}

void DrawMenu() {
    const ImVec2 window_size = ImVec2(680, 720);
    ImGui::SetNextWindowSize(window_size, ImGuiCond_FirstUseEver);

    if (ImGui::Begin(OBFUSCATE("POLYFIELD ZYGISK MOD | BY FELIPE MODS"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
        static int tab = 0;
        if (ImGui::BeginTabBar(OBFUSCATE("MainTabs"), ImGuiTabBarFlags_None)) {
            if (ImGui::BeginTabItem(OBFUSCATE("ESP"))) { tab = 0; ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem(OBFUSCATE("CHAMS"))) { tab = 1; ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem(OBFUSCATE("SETTINGS"))) { tab = 2; ImGui::EndTabItem(); }
            ImGui::EndTabBar();
        }

        if (tab == 0) {
            ImGui::Text(OBFUSCATE("ESP Features"));
            ImGui::Checkbox(OBFUSCATE("Enable ESP"), &settings.ESPMenu.Esp);
            if (settings.ESPMenu.Esp) {
                ImGui::Checkbox(OBFUSCATE("Box ESP"), &settings.ESPMenu.ESPBox);
                ImGui::SameLine(); ImGui::ColorEdit3(OBFUSCATE("Box Color"), (float*)&espBoxColor, ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox(OBFUSCATE("Line ESP"), &settings.ESPMenu.ESPLine);
                ImGui::SameLine(); ImGui::ColorEdit3(OBFUSCATE("Line Color"), (float*)&espLineColor, ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox(OBFUSCATE("Health Bar"), &settings.ESPMenu.EspObject);
                ImGui::SameLine(); ImGui::ColorEdit3(OBFUSCATE("Health Color"), (float*)&espHealthColor, ImGuiColorEditFlags_NoInputs);
            }
            ImGui::Separator();
            ImGui::Text(OBFUSCATE("Aimbot Features"));
            static bool aimbotEnabled = false;
            ImGui::Checkbox(OBFUSCATE("Enable Aimbot (Auto-Update)"), &aimbotEnabled);
            if (aimbotEnabled) {
                static float aimFov = 90.0f;
                ImGui::SliderFloat(OBFUSCATE("Aimbot FOV"), &aimFov, 10.0f, 360.0f);
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), OBFUSCATE("Aimbot uses Unity API dynamically."));
            }
        }

        if (tab == 1) {
            ImGui::Text(OBFUSCATE("Chams Settings"));
            ImGui::Combo("##ChamsType", &chamsint, Chams, IM_ARRAYSIZE(Chams));
        }

        if (tab == 2) {
            ImGui::Text(OBFUSCATE("General Settings"));
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), OBFUSCATE("Menu Author: FELIPE MODS"));
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), OBFUSCATE("Join Telegram: https://t.me/CheatCodeRevo"));
        }
        ImGui::End();
    }
    DrawESP();
}

// Zygisk Integration
class PolyfieldMod : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        const char *process = env->GetStringUTFChars(args->nice_name, nullptr);
        if (process && strcmp(process, targetPackageName) == 0) {
            enable_hack = true;
        }
        env->ReleaseStringUTFChars(args->nice_name, process);
    }

    void postAppSpecialize(const AppSpecializeArgs *args) override {
        if (enable_hack) {
            // Hooking logic here
        }
    }

private:
    Api *api;
    JNIEnv *env;
    bool enable_hack = false;
};

void companion_handler(int client) {
    // Handle companion requests here if needed
}

REGISTER_ZYGISK_MODULE(PolyfieldMod)
REGISTER_ZYGISK_COMPANION(companion_handler)
