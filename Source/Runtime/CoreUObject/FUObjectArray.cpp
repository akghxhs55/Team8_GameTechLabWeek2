#include "FUObjectArray.h"

#include <algorithm>

FUObjectArray GUObjectArray;

void FUObjectArray::AddObject(UObject* Object)
{
	Object->InternalIndex = static_cast<uint32>(Objects.size());
	Object->UUID = NextUUID++;
	Objects.push_back(Object);
}

void FUObjectArray::RemoveObject(UObject* Object)
{
	Objects.erase(std::remove(Objects.begin(), Objects.end(), Object), Objects.end());
}
