#pragma once
#include <string>
#include <GL/glew.h>
#include "IOManager.h"

namespace WebEngine {

	//This class handles the compilation, linking, and usage of a GLSL shader program.
	//Reference: http://www.opengl.org/wiki/Shader_Compilation
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

		void linkShaders();

		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string& uniformName);

		void use();
		void unuse();

		void close();

	private:

		int mNumAttributes = 0;

		void compileShader(const char* source, const std::string& name, GLuint ID);

		//handle for program
		GLuint mProgramID = 0;

		GLuint mVertexShaderID = 0;
		GLuint mFragmentShaderID = 0;

	};

}