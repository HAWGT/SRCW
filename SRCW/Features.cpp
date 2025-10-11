#include "Features.h"

void HookGame()
{
	bool bHooked = false;

	while (!bHooked)
	{
		auto World = SDK::UWorld::GetWorld();
		auto Engine = SDK::UEngine::GetEngine();

		if (!World || !Engine || !Engine->GameViewport)
		{
			continue;
		}

		Orig_AActor_ProcessEvent = reinterpret_cast<AActor_ProcessEvent_t>(TrampHook64(PatternScan("48 89 5C 24 10 48 89 6C 24 18 57 48 83 EC ? F7 82 B0 00 00 00 ? ? ? ?"), (BYTE*)hk_AActor_ProcessEvent, 15));

		bHooked = true;
	}
}

void Clear(SDK::AActor* Class, SDK::UFunction* Function)
{
	for (int i = 0; i < SDK::UObject::GObjects->Num(); i++)
	{
		SDK::UObject* Obj = SDK::UObject::GObjects->GetByIndex(i);

		if (!Obj)
			continue;

		if (Obj->IsDefaultObject())
			continue;

		if (Obj->IsA(SDK::UContentDataAsset::StaticClass()))
		{
			auto Content = static_cast<SDK::UContentDataAsset*>(Obj);

			for (auto& cm : Content->CharaDLCMap)
			{
				cm.Value().DriverIds.Clear();
			}

			for (auto& mm : Content->MachineDLCMap)
			{
				mm.Value().MachineIds.Clear();
			}

			for (auto& sm : Content->StageDLCMap)
			{
				sm.Value().StageIds.Clear();
			}
		

			for (auto& hm : Content->HonorOtherTitleDLCMap)
			{
				hm.Value().HonorTitleIds.Clear();
			}

			for (auto& am : Content->AlbumDLCMap)
			{
				am.Value().AlbumIds.Clear();
			}

			for (auto& sm : Content->StickerDLCMap)
			{
				sm.Value().StickerIds.Clear();
			}

			std::cout << Class->GetName() << " " << Function->GetName() << " Cleared DLCs!\n";

			bCleared = true;
		}
	}
}

void UnlockAll()
{
	for (int i = 0; i < SDK::UObject::GObjects->Num(); i++)
	{
		SDK::UObject* Obj = SDK::UObject::GObjects->GetByIndex(i);

		if (!Obj)
			continue;

		if (Obj->IsDefaultObject())
			continue;

		if (Obj->IsA(SDK::UHonorTitleListDataAsset::StaticClass()))
		{
			auto Title = static_cast<SDK::UHonorTitleListDataAsset*>(Obj);

			for (const auto& t : Title->HonorTitleTableDataMap)
			{
				SDK::UAppSaveGameHelper::UnlockHonorTitle(t.Key());
			}
		}
	}

	for (int i = 0; i < (uint8_t)SDK::EDriverId::Num; i++)
	{
		SDK::UAppSaveGameHelper::SetDriverSelectable(SDK::EDriverId(i));
	}

	SDK::UMachineCustomizeUtilityLibrary::StoreAllAura();
	SDK::UMachineCustomizeUtilityLibrary::StoreAllHorn();
	SDK::UMachineCustomizeUtilityLibrary::StoreAllMachineAssembly();
	SDK::UMachineCustomizeUtilityLibrary::StoreAllMachineParts();
	SDK::UMachineCustomizeUtilityLibrary::StoreAllSticker();
	SDK::UMachineCustomizeUtilityLibrary::UnlockGadgetAll();

	std::cout << "Unlocked Everything!\n";

	bUnlocked = true;
}

void __fastcall hk_AActor_ProcessEvent(SDK::AActor* Class, SDK::UFunction* Function, void* Parms)
{
	if (!bCleared)
	{
		Clear(Class, Function);
		bCleared = true;
	}

	if (!Function->GetName().compare("OnInitStateSelectPlayMode") && !bUnlocked)
	{
		UnlockAll();
	}

	Orig_AActor_ProcessEvent(Class, Function, Parms);
}

