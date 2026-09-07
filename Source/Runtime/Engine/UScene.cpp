#include "UScene.h"

#include "Runtime/CoreUObject/USceneComponent.h"
#include "Runtime/CoreUObject/UPrimitiveComponent.h"
#include "Runtime/Core/TArray.h"
#include <string>
#include "RunTime/CoreUObject/UClass.h"

void UScene::RegisterComponent(USceneComponent& Component)
{
	Component.OnRegister(*this);
	Components.push_back(&Component);
}

void UScene::UnregisterComponent(USceneComponent& Component)
{
	Component.OnUnregister(*this);
	// TODO: std 말고 TArray erase 사용
	std::erase(Components, &Component);
}

TArray<UPrimitiveComponent*> UScene::GetPrimitiveComponents() const
{
	// TODO: 매우 비효율적인 방식...
	TArray<UPrimitiveComponent*> PrimitiveComponents{};
	for (const auto* Component : Components)
	{
		if (const auto* PrimitiveComponent = dynamic_cast<const UPrimitiveComponent*>(Component))
		{
			PrimitiveComponents.push_back(const_cast<UPrimitiveComponent*>(PrimitiveComponent));
		}
	}
	return PrimitiveComponents;
}

json::JSON UScene::Serialize() const
{
	json::JSON result;

	result["NextUUID"] = NextUUID;

	for(UObject* object : Components)
	{
		if (object == nullptr)
			continue;

		json::JSON objectJson = object->Serialize();
		if(!objectJson.IsNull())
		{
			result["Primitives"][std::to_string(object->GetUUID())] = objectJson;
		}
	}


	return result;
}

void UScene::CreateFromJson(json::JSON data)
{
	Deserialize(data);
	


}
bool UScene::Deserialize(const json::JSON& data)
{
	NextUUID = data.at("NextUUID").ToInt();

	Components.clear();
	json::JSON primitivesJson = data.at("Primitives");
	for (auto& primitiveJson : primitivesJson.ObjectRange())
	{
		uint32 uuid = std::stoi(primitiveJson.first);
		json::JSON usceneComponentData = primitiveJson.second;


		UClass* _class = UClass::FindClassWithDisplayName(usceneComponentData.at("Type").ToString());
		UObject* obj = _class->CreateDefaultObject();
		USceneComponent* component = static_cast<USceneComponent*>(obj);
		
		component->Deserialize(usceneComponentData);
		component->SetUUID(uuid);

		RegisterComponent(*component);
	}
	

	return true;
}
