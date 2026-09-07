#pragma once
#include "UScene.h"
class USceneManager final // TODO: UObject 상속이 아니므로 F 접두어이어야 할 필요 있을듯
{

public:
	void SaveScene(const FString& path) const;
	void LoadScene();

	UScene* currentScene = nullptr;

};