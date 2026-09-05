#include "FUObjectArray.h"

#include <algorithm>

void FUObjectArray::AddObject(UObject* Object)
{
	Object->InternalIndex = static_cast<uint32>(Objects.size());
	Object->UUID = AcquireUUID();
	Objects.push_back(Object);
}

void FUObjectArray::RemoveObject(UObject* Object)
{
	std::erase(Objects, Object);
}
