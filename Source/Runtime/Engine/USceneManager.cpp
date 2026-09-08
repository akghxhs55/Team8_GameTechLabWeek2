#include "USceneManager.h"
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>
#include "ThirdParty/Json/json.hpp"


void USceneManager::SaveScene(const FString& path) const
{
	std::filesystem::path fsPath(path);

	if (std::filesystem::exists(fsPath))
	{
		std::ifstream file(path);
		if (file)
		{
			std::stringstream buffer;
			buffer << file.rdbuf();
			json::JSON sceneData = json::JSON::Load(buffer.str());
		}
	}

	json::JSON sceneData = currentScene->Serialize();

	std::ofstream file(path);

	if (!file)
	{
		// Log error: failed to open file
		return;
	}

	file << sceneData.dump();

}

void USceneManager::LoadScene(const FString& path)
{
	std::ifstream file(path);
	if(!file)
	{

		return;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();

	json::JSON sceneData = json::JSON::Load(buffer.str());
	SetScene(NewObject<UScene>(*ResourceLibrary));
	currentScene->CreateFromJson(sceneData);

}

void USceneManager::SetScene(UScene* scene)
{
	if(currentScene != nullptr)
	{
		delete currentScene;
	}

	currentScene = scene;
	
}
