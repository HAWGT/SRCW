#pragma once

// Runtime UE4 Reflection API — resolves everything by name, no game-specific headers

#include <Windows.h>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <cstring>
#include <iostream>

#include "CppSDK/SDK/CoreUObject_classes.hpp"

namespace Reflect
{
    SDK::UClass* FindClass(const char* className);
    SDK::UObject* FindCDO(const char* className);
    SDK::UObject* FindInstance(const char* className);
    SDK::UFunction* FindFunction(const char* className, const char* funcName);
    SDK::FProperty* FindProperty(SDK::UStruct* structType, const char* propName);
    int32_t FindPropertyOffset(SDK::UStruct* structType, const char* propName);

    template<typename T>
    T* GetPropertyPtr(SDK::UObject* obj, const char* propName)
    {
        int32_t offset = FindPropertyOffset(obj->Class, propName);
        if (offset < 0) return nullptr;
        return reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(obj) + offset);
    }

    bool CallStatic(const char* className, const char* funcName);
    bool CallStaticBool(const char* className, const char* funcName, bool value);
    bool CallStaticInt32(const char* className, const char* funcName, int32_t value);
    bool CallStaticUInt8(const char* className, const char* funcName, uint8_t value);
    bool CallStaticFloat(const char* className, const char* funcName, float value);
    bool CallStaticUInt8Bool(const char* className, const char* funcName, uint8_t p1, bool p2);
    bool CallStaticUInt8UInt8(const char* className, const char* funcName, uint8_t p1, uint8_t p2);
    bool CallStaticInt32Bool(const char* className, const char* funcName, int32_t p1, bool p2);
    bool CallStaticRaw(const char* className, const char* funcName, void* params, int32_t paramsSize);

    bool CallInstance(SDK::UObject* obj, const char* className, const char* funcName);
    bool CallInstanceBool(SDK::UObject* obj, const char* className, const char* funcName, bool value);
    bool CallInstanceUInt8Bool(SDK::UObject* obj, const char* className, const char* funcName, uint8_t p1, bool p2);

    int32_t CallStaticRetInt32(const char* className, const char* funcName);
    uint8_t CallStaticUInt8RetUInt8(const char* className, const char* funcName, uint8_t param);

    // Returns the bool ReturnValue of a no-arg static function
    bool CallStaticRetBool(const char* className, const char* funcName);

    // Calls a static function with one uint8 param and returns its bool ReturnValue
    // Used to call IsDriverSelectable(driverId) and read the result
    bool CallStaticUInt8RetBool(const char* className, const char* funcName, uint8_t param);

    int32_t GetEnumNum(const char* enumName);
    void ClearCache();
}

namespace ReflectRaw
{
    struct RawTArray { void* Data; int32_t NumElements; int32_t MaxElements; };

    inline void ClearTArray(void* arrayAddr) {
        reinterpret_cast<RawTArray*>(arrayAddr)->NumElements = 0;
    }
    inline int32_t TArrayNum(void* arrayAddr) {
        return reinterpret_cast<RawTArray*>(arrayAddr)->NumElements;
    }
    inline void* TArrayAt(void* arrayAddr, int32_t index, int32_t elementSize) {
        auto* arr = reinterpret_cast<RawTArray*>(arrayAddr);
        return reinterpret_cast<uint8_t*>(arr->Data) + index * elementSize;
    }
    inline int32_t ReadInt32(void* addr, int32_t offset) {
        return *reinterpret_cast<int32_t*>(reinterpret_cast<uint8_t*>(addr) + offset);
    }
    inline void WriteBool(void* addr, int32_t offset, bool value) {
        *reinterpret_cast<bool*>(reinterpret_cast<uint8_t*>(addr) + offset) = value;
    }
}
