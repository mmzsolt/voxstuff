#pragma once
#include "forward.hpp"
#include <GL/glew.h>
#include <limits>
#include <assert.h>
#include "stuff/optional.hpp"
#include <numeric>

namespace gfx
{
	struct VertexAttribDescription
	{
		VertexAttribDescription(int t, int c)
			: type(t)
			, count(c)
		{}

		int type = 0;
		int count = 0;
		int getSizeInBytes() const
		{
			switch (type)
			{
			case GL_BYTE:
			case GL_UNSIGNED_BYTE:
				return 1;

			case GL_SHORT:
			case GL_UNSIGNED_SHORT:
			case GL_HALF_FLOAT:
				return 2;

			case GL_INT:
			case GL_UNSIGNED_INT:
			case GL_FLOAT:
			case GL_FIXED:
				return 4;

			case GL_DOUBLE:
				return 8;

			default:
				assert("vertex attrib description from hell");
				return 0;
			}
		}
	};

	typedef std::vector<VertexAttribDescription> VertexAttribDescriptionVec;

	// yeah, i don't know what i'm doing here, but this is fine :)
	template <typename VertexContainerType, typename IndexContainerType>
	std::experimental::optional<GLuint> createVAO(const VertexContainerType& vertices, const IndexContainerType& indices, const VertexAttribDescriptionVec& descriptions)
	{
		std::experimental::optional<GLuint> ret = 0;

		GLuint VAO;
		glGenVertexArrays(1, &VAO);

		if (VAO == 0)
		{
			return ret;
		}

		glBindVertexArray(VAO);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		if (VBO == 0)
		{
			return ret;
		}

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(typename VertexContainerType::value_type), &(*vertices.begin()), GL_STATIC_DRAW);

		GLuint EBO;
		glGenBuffers(1, &EBO);
		if (EBO == 0)
		{
			return ret;
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(typename IndexContainerType::value_type), &(*indices.begin()), GL_STATIC_DRAW);
		
		long long offset = 0;
		GLsizei stride = std::accumulate(descriptions.begin(), descriptions.end(), 0, [](GLsizei curr, auto desc) {return curr + (desc.count * desc.getSizeInBytes()); });
		for (size_t i = 0, n = descriptions.size(); i < n; ++i)
		{
			const auto& desc = descriptions[i];
			glVertexAttribPointer(static_cast<GLuint>(i), desc.count, desc.type, false, stride, reinterpret_cast<void*>(offset));
			glEnableVertexAttribArray(i);
			offset += desc.count * desc.getSizeInBytes();
		}

		glBindVertexArray(0);

		ret = VAO;
		return ret;
	}

	class Object
	{
	public:
		Object()
			: m_vao(std::numeric_limits<GLuint>::max())
			, m_indexCount(0)
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
		void setIndexCount(int count) { m_indexCount = count; }

		void render() const;

	private:
		std::string m_name;
		GLuint m_vao;
		int m_indexCount;
	};
}
