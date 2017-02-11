#include <cassert>

#include "object.hpp"
#include "shader.hpp"

void gfx::Object::render() const
{
	assert(m_vertexCount >= 3);
	assert(m_shader);
	assert(m_vao != std::numeric_limits<GLuint>::max());

	m_shader->use();
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}