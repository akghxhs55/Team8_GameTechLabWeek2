#pragma once
#include "Runtime/Core/TFunction.h"
#include "Runtime/Core/FString.h"
#include "Runtime/Core/TArray.h"
#include "Runtime/Core/IntTypes.h"
#include "Runtime/Core/PointerTypes.h"
#include "Runtime/Core/TMap.h"

class UObject;

class UClass
{
private:
	static inline uint32 registeredCount = 0;
	static inline TArray<TUniquePtr<UClass>> classList;
	static inline TMap<FString, uint32> nameToId;
	static inline TMap<FString, uint32> displayNameToId;
	FString className, superClassTypeName;
	TFunction<UObject* ()> createFunction;
	uint32 typeId;
	TMap<FString, FString> metadata;

public:
	UObject* CreateDefaultObject() const; 
	static UClass* RegisterToFactory(
		const FString& typeName,
		const TFunction<UObject* ()>& createFunction, 
		const FString& superClassTypeName);

	static UClass* FindByName(const FString& Name);
	const FString& GetDisplayName() const;
	void SetMeta(const FString& key, const FString& value);


	static UClass* FindClassWithDisplayName(const FString& name)
	{
		// 1) DisplayName lookup
		auto it = displayNameToId.find(name);
		if (it != displayNameToId.end())
			return GetClassById(it->second);

		// 2) className fallback
		it = nameToId.find(name);
		return (it != nameToId.end()) ? GetClassById(it->second) : nullptr;
	}

	static UClass* GetClassById(uint32 typeId) {
		return (typeId < classList.size()) ? classList[typeId].get() : nullptr;
	}





};