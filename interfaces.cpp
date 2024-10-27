#include "interfaces.h"
#include <Windows.h>


void* GetInterface(HMODULE dllBase, const char* name)
{
	if (!dllBase)
		return NULL;

	using fnCreateInterface = void* (__cdecl*)(const char*, int*);
	fnCreateInterface CreateInterface = reinterpret_cast<fnCreateInterface>(GetProcAddress(dllBase, "CreateInterface"));

	if (!CreateInterface)
		return nullptr;

	int returnCode = 0;
	return CreateInterface(name, &returnCode);
}


bool interfaces::core::Setup() {
	wchar_t schemasystemName[] = L"schemasystem.dll";
	uintptr_t schema_system = memory::GetModuleBaseAddress(schemasystemName);
	
	if (!schema_system)
		return false;
	
	SchemaSystem = reinterpret_cast<ISchemaSystem*>(GetInterface(reinterpret_cast<HMODULE>(schema_system), "SchemaSystem_001"));

	return SchemaSystem != 0;
}