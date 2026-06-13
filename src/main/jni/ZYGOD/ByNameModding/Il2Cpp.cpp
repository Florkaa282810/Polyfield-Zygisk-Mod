#include "ZYGOD/Includes/obfuscate.h"
#include "Il2cpp.h"
#include <map>
#include <string>
#include <dlfcn.h>
#include <unistd.h>
#include <android/log.h>

#define targetLib OBFUSCATE("libil2cpp.so")
#define g_LogTag OBFUSCATE("IL2CPP-LOGGER")

typedef unsigned short UTF16;
typedef wchar_t UTF32;
typedef char UTF8;

namespace {
    Il2CppString *(*il2cpp_string_new)(const char *str);
    Il2CppString *(*il2cpp_string_new_utf16)(const wchar_t *str, int32_t length);
    void **(*il2cpp_domain_get_assemblies)(const void *domain, size_t *size);
    void *(*il2cpp_domain_get)();
    const void *(*il2cpp_assembly_get_image)(const void *assembly);
    const char *(*il2cpp_image_get_name)(void *image);
    void *(*il2cpp_class_from_name)(const void *image, const char *namespaze, const char *name);
    void *(*il2cpp_class_get_field_from_name)(void *klass, const char *name);
    void (*il2cpp_field_static_get_value)(void *field, void *value);
    void (*il2cpp_field_static_set_value)(void *field, void *value);
    void *(*il2cpp_class_get_method_from_name)(void *klass, const char *name, int argsCount);
    size_t (*il2cpp_field_get_offset)(void *field);
}

void *IL2Cpp::Il2CppGetImageByName(const char *image) {
    size_t size;
    void **assemblies = il2cpp_domain_get_assemblies(il2cpp_domain_get(), &size);
    for(int i = 0; i < size; ++i) {
        void *img = (void *)il2cpp_assembly_get_image(assemblies[i]);
        const char *img_name = il2cpp_image_get_name(img);
        if (strcmp(img_name, image) == 0) {
            return img;
        }
    }
    return nullptr;
}

void *IL2Cpp::Il2CppGetClassType(const char *image, const char *namespaze, const char *clazz) {
    static std::map<std::string, void *> cache;
    std::string s = std::string(image) + namespaze + clazz;
    if (cache.count(s) > 0) return cache[s];
    void *img = IL2Cpp::Il2CppGetImageByName(image);
    if (!img) return nullptr;
    void *klass = il2cpp_class_from_name(img, namespaze, clazz);
    if (klass) cache[s] = klass;
    return klass;
}

void IL2Cpp::Il2CppGetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void *output) {
    void *klass = IL2Cpp::Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return;
    void *field = il2cpp_class_get_field_from_name(klass, name);
    if (!field) return;
    il2cpp_field_static_get_value(field, output);
}

void IL2Cpp::Il2CppSetStaticFieldValue(const char *image, const char *namespaze, const char *clazz, const char *name, void* value) {
    void *klass = IL2Cpp::Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return;
    void *field = il2cpp_class_get_field_from_name(klass, name);
    if (!field) return;
    il2cpp_field_static_set_value(field, value);
}

void *IL2Cpp::Il2CppGetMethodOffset(const char *image, const char *namespaze, const char *clazz, const char *name, int argsCount) {
    void *klass = IL2Cpp::Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return nullptr;
    void **method = (void**)il2cpp_class_get_method_from_name(klass, name, argsCount);
    return method ? *method : nullptr;
}

size_t IL2Cpp::Il2CppGetFieldOffset(const char *image, const char *namespaze, const char *clazz, const char *name) {
    void *klass = IL2Cpp::Il2CppGetClassType(image, namespaze, clazz);
    if (!klass) return -1;
    void *field = il2cpp_class_get_field_from_name(klass, name);
    return field ? il2cpp_field_get_offset(field) : -1;
}

// Simple UTF-16 to UTF-8 conversion without std::codecvt (deprecated and problematic in Android NDK)
const char* utf16_to_utf8(const uint16_t* source, size_t len) {
    static char buffer[1024];
    size_t out_idx = 0;
    for (size_t i = 0; i < len && out_idx < 1023; i++) {
        uint16_t c = source[i];
        if (c < 0x80) buffer[out_idx++] = (char)c;
        else if (c < 0x800) {
            buffer[out_idx++] = (char)(0xC0 | (c >> 6));
            buffer[out_idx++] = (char)(0x80 | (c & 0x3F));
        } else {
            buffer[out_idx++] = (char)(0xE0 | (c >> 12));
            buffer[out_idx++] = (char)(0x80 | ((c >> 6) & 0x3F));
            buffer[out_idx++] = (char)(0x80 | (c & 0x3F));
        }
    }
    buffer[out_idx] = '\0';
    return buffer;
}

const char* Il2CppString::CString() {
    return utf16_to_utf8(&this->start_char, this->length);
}

const wchar_t* Il2CppString::WCString() {
    return (const wchar_t*)&this->start_char; // Simplified for NDK compatibility
}

Il2CppString *Il2CppString::Create(const char *s) {
    return il2cpp_string_new(s);
}

Il2CppString *Il2CppString::Create(const wchar_t *s, int len) {
    return il2cpp_string_new_utf16(s, len);
}

void IL2Cpp::Il2CppAttach() {
    void *IL2Cpp_Handle = nullptr;
    while (!IL2Cpp_Handle) {
        IL2Cpp_Handle = dlopen(targetLib, RTLD_LAZY);
        if (!IL2Cpp_Handle) sleep(1);
    }
    il2cpp_domain_get_assemblies = (void **(*)(const void *, size_t *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_domain_get_assemblies"));
    il2cpp_string_new = (Il2CppString *(*)(const char *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_string_new"));
    il2cpp_string_new_utf16 = (Il2CppString *(*)(const wchar_t *, int32_t)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_string_new_utf16"));
    il2cpp_domain_get = (void *(*)()) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_domain_get"));
    il2cpp_assembly_get_image = (const void *(*)(const void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_assembly_get_image"));
    il2cpp_image_get_name = (const char *(*)(void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_image_get_name"));
    il2cpp_class_from_name = (void* (*)(const void*, const char*, const char *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_class_from_name"));
    il2cpp_class_get_field_from_name = (void* (*)(void*, const char *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_class_get_field_from_name"));
    il2cpp_field_static_get_value = (void (*)(void*, void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_field_static_get_value"));
    il2cpp_field_static_set_value = (void (*)(void*, void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_field_static_set_value"));
    il2cpp_class_get_method_from_name = (void* (*)(void *, const char*, int)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_class_get_method_from_name"));
    il2cpp_field_get_offset = (size_t (*)(void *)) dlsym(IL2Cpp_Handle, OBFUSCATE("il2cpp_field_get_offset"));
}
