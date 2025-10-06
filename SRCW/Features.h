#pragma once
#include <random>

#include "Helpers.h"
#include "CppSDK/SDK/Engine_classes.hpp"
#include "CppSDK/SDK/BP_UnionGameInstance_classes.hpp"
#include "CppSDK/SDK/BP_MenuPlayerController_classes.hpp"
#include "CppSDK/SDK/UnionSystem_classes.hpp"
#include "CppSDK/SDK/UnionSystem_structs.hpp"
#include "CppSDK/SDK/UNION_parameters.hpp"
#include "CppSDK/SDK/BP_MenuRacerLobby_classes.hpp"
#include "CppSDK/SDK/BP_MenuDriverPawn_classes.hpp"

inline const char ConfigFileName[] = ".\\UNION\\Binaries\\Win64\\SRCW.ini";

inline bool bUnlocked = false;

void HookGame();
void UnlockAll();

void __fastcall hk_AActor_ProcessEvent(SDK::AActor* Class, SDK::UFunction* Function, void* Parms);
typedef void(__fastcall* AActor_ProcessEvent_t)(SDK::AActor* Class, SDK::UFunction* Function, void* Parms);
inline AActor_ProcessEvent_t Orig_AActor_ProcessEvent;

