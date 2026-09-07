#pragma once

#include "Runtime/Core/IntTypes.h"

#include <concepts>

/*
 * UObject를 상속받는 클래스는 반드시 GENERATED_BODY() 매크로를 사용해야 한다.
 * 또한 public 생성자를 만들면 안 된다.
 */
#define GENERATED_BODY() \
	template <typename TObject, typename... TArgs> \
		requires std::derived_from<TObject, UObject> \
	friend TObject* NewObject(TArgs&&... Args);

class UObject
{
	GENERATED_BODY()

	friend class FUObjectArray;

public:
	[[nodiscard]] uint32 GetUUID() const { return UUID; }

	UObject(const UObject&) = delete;
	UObject& operator=(const UObject&) = delete;

	UObject(UObject&&) = delete;
	const UObject& operator=(UObject&&) = delete;

	virtual void Serialize();

protected:
	UObject() = default;
	virtual ~UObject() = default;

private:
	uint32 UUID = 0u;
	uint32 InternalIndex = 0u;
};
