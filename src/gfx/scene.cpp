#include "scene.hpp"
#include "object.hpp"
#include "node.hpp"
#include "shader.hpp"
#include "GL/glew.h"

gfx::ObjectPtr gfx::createCube()
{
	ObjectPtr obj = std::make_shared<Object>();

	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f, // Top Right
		0.5f, -0.5f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, // Bottom Left
		-0.5f, 0.5f, 0.0f, // Top Left 
	};

	GLuint indices[] = {
		0, 1, 3,   // First Triangle
		1, 2, 3    // Second Triangle
	};


	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	obj->setVao(VAO);
	obj->setVertexCount(6);

	glBindVertexArray(0);

	return obj;
}

gfx::NodePtr gfx::createNode()
{
	NodePtr node = std::make_shared<Node>();
	return node;
}

gfx::ShaderPtr gfx::createShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	ShaderPtr shader = std::make_shared<Shader>();
	shader->load(vertexShaderPath, fragmentShaderPath);
	return shader;
}

void gfx::Scene::render()
{
	for (auto& nodeObjPair : m_nodeObjectPairs)
	{
		nodeObjPair.second->render();
	}
}