#pragma once

#include "Runtime/Core/Types.h"

class UObject
{
	friend class FUObjectArray;

public:
	[[nodiscard]] uint32 GetUUID() const { return UUID; }

protected:
	UObject() = default;
	virtual ~UObject() = default;

private:
	uint32 UUID = 0u;
	uint32 InternalIndex = 0u;
};
