#pragma once
#include <glad/glad.h>
#include <string>
#include <memory>
#include <map>
namespace Renderer {
	class basicShader;
}

class Manager {
public:
	Manager(const std::string& exePath);
	~Manager() = default;

	Manager(const Manager&) = delete;
	Manager& operator = (const Manager&) = delete;
	Manager& operator = (Manager&&) = delete;
	Manager(Manager&&) = delete;

	std::shared_ptr<Renderer::basicShader> loadShaders(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
	std::shared_ptr<Renderer::basicShader> getShader(const std::string& name);

private:
	std::string getFileData(const std::string& relPath) const;
	typedef std:: map<std::string, std::shared_ptr<Renderer::basicShader>> shadersMap;
	shadersMap mShaders;
	std::string mPath;
};