#include "Reflect.h"

namespace Reflect
{
    static std::unordered_map<std::string, SDK::UClass*> classCache;
    static std::unordered_map<std::string, SDK::UFunction*> funcCache;
    static std::unordered_map<std::string, int32_t> propOffsetCache;

    void ClearCache() { classCache.clear(); funcCache.clear(); propOffsetCache.clear(); }

    SDK::UClass* FindClass(const char* className)
    {
        auto it = classCache.find(className);
        if (it != classCache.end()) return it->second;
        SDK::UClass* cls = SDK::UObject::FindClassFast(className);
        if (cls) classCache[className] = cls;
        return cls;
    }

    SDK::UObject* FindCDO(const char* className)
    {
        SDK::UClass* cls = FindClass(className);
        return cls ? cls->ClassDefaultObject : nullptr;
    }

    SDK::UObject* FindInstance(const char* className)
    {
        SDK::UClass* cls = FindClass(className);
        if (!cls) return nullptr;
        for (int i = 0; i < SDK::UObject::GObjects->Num(); i++)
        {
            SDK::UObject* obj = SDK::UObject::GObjects->GetByIndex(i);
            if (!obj || obj->IsDefaultObject()) continue;
            if (obj->IsA(cls)) return obj;
        }
        return nullptr;
    }

    SDK::UFunction* FindFunction(const char* className, const char* funcName)
    {
        std::string key = std::string(className) + "::" + funcName;
        auto it = funcCache.find(key);
        if (it != funcCache.end()) return it->second;
        SDK::UClass* cls = FindClass(className);
        if (!cls) return nullptr;
        SDK::UFunction* func = cls->GetFunction(className, funcName);
        if (func) funcCache[key] = func;
        return func;
    }

    SDK::FProperty* FindProperty(SDK::UStruct* structType, const char* propName)
    {
        if (!structType) return nullptr;
        for (SDK::FField* field = structType->ChildProperties; field; field = field->Next)
        {
            if (field->Name.ToString() == propName)
                return static_cast<SDK::FProperty*>(field);
        }
        if (structType->SuperStruct)
            return FindProperty(structType->SuperStruct, propName);
        return nullptr;
    }

    int32_t FindPropertyOffset(SDK::UStruct* structType, const char* propName)
    {
        if (!structType) return -1;
        std::string key = structType->GetName() + "." + propName;
        auto it = propOffsetCache.find(key);
        if (it != propOffsetCache.end()) return it->second;
        SDK::FProperty* prop = FindProperty(structType, propName);
        if (!prop) return -1;
        propOffsetCache[key] = prop->Offset;
        return prop->Offset;
    }

    static SDK::FProperty* FindFirstParam(SDK::UFunction* func)
    {
        for (SDK::FField* field = func->ChildProperties; field; field = field->Next)
        {
            auto* prop = static_cast<SDK::FProperty*>(field);
            if (!(prop->PropertyFlags & static_cast<uint64_t>(SDK::EPropertyFlags::ReturnParm)))
                return prop;
        }
        return nullptr;
    }

    static SDK::FProperty* FindReturnParam(SDK::UFunction* func)
    {
        for (SDK::FField* field = func->ChildProperties; field; field = field->Next)
        {
            auto* prop = static_cast<SDK::FProperty*>(field);
            if (prop->PropertyFlags & static_cast<uint64_t>(SDK::EPropertyFlags::ReturnParm))
                return prop;
        }
        return nullptr;
    }

    static SDK::FProperty* FindNthParam(SDK::UFunction* func, int n)
    {
        int count = 0;
        for (SDK::FField* field = func->ChildProperties; field; field = field->Next)
        {
            auto* prop = static_cast<SDK::FProperty*>(field);
            if (!(prop->PropertyFlags & static_cast<uint64_t>(SDK::EPropertyFlags::ReturnParm)))
            {
                if (count == n) return prop;
                count++;
            }
        }
        return nullptr;
    }

    bool CallStatic(const char* className, const char* funcName)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return false;
        uint8_t params[512] = {};
        cdo->ProcessEvent(func, func->Size > 0 ? params : nullptr);
        return true;
    }

    bool CallStaticBool(const char* className, const char* funcName, bool value)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return false;
        uint8_t params[512] = {};
        SDK::FProperty* p = FindFirstParam(func);
        if (p) *(bool*)(params + p->Offset) = value;
        cdo->ProcessEvent(func, params);
        return true;
    }

    bool CallStaticInt32(const char* className, const char* funcName, int32_t value)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return false;
        uint8_t params[512] = {};
        SDK::FProperty* p = FindFirstParam(func);
        if (p) *(int32_t*)(params + p->Offset) = value;
        cdo->ProcessEvent(func, params);
        return true;
    }

    bool CallStaticUInt8(const char* className, const char* funcName, uint8_t value)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return false;
        uint8_t params[512] = {};
        SDK::FProperty* p = FindFirstParam(func);
        if (p) *(uint8_t*)(params + p->Offset) = value;
        cdo->ProcessEvent(func, params);
        return true;
    }

    bool CallStaticFloat(const char* className, const char* funcName, float value)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return false;
        uint8_t params[512] = {};
        SDK::FProperty* p = FindFirstParam(func);
        if (p) *(float*)(params + p->Offset) = value;
        cdo->ProcessEvent(func, params);
        return true;
    }

    bool CallStaticUInt8Bool(const char* className, const char* funcName, uint8_t p1, bool p2)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return false;
        uint8_t params[512] = {};
        SDK::FProperty* param0 = FindNthParam(func, 0);
        SDK::FProperty* param1 = FindNthParam(func, 1);
        if (param0) *(uint8_t*)(params + param0->Offset) = p1;
        if (param1) *(bool*)(params + param1->Offset) = p2;
        cdo->ProcessEvent(func, params);
        return true;
    }

    bool Reflect::CallStaticUInt8UInt8(const char* className, const char* funcName, uint8_t p1, uint8_t p2)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);

        if (!func || !cdo)
            return false;

        uint8_t params[512] = {};

        SDK::FProperty* param0 = FindNthParam(func, 0);
        SDK::FProperty* param1 = FindNthParam(func, 1);

        if (!param0 || !param1)
            return false;

        *(uint8_t*)(params + param0->Offset) = p1;
        *(uint8_t*)(params + param1->Offset) = p2;

        cdo->ProcessEvent(func, params);
        return true;
    }

    bool CallStaticInt32Bool(const char* className, const char* funcName, int32_t p1, bool p2)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return false;
        uint8_t params[512] = {};
        SDK::FProperty* param0 = FindNthParam(func, 0);
        SDK::FProperty* param1 = FindNthParam(func, 1);
        if (param0) *(int32_t*)(params + param0->Offset) = p1;
        if (param1) *(bool*)(params + param1->Offset) = p2;
        cdo->ProcessEvent(func, params);
        return true;
    }

    bool CallStaticRaw(const char* className, const char* funcName, void* paramsIn, int32_t paramsSize)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return false;
        cdo->ProcessEvent(func, paramsIn);
        return true;
    }

    bool CallInstance(SDK::UObject* obj, const char* className, const char* funcName)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        if (!func || !obj) return false;
        uint8_t params[512] = {};
        obj->ProcessEvent(func, func->Size > 0 ? params : nullptr);
        return true;
    }

    bool CallInstanceBool(SDK::UObject* obj, const char* className, const char* funcName, bool value)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        if (!func || !obj) return false;
        uint8_t params[512] = {};
        SDK::FProperty* p = FindFirstParam(func);
        if (p) *(bool*)(params + p->Offset) = value;
        obj->ProcessEvent(func, params);
        return true;
    }

    bool CallInstanceUInt8Bool(SDK::UObject* obj, const char* className, const char* funcName, uint8_t p1, bool p2)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        if (!func || !obj) return false;
        uint8_t params[512] = {};
        SDK::FProperty* param0 = FindNthParam(func, 0);
        SDK::FProperty* param1 = FindNthParam(func, 1);
        if (param0) *(uint8_t*)(params + param0->Offset) = p1;
        if (param1) *(bool*)(params + param1->Offset) = p2;
        obj->ProcessEvent(func, params);
        return true;
    }

    int32_t CallStaticRetInt32(const char* className, const char* funcName)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return 0;
        uint8_t params[512] = {};
        cdo->ProcessEvent(func, params);
        SDK::FProperty* ret = FindReturnParam(func);
        if (!ret) return 0;
        return *(int32_t*)(params + ret->Offset);
    }

    uint8_t CallStaticUInt8RetUInt8(const char* className, const char* funcName, uint8_t param)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return 0;
        uint8_t params[512] = {};
        SDK::FProperty* p = FindFirstParam(func);
        if (p) *(uint8_t*)(params + p->Offset) = param;
        cdo->ProcessEvent(func, params);
        SDK::FProperty* ret = FindReturnParam(func);
        if (!ret) return 0;
        return *(uint8_t*)(params + ret->Offset);
    }

    bool CallStaticRetBool(const char* className, const char* funcName)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return false;
        uint8_t params[512] = {};
        cdo->ProcessEvent(func, params);
        SDK::FProperty* ret = FindReturnParam(func);
        if (!ret) return false;
        return *(bool*)(params + ret->Offset);
    }

    bool CallStaticUInt8RetBool(const char* className, const char* funcName, uint8_t param)
    {
        SDK::UFunction* func = FindFunction(className, funcName);
        SDK::UObject* cdo = FindCDO(className);
        if (!func || !cdo) return false;
        uint8_t params[512] = {};
        SDK::FProperty* p = FindFirstParam(func);
        if (p) *(uint8_t*)(params + p->Offset) = param;
        cdo->ProcessEvent(func, params);
        SDK::FProperty* ret = FindReturnParam(func);
        if (!ret) return false;
        return *(bool*)(params + ret->Offset);
    }

    int32_t GetEnumNum(const char* enumName)
    {
        for (int i = 0; i < SDK::UObject::GObjects->Num(); i++)
        {
            SDK::UObject* obj = SDK::UObject::GObjects->GetByIndex(i);
            if (!obj) continue;
            if (obj->IsA(SDK::EClassCastFlags::Enum) && obj->GetName() == enumName)
            {
                auto* uenum = static_cast<SDK::UEnum*>(obj);
                for (int j = 0; j < uenum->Names.Num(); j++)
                {
                    std::string name = uenum->Names[j].Key().ToString();
                    size_t colonPos = name.rfind("::");
                    std::string valueName = (colonPos != std::string::npos) ? name.substr(colonPos + 2) : name;
                    if (valueName == "Num")
                        return (int32_t)uenum->Names[j].Value();
                }
                if (uenum->Names.Num() > 1)
                    return (int32_t)uenum->Names[uenum->Names.Num() - 2].Value();
            }
        }
        std::cout << "[Reflect] Warning: enum '" << enumName << "' not found\n";
        return 0;
    }
}
