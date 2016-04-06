#include "GLSLProgram.h"

#include <fstream>
#include <vector>
#include "Err.h"

namespace WebEngine {

	//Initialization lists are faster
	GLSLProgram::GLSLProgram()
	{
		//Empty
	}


	GLSLProgram::~GLSLProgram()
	{
		//Empty
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
	{
		std::string vertSource;
		std::string fragSource;

		IOManager::readFileToBuffer(vertexShaderFilePath, vertSource);
		IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);

		compileShadersFromSource(vertSource.c_str(), fragSource.c_str());
	}

	void GLSLProgram::compileShadersFromSource(const char* vertSource, const char* fragSource)
	{
		//Link compiled vertex and fragment shaders into a program
		mProgramID = glCreateProgram();

		mVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (mVertexShaderID == 0)
		{
			fatalError("Vertex shader failed to be created");
		}

		mFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (mFragmentShaderID == 0)
		{
			fatalError("Fragment shader failed to be created");
		}

		//compile shaders with modular compiler
		compileShader(vertSource, "Vertex Shader", mVertexShaderID);
		compileShader(fragSource, "Fragment Shader", mFragmentShaderID);

	}


	void GLSLProgram::linkShaders()
	{
		//Vertex and fragment shaders are successfully compiled.
		//Now time to link them together into a program.

		//Attach our shaders to our program
		glAttachShader(mProgramID, mVertexShaderID);
		glAttachShader(mProgramID, mFragmentShaderID);

		//Link our program
		glLinkProgram(mProgramID);

		//Error Checking
		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(mProgramID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(mProgramID, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(mProgramID);
			//Don't leak shaders either.
			glDeleteShader(mVertexShaderID);
			glDeleteShader(mFragmentShaderID);

			//use errorLog to print errors
			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaders failed to link");
		}

		//Always detach shaders after a successful link.
		glDetachShader(mProgramID, mVertexShaderID);
		glDetachShader(mProgramID, mFragmentShaderID);
		glDeleteShader(mVertexShaderID);
		glDeleteShader(mFragmentShaderID);
	}

	void GLSLProgram::addAttribute(const std::string& attributeName)
	{
		glBindAttribLocation(mProgramID, mNumAttributes++, attributeName.c_str());
	}

	//get location of uniform variable from shaders wrapper class
	GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
	{
		GLint location = glGetUniformLocation(mProgramID, uniformName.c_str());
		if (location == GL_INVALID_INDEX)
		{
			fatalError("Uniform" + uniformName + "not found in shader");
		}
		return location;
	}

	//enable shader and all its attr.
	void GLSLProgram::use()
	{
		glUseProgram(mProgramID);
		//enable attributes
		for (int i = 0; i < mNumAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}
	//disable shader
	void GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < mNumAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLProgram::compileShader(const char* source, const std::string& name, GLuint ID)
	{
		//Tell opengl we want to use fileContents as the contents of the shader file
		//ID, number of strings, array of string pointers, pointer to rray of ints that give length of each line
		glShaderSource(ID, 1, &source, nullptr);

		//compile shader
		glCompileShader(ID);

		//Error checking
		GLint success = 0;
		glGetShaderiv(ID, GL_COMPILE_STATUS,  &success);

		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &maxLength);

			//the maxlength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(ID, maxLength, &maxLength, &errorLog[0]);

			//Provide infolog in whatever manor you deem best
			//Exit with failure
			glDeleteShader(ID);//Don't leak shader

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader" + name + " failed to compile");
		}
	}

	void GLSLProgram::close()
	{
		if (mProgramID)
		{
			glDeleteProgram(mProgramID);
		}
	}

}