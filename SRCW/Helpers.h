#pragma once

#include <Windows.h>
#include <basetsd.h>
#include <minwindef.h>
#include <memoryapi.h>
#include <vector>
#include <cstdint>
#include <unordered_map>

inline void* module = GetModuleHandle(nullptr);

BYTE* PatternScan(const char* signature);
void Patch(BYTE* src, BYTE* dst, const ULONG64 size);
bool Detour64(BYTE* src, BYTE* dst, const ULONG64 size);
BYTE* TrampHook64(BYTE* src, BYTE* dst, const ULONG64 size);
BYTE* RemoveHook(BYTE* src, BYTE* orig, const ULONG64 size);
std::uintptr_t GetAddressFromInstruction(std::uintptr_t address, int instruction_size);
const void* VTHook(const void** vtable, const int index, const void* hook);
void* ShadowVT(void* instance);

inline std::unordered_map <uint64_t, uint64_t> ShadowVTMap;
