#include "shader.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

bool gfx::Shader::load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	bool allOk(true);

	std::ifstream fstream;
	std::stringstream vertexShaderStream;
	std::stringstream fragmentShaderStream;
	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	fstream.open(vertexShaderPath);
	vertexShaderStream << fstream.rdbuf();
	vertexShaderSource = vertexShaderStream.str();
	const GLchar* glCVertexShaderSource = vertexShaderSource.c_str();

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &glCVertexShaderSource, 0);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
		std::cout << "VERTEX SHADER ERROR:" << infoLog << std::endl;
		allOk = false;
	}

	fstream.close();
	fstream.open(fragmentShaderPath);
	fragmentShaderStream << fstream.rdbuf();
	fragmentShaderSource = fragmentShaderStream.str();
	const GLchar* glCFragmentShaderSource = fragmentShaderSource.c_str();

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &glCFragmentShaderSource, 0);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
		std::cout << "FRAGMENT SHADER ERROR:" << infoLog << std::endl;
		allOk = false;
	}

	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	glLinkProgram(m_shaderProgram);

	glGetProgramiv(m_shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		m_shaderProgram = std::numeric_limits<GLuint>::max();
		glGetProgramInfoLog(m_shaderProgram, 512, 0, infoLog);
		std::cout << "PROGRAM ERROR:" << infoLog << std::endl;
		allOk = false;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return allOk;
}