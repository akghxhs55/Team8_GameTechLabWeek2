#include "UClass.h"
#include "Runtime/Core/PointerTypes.h"
UClass* UClass::RegisterToFactory(const FString& typeName, const TFunction<UObject* ()>& createFunction, const FString& superClassTypeName)
{

    TUniquePtr<UClass> classType = MakeUnique<UClass>();
    classType->className = typeName;
    classType->superClassTypeName = superClassTypeName;
    classType->createFunction = createFunction;
    classType->typeId = registeredCount++;

    nameToId[typeName] = classType->typeId;

    UClass* rawPtr = classType.get();


    classList.push_back(std::move(classType));
    return rawPtr;
}
UClass* UClass::FindByName(const FString& Name)
{
    auto it = nameToId.find(Name);
    return (it != nameToId.end()) ? classList[it->second].get() : nullptr;
}

const FString& UClass::GetDisplayName() const
{
    auto itr = metadata.find("DisplayName");
    if (itr != metadata.end())
    {
        return itr->second;
    }

    return className;
}

void UClass::SetMeta(const FString& key, const FString& value)
{
    metadata[key] = value;

    if (key == "DisplayName")
    {
        displayNameToId[value] = typeId;  // typeId는 인스턴스 멤버
    }

}

UObject* UClass::CreateDefaultObject() const
{
    return createFunction ? createFunction() : nullptr;
}

