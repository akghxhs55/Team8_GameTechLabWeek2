#include "UScene.h"

#include "Runtime/CoreUObject/USceneComponent.h"
#include "Runtime/CoreUObject/UPrimitiveComponent.h"
#include "Runtime/Core/TArray.h"

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
