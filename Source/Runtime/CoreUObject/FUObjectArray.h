#pragma once

#include "UObject.h"
#include "Runtime/Core/TArray.h"
#include "Runtime/Core/IntTypes.h"

class FUObjectArray final
{
public:
	uint32 GetNextUUID() { return NextUUID++; }
	[[nodiscard]] uint32 GetNumObjects() const { return static_cast<uint32>(Objects.size()); }
	[[nodiscard]] UObject* GetObjectByIndex(uint32 Index) const { return Objects[Index]; }

private:
	void AddObject(UObject* Object);
	void RemoveObject(UObject* Object);

	TArray<UObject*> Objects;
	uint32 NextUUID = 1u;
};

extern FUObjectArray GUObjectArray;

template <typename TObject, typename ... TArgs>
	requires std::derived_from<TObject, UObject>
TObject* NewObject(TArgs&&... Args)
{
	TObject* Object = new TObject(std::forward<TArgs>(Args)...);
	GUObjectArray.AddObject(Object);
	return Object;
}
