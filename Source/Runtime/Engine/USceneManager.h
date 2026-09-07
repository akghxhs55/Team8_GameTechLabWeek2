#pragma once
#include "UScene.h"
class USceneManager final
{

public:
	void SaveScene(const FString& path) const;
	void LoadScene();

	UScene* currentScene;

};