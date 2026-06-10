#pragma once 
#include <list>
#include <vector>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <dlfcn.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"

#include "Zygisk/zygisk.h"
#include "ImGui/imgui.h"
#include "ImGui/Font.h"
#include "ImGui/Icon.h"
#include "ImGui/Iconcpp.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"

#include "ByNameModding/Tools.h"
#include "ByNameModding/Il2Cpp.h"

#include "KittyMemory/MemoryPatch.h"
#include "Includes/Macros.h"

#include "Dobby/dobby.h"
#include "InitHacks.h"

#define targetLib "libil2cpp.so"
#define targetLib2 "libunity.so"

uintptr_t G_IL2CPP;
std::map<std::string, uintptr_t> Fields;
std::map<std::string, uintptr_t> Methods;



