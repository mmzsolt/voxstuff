#include <cassert>

#include "object.hpp"
#include "shader.hpp"

void gfx::Object::render() const
{
	assert(m_indexCount >= 3);
	assert(m_vao != std::numeric_limits<GLuint>::max());

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}