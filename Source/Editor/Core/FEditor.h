#pragma once
#include "Runtime/CoreUObject/UObject.h"

class FEditor final {
private:
	UObject* SelectedObject = nullptr;
public:
	bool SelectObject(UObject* Object);
	void UnSelectObject();
	UObject* GetSelectedObject();
};