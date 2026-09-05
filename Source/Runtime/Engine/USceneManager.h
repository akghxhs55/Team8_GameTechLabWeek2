#pragma once
#include "UScene.h"
class USceneManager final
{
	TUniquePtr<UScene> CurrentScene;

	void SaveScene();
	void LoadScene();



};