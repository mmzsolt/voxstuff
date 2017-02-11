#pragma once

#include "GL/glew.h"
#include <limits>
#include <string>

namespace gfx
{
	class Shader
	{
	public:
		Shader()
			: m_shaderProgram(std::numeric_limits<GLuint>::max())
		{}
		~Shader()
		{
			if (m_shaderProgram != std::numeric_limits<GLuint>::max())
			{
			}
		}

		bool load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		void use() const
		{
			glUseProgram(m_shaderProgram);
		}

		void setUniform2f(const std::string& name, float f1, float f2)
		{
			GLint location = glGetUniformLocation(m_shaderProgram, name.c_str());
			use();
			glUniform2f(location, f1, f2);
		}

		void setUniform1f(const std::string& name, float f)
		{
			GLint location = glGetUniformLocation(m_shaderProgram, name.c_str());
			use();
			glUniform1f(location, f);
		}

		void setTextureSampler(int samplerIdx, GLuint textureId, const std::string& uniformName)
		{
			glActiveTexture(GL_TEXTURE0 + samplerIdx);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glUniform1i(glGetUniformLocation(m_shaderProgram, uniformName.c_str()), samplerIdx);
		}

	private:
		GLuint m_shaderProgram;
	};
}