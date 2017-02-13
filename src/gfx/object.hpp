#pragma once
#include "forward.hpp"
#include <GL/glew.h>
#include <limits>

namespace gfx
{
	class Object
	{
	public:
		Object()
			: m_vao(std::numeric_limits<GLuint>::max())
			, m_vertexCount(0)
		{}

		~Object()
		{
			if (m_vao != std::numeric_limits<GLuint>::max())
			{
				glDeleteVertexArrays(1, &m_vao);
			}
		}

		void setName(const std::string& name) { m_name = name; }
		const std::string& getName() const { return m_name; }

		void setVao(GLuint vao) { m_vao = vao; }
		void setVertexCount(int count) { m_vertexCount = count; }

		void render() const;

	private:
		std::string m_name;
		GLuint m_vao;
		int m_vertexCount;
	};
}
