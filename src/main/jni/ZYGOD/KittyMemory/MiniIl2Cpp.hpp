#pragma once

#include <android/log.h>
#include <inttypes.h>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include "KittyMemory.h"
#include "KittyUtils.h"

#define IL2CPP_LOG_TAG "MiniIL2CPP"

#define IL2CPP_LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, IL2CPP_LOG_TAG, __VA_ARGS__))
#define IL2CPP_LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, IL2CPP_LOG_TAG, __VA_ARGS__))
#define IL2CPP_LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG, __VA_ARGS__))
#define IL2CPP_LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, IL2CPP_LOG_TAG, __VA_ARGS__))

// Use structures from ByNameModding/Il2Cpp.h if possible, otherwise define them here if missing
#include "../ByNameModding/Il2Cpp.h"

namespace MiniIl2Cpp_Internal
{
    static bool isinit;
    static void *(*il2cpp_domain_get)();
    static void **(*il2cpp_domain_get_assemblies)(const void *domain, size_t *size);
    static Il2CppImage *(*il2cpp_assembly_get_image)(const void *assembly);
    static Il2CppClass *(*il2cpp_image_get_class)(Il2CppImage *image, size_t index);
    static FieldInfo *(*il2cpp_class_get_field_from_name)(Il2CppClass *klass, const char *name);
    static MethodInfo *(*il2cpp_class_get_method_from_name)(Il2CppClass *klass, const char *name, int argsCount);
    static Il2CppImage* (*il2cpp_get_corlib)();
    static Il2CppClass* (*il2cpp_class_from_name)(const Il2CppImage * image, const char* namespaze, const char *name);
    static void* (*il2cpp_string_new)(const char* str);
    static Il2CppClass* (*il2cpp_class_from_system_type)(void *type);
    
    void Init() {
        if (isinit) return;
        KittyMemory::ProcMap il2cpp = KittyMemory::getLibraryMap("libil2cpp.so");
        if (!il2cpp.isValid()) return;
        
        auto find_sym = [&](const char* name) -> void* {
            return (void*)((uintptr_t)il2cpp.startAddr + KittyMemory::getAbsoluteAddress("libil2cpp.so", 0) - (uintptr_t)il2cpp.startAddr); // Placeholder for actual symbol finding if needed
        };

        // For Zygisk/Android, we usually use dlsym or KittyScanner if available. 
        // Since KittyScanner is missing, let's use a simpler approach or assume symbols are exported.
        #include <dlfcn.h>
        void* handle = dlopen("libil2cpp.so", RTLD_LAZY);
        if(!handle) return;

        #define RES_(a) a = (decltype(a))dlsym(handle, #a)
        RES_(il2cpp_domain_get);
        RES_(il2cpp_domain_get_assemblies);
        RES_(il2cpp_assembly_get_image);
        RES_(il2cpp_image_get_class);
        RES_(il2cpp_class_get_field_from_name);
        RES_(il2cpp_class_get_method_from_name);
        RES_(il2cpp_get_corlib);
        RES_(il2cpp_class_from_name);
        RES_(il2cpp_string_new);
        RES_(il2cpp_class_from_system_type);
        isinit = true;
    }
}

class LoadClass {
public:
    LoadClass(const std::string &namespaceClass) {
        MiniIl2Cpp_Internal::Init();
        if (!MiniIl2Cpp_Internal::isinit)
            return;
        using namespace MiniIl2Cpp_Internal;
        std::string namespaze;
        std::string clazz;
        { // parse namespaceClass
            size_t dot = namespaceClass.find_last_of('.');
            if (dot != std::string::npos) {
                namespaze = namespaceClass.substr(0, dot);
                clazz = namespaceClass.substr(dot + 1);
            } else {
                clazz = namespaceClass;
            }
        }
        Il2CppClass *fClass = nullptr;
        size_t size;
    	void **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &size);
        
        for (int i = 0; i < size; ++i) {
            Il2CppImage *image = il2cpp_assembly_get_image(assemblies[i]);
            if (!image) continue;
            // image->typeCount might not be directly accessible depending on Il2Cpp version
            // For simplicity, we use il2cpp_class_from_name which is more reliable
            fClass = il2cpp_class_from_name(image, namespaze.c_str(), clazz.c_str());
            if (fClass) break;
        }
        
        if (!fClass) {
            IL2CPP_LOGE("IL2CPP Error: class %s not found", clazz.c_str());
            return;
        }
        this->Clazz = fClass;
    }
    
    void *GetMethodPtr(const char *name, int argc) {
        if (!Clazz) return nullptr;
        auto Method = MiniIl2Cpp_Internal::il2cpp_class_get_method_from_name(Clazz, name, argc);
        if (!Method) return nullptr;
        return Method->methodPointer;
    }
    
    int GetFieldOffset(const char *name) {
        if (!Clazz) return 0;
        auto Field = MiniIl2Cpp_Internal::il2cpp_class_get_field_from_name(Clazz, name);
        if (!Field) return 0;
        return Field->offset;
    }
    
protected:
    Il2CppClass *Clazz = nullptr;
};
