//
// Created by AKNoryx28 on 2023/01/01.
//

/* 
 * SIMPLE Unity Il2Cpp Api Wrapper to retrieve Methods and Fields
 * Fast Header Only 😉
 *
 * Depends on KittyMemory by MJx0
 * https://github.com/MJx110/KittyMemory (b852955)
 */

#pragma once

#include <android/log.h>
#include <inttypes.h>
#include <unordered_map>
#include <KittyMemory.hpp>
#include <KittyScanner.hpp>

#define IL2CPP_LOG_TAG "MiniIL2CPP"

#define IL2CPP_LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, IL2CPP_LOG_TAG, __VA_ARGS__))
#define IL2CPP_LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, IL2CPP_LOG_TAG, __VA_ARGS__))
#define IL2CPP_LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, IL2CPP_LOG_TAG, __VA_ARGS__))
#define IL2CPP_LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, IL2CPP_LOG_TAG, __VA_ARGS__))

struct Il2CppImage {
    const char* name;
    const char *nameNoExt;
    void* assembly;
    uint32_t typeCount;
    uint32_t exportedTypeCount;
    uint32_t customAttributeCount;
};
struct Il2CppClass {
    const Il2CppImage* image;
    void* gc_desc;
    const char* name;
    const char* namespaze;
};
struct Il2CppArray {
#ifdef __LP64__
    char pad[0x10];
#else
    char pad[0x8];
#endif
    void *bounds;
    uintptr_t max_length;
    void *vector[32];
};
struct MethodInfo {
    void* methodPointer;
    void* invoker_method;
    const char* name;
    Il2CppClass *klass;
    void* return_type;
    const void* parameters;
};
struct FieldInfo {
    const char* name;
    void *type;
    Il2CppClass *parent;
    int32_t offset;
    uint32_t token;
};

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
        KittyMemory::ProcMap il2cpp = KittyMemory::getElfBaseMap("libil2cpp.so");
        if (!il2cpp.isValid()) return;
        auto handle = KittyScanner::ElfScanner::createWithMap(il2cpp);
        #define RES_(a) a = decltype(a)(handle.findSymbol(#a))
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
            std::istringstream ss(namespaceClass);
            std::string token;
            std::getline(ss, token, '.');
            namespaze = token;
            std::getline(ss, token, '.');
            clazz = token;
        }
        Il2CppClass *fClass = nullptr;
        size_t size;
    	void **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &size);
        // Version greater than 2018.3
        if (il2cpp_image_get_class)
        {
            for (int i = 0; i < size; ++i) {
                Il2CppImage *image = il2cpp_assembly_get_image(assemblies[i]);
	        	if (!image) continue;
                size_t typeCount = image->typeCount;
                for (size_t i = 0; i < typeCount; ++i) {
                    auto cls = il2cpp_image_get_class(image, i);
                    if (!cls) continue;
                    if (strcmp("<Module>", cls->name) == 0) continue;
                    if (namespaze == cls->namespaze && clazz == cls->name) {
                        fClass = cls;
                        goto done;
                    }
                }
            }
        }
        // Version less than 2018.3
        else
        {
            auto corlib = il2cpp_get_corlib();
            auto assemblyClass = il2cpp_class_from_name(corlib, "System.Reflection", "Assembly");
            auto assemblyLoad = il2cpp_class_get_method_from_name(assemblyClass, "Load", 1);
            auto assemblyGetTypes = il2cpp_class_get_method_from_name(assemblyClass, "GetTypes", 0);
            if (!assemblyLoad || !assemblyLoad->methodPointer) {
                IL2CPP_LOGE("missing Assembly::Load");
                return;
            }
            if (!assemblyGetTypes || !assemblyGetTypes->methodPointer) {
                IL2CPP_LOGE("missing Assembly::GetTypes");
                return;
            }
            typedef void *(*Assembly_Load_ftn)(void *, void *, void *);
            typedef Il2CppArray *(*Assembly_GetTypes_ftn)(void *, void *);
            for (int i = 0; i < size; ++i) {
                Il2CppImage *image = il2cpp_assembly_get_image(assemblies[i]);
	            if (!image) continue;
                auto imageName = std::string(image->name);
                auto imageNameNoExt = imageName.substr(0, imageName.rfind('.'));
                auto assemblyFileName = il2cpp_string_new(imageNameNoExt.data());
                auto reflectionAssembly = ((Assembly_Load_ftn) assemblyLoad->methodPointer)(nullptr, assemblyFileName, nullptr);
                auto reflectionTypes = ((Assembly_GetTypes_ftn) assemblyGetTypes->methodPointer)(reflectionAssembly, nullptr);
                auto items = reflectionTypes->vector;
                for (int j = 0; j < reflectionTypes->max_length; ++j) {
                    auto cls = il2cpp_class_from_system_type((void*) items[j]);
                    if (!cls) continue;
                    if (strcmp("<Module>", cls->name) == 0) continue;
                    if (namespaze == cls->namespaze && clazz == cls->name) {
                        fClass = cls;
                        goto done;
                    }
                }
            }
        }
        done:
        if (!fClass) {
            IL2CPP_LOGE("IL2CPP Error: class %s not found", clazz.c_str());
            return;
        }
        this->Clazz = fClass;
        this->Methods_Cache = {};
        this->Fields_Cache = {};
    }
    
    void *GetMethodPtr(const char *name, int argc) {
        MiniIl2Cpp_Internal::Init();
        if (!MiniIl2Cpp_Internal::isinit)
            return nullptr;
        using namespace MiniIl2Cpp_Internal;
        if (!Clazz) {
           IL2CPP_LOGE("IL2CPP Error: class %s not found", Clazz->name);
           return nullptr;
        }
        
        std::string cachedName(std::to_string(argc));
        cachedName.append(name);
        if (Methods_Cache.count(cachedName) > 0)
            return Methods_Cache[cachedName];
        
        auto Method = il2cpp_class_get_method_from_name(Clazz, name, argc);
        if (!Method) {
            IL2CPP_LOGE("IL2CPP Error: method %s argc %i in class %s not found", name, argc, Clazz->name);
            return nullptr;
        }
        
        Methods_Cache[cachedName] = Method->methodPointer;
        return Method->methodPointer;
    }
    
    int GetFieldOffset(const char *name) {
        MiniIl2Cpp_Internal::Init();
        if (!MiniIl2Cpp_Internal::isinit)
            return 0;
        using namespace MiniIl2Cpp_Internal;
        if (!Clazz) {
           IL2CPP_LOGE("IL2CPP Error: class %s not found", Clazz->name);
           return 0;
        }
        
        std::string cachedName(name);
        if (Fields_Cache.count(cachedName) > 0)
            return Fields_Cache[cachedName];
        
        auto Field = il2cpp_class_get_field_from_name(Clazz, name);
        if (!Field) {
            IL2CPP_LOGE("IL2CPP Error: field %s in class %s not found", name, Clazz->name);
            return 0;
        }
        
        Fields_Cache[cachedName] = Field->offset;
        return Field->offset;
    }
    
protected:
    Il2CppClass *Clazz;
    std::unordered_map<std::string, void*> Methods_Cache;
    std::unordered_map<std::string, int> Fields_Cache;
    
};

/*
int main_example()
{
    // LoadClass("Namespace.Class")
    LoadClass clsPlayerController = LoadClass("PlayerScript.PlayerController");
    // GetMethodPtr("Method", parametersCount);
    void *updateMethod = clsPlayerController.GetMethodPtr("Update", 0);
    
    HOOK(updateMethod, hook_Update, orig_Update);
    
    return 0;
}
*/