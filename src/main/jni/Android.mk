LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := ZYGOD/Dobby/libraries/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_STATIC_LIBRARIES := libdobby

LOCAL_MODULE    := $(TARGET_ARCH_ABI)

# Code optimization
# -std=c++17 is required to support AIDE app with NDK
LOCAL_CFLAGS := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fpermissive
LOCAL_CPPFLAGS := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all, -llog
LOCAL_ARM_MODE := arm

LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)


# Here you add the cpp file
LOCAL_SRC_FILES := Main.cpp \
    ZYGOD/ByNameModding/Tools.cpp \
    ZYGOD/ByNameModding/Il2Cpp.cpp \
    ZYGOD/Important/hde64.c \
	ZYGOD/Important/SubstrateDebug.cpp \
	ZYGOD/Important/SubstrateHook.cpp \
	ZYGOD/Important/SubstratePosixMemory.cpp \
	ZYGOD/Important/SymbolFinder.cpp \
	ZYGOD/KittyMemory/KittyMemory.cpp \
	ZYGOD/KittyMemory/MemoryPatch.cpp \
    ZYGOD/KittyMemory/MemoryBackup.cpp \
    ZYGOD/KittyMemory/KittyUtils.cpp \
	ZYGOD/And64InlineHook/And64InlineHook.cpp \
    ZYGOD/ImGui/imgui.cpp \
    ZYGOD/ImGui/imgui_draw.cpp \
    ZYGOD/ImGui/imgui_widgets.cpp \
    ZYGOD/ImGui/imgui_tables.cpp \
    ZYGOD/ImGui/imgui_demo.cpp \
    ZYGOD/ImGui/backends/imgui_impl_android.cpp \
    ZYGOD/ImGui/backends/imgui_impl_opengl3.cpp \

LOCAL_LDLIBS := -llog -landroid -lGLESv2 -lEGL -lGLESv3

include $(BUILD_SHARED_LIBRARY)
