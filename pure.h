#pragma once

#include <libloaderapi.h>

namespace ent_list {

	class IClientEntityList {
	public:
		// Get IClientNetworkable interface for specified entity.
		virtual void* GetClientNetworkable(int entnum) = 0;
		virtual void* GetClientNetworkableFromHandle(int hEnt) = 0;
		virtual void* GetClientUnknownFromHandle(int hEnt) = 0;
		// NOTE: This function is only a convenience wrapper.
		// It returns GetClientNetworkable( entnum )->GetIClientEntity().
		virtual void* GetClientEntity(int entnum) = 0;
		virtual void* GetClientEntityFromHandle(int hEnt) = 0;
		// Returns number of entities currently in use.
		virtual int	NumberOfEntities(bool bIncludeNonNetworkable) = 0;
		// Returns highest index actually used.
		virtual int	GetHighestEntityIndex(void) = 0;
		// Sizes entity list to specified size.
		virtual void SetMaxEntities(int maxents) = 0;
		virtual int GetMaxEntities() = 0;
	};

	typedef void* (__cdecl* tCreateInterface)(const char* name, int* returnCode);

	// ===================================
	//	Implement these in required files
	// ===================================
	// 
	// void* GetInterface(const char* dllname, const char* interfacename) {
	// 	tCreateInterface CreateInterface = (tCreateInterface) GetProcAddress(GetModuleHandleA(dllname), "CreateInterface");
	// 	int returnCode = 0;
	// 	void* interface_ = CreateInterface(interfacename, &returnCode);
	// 	return interface_;
	// }
	// IClientEntityList* client_entity_list = (ent_list::IClientEntityList*) ent_list::GetInterface("Client_panorama.dll", "VClientEntityList003");
	// ===================================
}