#include "Manager.h"
#include "../Renderer/basicShader.h"

#include <sstream>
#include <fstream>
#include <iostream>

Manager::Manager(const std::string& executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    mPath = executablePath.substr(0, found);
}

std::string Manager::getFileData(const std::string& relativeFilePath) const
{
    std::ifstream f;
    f.open(mPath + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
    if (!f.is_open())
    {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return std::string{};
    }

    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

std::shared_ptr<Renderer::basicShader> Manager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexString = getFileData(vertexPath);
    if (vertexString.empty())
    {
        std::cerr << "No vertex shader!" << std::endl;
        return nullptr;
    }

    std::string fragmentxString = getFileData(fragmentPath);
    if (fragmentxString.empty())
    {
        std::cerr << "No fragment shader!" << std::endl;
        return nullptr;
    }

    std::shared_ptr<Renderer::basicShader>& newShader =mShaders.emplace(shaderName, std::make_shared<Renderer::basicShader>(vertexString, fragmentxString)).first->second;
    if (newShader->isCompiled())
    {
        return newShader;
    }

    std::cerr << "Can't load shader program:\n"
        << "Vertex: " << vertexPath << "\n"
        << "Fragment: " << fragmentPath << std::endl;

    return nullptr;
}


std::shared_ptr<Renderer::basicShader> Manager::getShader(const std::string& shaderName)
{
    shadersMap::const_iterator it = mShaders.find(shaderName);
    if (it != mShaders.end())
    {
        return it->second;
    }
    std::cerr << "Can't find the shader program: " << shaderName << std::endl;
    return nullptr;
}