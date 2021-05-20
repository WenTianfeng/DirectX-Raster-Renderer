#pragma once

#include <io.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

//常用文件路径
const std::string SceneFiles = "Assets\\Scenes\\";
const std::string ShaderFiles = "Assets\\Shaders\\";

const std::string PresetTexturesFiles = "Assets\\Preset\\Textures\\";
const std::string PresetMeshFiles = "Assets\\Preset\\Meshes\\";
const std::string PresetFontFiles = "Assets\\Preset\\Fonts\\";
const std::string PresetAudioFiles = "Assets\\Preset\\Audios\\";

const std::string CustomizedTextureFiles = "Assets\\Custom\\Textures\\";
const std::string CustomizedMeshFiles = "Assets\\Custom\\Meshes\\";
const std::string CustomizedFontFiles = "Assets\\Custom\\Fonts\\";
const std::string CustomizedAudioFiles = "Assets\\Custom\\Audios\\";

class FileManager
{
public:
	static void GetFilePathsByFormat(std::string path, std::vector<std::string>& files, std::string format);
	static void GetFileNamesByFormat(std::string path, std::vector<std::string>& files, std::string format);
};
