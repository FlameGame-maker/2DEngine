#pragma once
#include <glad/glad.h>
#include <string>

namespace Renderer {
	class basicShader {
	public:
		basicShader(const std::string& vertexShader, const std::string& fragmentShader);
		~basicShader();
		bool isCompiled() const {return vIsCompiled;}
		void use() const;

		basicShader() = delete;
		basicShader(const basicShader&) = delete;
		basicShader& operator = (const basicShader&) = delete;
		basicShader& operator = (basicShader&& basicShader) noexcept;
		basicShader(basicShader&& basicShader) noexcept;
	private:
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
		bool vIsCompiled = false;
		GLuint programID = 0;
	};
}