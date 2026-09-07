#include "UObject.h"
#include "UClass.h"
#include "UObjectGlobals.h"

IMPLEMENT_ROOT_UCLASS(UObject)

json::JSON UObject::Serialize() const
{
	return json::JSON();
}

bool UObject::Deserialize(const json::JSON& data)
{
	return false;
}
