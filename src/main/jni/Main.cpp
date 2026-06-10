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

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

// Game Package Name Here package 📦 
#define targetPackageName OBFUSCATE("com.Nobodyshot.kuboom") // Polyfield package name

// ESP Globals
ImVec4 espBoxColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
ImVec4 espLineColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
ImVec4 espHealthColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue

// Modern ImGui Style
void SetModernDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Base colors
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 0.94f); // Darker background
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border]                 = ImVec4(0.08f, 0.08f, 0.08f, 0.88f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    // Styling
    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(5, 5);
    style.CellPadding = ImVec2(6, 6);
    style.ItemSpacing = ImVec2(6, 6);
    style.ItemInnerSpacing = ImVec2(6, 6);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.GrabMinSize = 10.0f;
    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 1.0f;
    style.WindowRounding = 7.0f;
    style.ChildRounding = 4.0f;
    style.FrameRounding = 3.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding = 3.0f;
    style.LogSliderDeadzone = 4.0f;
    style.TabRounding = 4.0f;
}

// ESP Drawing Function (Placeholder - needs actual game data)
void DrawESP() {
    if (!sConfig.ESPMenu.Esp) return;

    // Example: Draw a simple box at screen center
    // In a real scenario, you would iterate through game entities, get their world-to-screen coordinates
    // and then draw boxes, lines, health bars based on their position and state.
    
    // For Polyfield (Unity/IL2CPP), you'd typically:
    // 1. Find the PlayerManager/EntityManager class using MiniIl2Cpp.
    // 2. Get a list of active players/entities.
    // 3. For each player, get their transform (position), health, etc.
    // 4. Convert world coordinates to screen coordinates using the game's camera matrix.
    // 5. Draw using ImGui::GetBackgroundDrawList() or ImGui::GetForegroundDrawList().

    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    float screenWidth = ImGui::GetIO().DisplaySize.x;
    float screenHeight = ImGui::GetIO().DisplaySize.y;

    if (sConfig.ESPMenu.ESPBox) {
        // Example Box (replace with actual player data)
        float boxWidth = 100.0f;
        float boxHeight = 150.0f;
        ImVec2 boxTopLeft = ImVec2(screenWidth / 2 - boxWidth / 2, screenHeight / 2 - boxHeight / 2);
        ImVec2 boxBottomRight = ImVec2(screenWidth / 2 + boxWidth / 2, screenHeight / 2 + boxHeight / 2);
        drawList->AddRect(boxTopLeft, boxBottomRight, ImGui::GetColorU32(espBoxColor), 0.0f, 0, 2.0f);
    }

    if (sConfig.ESPMenu.ESPLine) {
        // Example Line (replace with actual player data)
        ImVec2 screenBottom = ImVec2(screenWidth / 2, screenHeight);
        ImVec2 playerFoot = ImVec2(screenWidth / 2, screenHeight / 2 + 75); // Example player foot position
        drawList->AddLine(screenBottom, playerFoot, ImGui::GetColorU32(espLineColor), 2.0f);
    }

    if (sConfig.ESPMenu.EspObject) {
        // Example Health Bar (replace with actual player health)
        float health = 75.0f; // Example health 0-100
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
    const ImVec2 window_size = ImVec2(680, 720); // Slightly larger for modern feel
    ImGui::SetNextWindowSize(window_size, ImGuiCond_FirstUseEver);

    if (ImGui::Begin(OBFUSCATE("POLYFIELD ZYGISK MOD"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
        static int tab = 0; // 0: ESP, 1: Chams, 2: Settings

        // Tab Bar
        if (ImGui::BeginTabBar(OBFUSCATE("MainTabs"), ImGuiTabBarFlags_None)) {
            if (ImGui::BeginTabItem(OBFUSCATE("ESP"))) {
                tab = 0;
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(OBFUSCATE("CHAMS"))) {
                tab = 1;
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem(OBFUSCATE("SETTINGS"))) {
                tab = 2;
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Content based on selected tab
        if (tab == 0) { // ESP Tab
            ImGui::Text(OBFUSCATE("ESP Features"));
            ImGui::Checkbox(OBFUSCATE("Enable ESP"), &sConfig.ESPMenu.Esp);
            if (sConfig.ESPMenu.Esp) {
                ImGui::Checkbox(OBFUSCATE("Box ESP"), &sConfig.ESPMenu.ESPBox);
                ImGui::SameLine();
                ImGui::ColorEdit3(OBFUSCATE("Box Color"), (float*)&espBoxColor, ImGuiColorEditFlags_NoInputs);

                ImGui::Checkbox(OBFUSCATE("Line ESP"), &sConfig.ESPMenu.ESPLine);
                ImGui::SameLine();
                ImGui::ColorEdit3(OBFUSCATE("Line Color"), (float*)&espLineColor, ImGuiColorEditFlags_NoInputs);

                ImGui::Checkbox(OBFUSCATE("Health Bar"), &sConfig.ESPMenu.EspObject);
                ImGui::SameLine();
                ImGui::ColorEdit3(OBFUSCATE("Health Color"), (float*)&espHealthColor, ImGuiColorEditFlags_NoInputs);

                // Add more ESP options here (e.g., Distance, Name, etc.)
            }
        }

        if (tab == 1) { // Chams Tab (existing logic)
            ImGui::PushItemWidth(400);
            ImGui::Text("%s Chams Type", ICON_FA_PAINT_BRUSH);
            ImGui::Combo("##Chams Type", &chamsint, Chams, IM_ARRAYSIZE(Chams));

            if (chamsint > 1) {
                ImGui::Separator();
                ImGui::PushItemWidth(45);
                ImGui::Text("%s Visible Color", ICON_FA_EYE_DROPPER);
                ImGui::ColorEdit3("##VisibleColor", (float*)&visibleColor);
                ImGui::SameLine();
                ImGui::Checkbox("RGB##Visible", &enableRainbow);
            }

            if (chamsint == 6 || chamsint == 7) {
                ImGui::Separator();
                ImGui::Text("%s In Wall Color", ICON_FA_EYE_DROPPER);
                ImGui::ColorEdit3("##InWallColor", (float*)&inWallColor);
                ImGui::SameLine();
                ImGui::Checkbox("RGB##Wall", &enableRainbowWall);
            }
        }

        if (tab == 2) { // Settings Tab
            ImGui::Text(OBFUSCATE("General Settings"));
            ImGui::Separator();
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), OBFUSCATE("Important Notice:"));
            ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), OBFUSCATE("Ensure game resolution is set appropriately for best menu display."));
            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0.0f, 0.7f, 1.0f, 1.0f), OBFUSCATE("Join My Telegram Channel for More Free Content!"));
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), OBFUSCATE("Link: https://t.me/CheatCodeRevo"));
        }

        ImGui::End(); // End of Main Window
    }

    // RGB Effect Handling (existing logic)
    if (enableRainbow) {
        visibleColor.x = redd / 255.0f;
        visibleColor.y = greenn / 255.0f;
        visibleColor.z = bluee / 255.0f;
    }
    if (enableRainbowWall) {
        inWallColor.x = redd / 255.0f;
        inWallColor.y = greenn / 255.0f;
        inWallColor.z = bluee / 255.0f;
    }
    performRGBChange();

    // Draw ESP elements outside the main window to ensure they are always on top
    DrawESP();
}

void (*old_input)(void *event, void *exAb, void *exAc);
void hook_input(void *event, void *exAb, void *exAc) {
    old_input(event, exAb, exAc);
    ImGui_ImplAndroid_HandleTouchEvent((AInputEvent *) event, {(float) egl.screenWidth / (float) egl.width, (float) egl.screenHeight / (float) egl.height});
    return;
}

int (*old_getWidth)(ANativeWindow* window);
int hook_getWidth(ANativeWindow* window) {
    egl.screenWidth = old_getWidth(window);
    return old_getWidth(window);
}

int (*old_getHeight)(ANativeWindow* window);
int hook_getHeight(ANativeWindow* window) {
    egl.screenHeight = old_getHeight(window);
    return old_getHeight(window);
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &egl.width);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &egl.height);
    
    if (!egl.setup) {
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(egl.width, egl.height);
        ImGui_ImplOpenGL3_Init(OBFUSCATE("#version 300 es"));
        ImGui_ImplAndroid_Init(NULL);
        
        SetModernDarkTheme(); // Use new modern theme
			
        ImGuiStyle* style = &ImGui::GetStyle();
			
        style->WindowTitleAlign = ImVec2(0.5, 0.5);
      
        io.IniFilename = NULL;     
        static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
        ImFontConfig icons_config;  
        ImFontConfig CustomFont;
        CustomFont.FontDataOwnedByAtlas = false;

        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 2.5;
        icons_config.OversampleV = 2.5;

        io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 23.0f, &CustomFont);
        io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 25.0f, &icons_config, icons_ranges);
        
        ImFontConfig font_cfg;
        font_cfg.SizePixels = 19.0f;
        io.Fonts->AddFontDefault(&font_cfg);
        
        ImGui::GetStyle().ScaleAllSizes(3.0f);
        egl.setup = true;
    }

    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(egl.width, egl.height);
    ImGui::NewFrame();
    
    DrawMenu();
    
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    return old_eglSwapBuffers(dpy, surface);
}

void *hack_start(const char *) {
    LOGI(OBFUSCATE("pthread created"));
	ProcMap mineMap;
    do {
        mineMap = KittyMemory::getLibraryMap(MineLib);
			handle = NULL;
			handle = dlopen(OBFUSCATE("libGLESv2.so"), RTLD_LAZY);
    } while (!mineMap.isValid());
		
	RenderHook();
  
    do {
        sleep(1);
    } while (dlopen(OBFUSCATE("libandroid.so"), 4) == NULL);

    do {
        sleep(1);
    } while (dlopen(OBFUSCATE("libinput.so"), 4) == NULL);

    do {
        sleep(1);
    } while (dlopen(OBFUSCATE("libEGL.so"), 4) == NULL);
    
    DobbyHook((void *) dlsym(dlopen(OBFUSCATE("libandroid.so"), 4), OBFUSCATE("ANativeWindow_getWidth")), (void *) hook_getWidth, (void **) &old_getWidth);
    DobbyHook((void *) dlsym(dlopen(OBFUSCATE("libandroid.so"), 4), OBFUSCATE("ANativeWindow_getHeight")), (void *) hook_getHeight, (void **) &old_getHeight);
    DobbyHook((void *) dlsym(dlopen(OBFUSCATE("libinput.so"), 4), OBFUSCATE("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE")), (void *) hook_input, (void **) &old_input);
    DobbyHook((void *) dlsym(dlopen(OBFUSCATE("libEGL.so"), 4), OBFUSCATE("eglSwapBuffers")), (void *) hook_eglSwapBuffers, (void **) &old_eglSwapBuffers);

    while (!G_IL2CPP) {
    G_IL2CPP = Tools::GetBaseAddress("libil2cpp.so");
    sleep(1);
    }
    
    IL2Cpp::Il2CppAttach();  
    sleep(5);  
    pthread_exit(nullptr);
    return nullptr;

}


class ImGuiModMenu : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api = api;
        this->env = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        auto package_name = env->GetStringUTFChars(args->nice_name, nullptr);
        auto app_data_dir = env->GetStringUTFChars(args->app_data_dir, nullptr);
        preSpecialize(package_name, app_data_dir);
        env->ReleaseStringUTFChars(args->nice_name, package_name);
        env->ReleaseStringUTFChars(args->app_data_dir, app_data_dir);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (enable_hack) {
            std::thread hack_thread(hack_start, game_data_dir);
            hack_thread.detach();
        }
    }

	
private:
    Api *api;
    JNIEnv *env;
    bool enable_hack;
    char *game_data_dir;

    void preSpecialize(const char *package_name, const char *app_data_dir) {
        if (strcmp(package_name, targetPackageName) == 0) {
            enable_hack = true;
            game_data_dir = new char[strlen(app_data_dir) + 1];
            strcpy(game_data_dir, app_data_dir);
        }
    }
};

REGISTER_ZYGISK_MODULE(ImGuiModMenu)
