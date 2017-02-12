#include "GL/glew.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "scene.hpp"
#include "object.hpp"
#include "node.hpp"
#include "shader.hpp"
#include "camera.hpp"

gfx::ObjectPtr gfx::createCube()
{
	ObjectPtr obj = std::make_shared<Object>();

	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f, // Top Right
		0.5f, -0.5f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, // Bottom Left
		-0.5f, 0.5f, 0.0f, // Top Left 
		0.5f, 0.5f, 0.5f, // Top Right Back
		0.5f, -0.5f, 0.5f, // Bottom Right Back
		-0.5f, -0.5f, 0.5f, // Bottom Left Back
		-0.5f, 0.5f, 0.5f, // Top Left Back
	};

	// strange how hard it is to find the index representation of the faces of a cube on the net
	GLuint indices[] = {
		0, 1, 3,   // First Triangle
		1, 2, 3,   // Second Triangle
		4, 5, 7,   // First Triangle
		5, 6, 7,   // Second Triangle
		//0, 1, 4,   // First Triangle
		//1, 4, 5,
		2, 3, 7
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
	obj->setVertexCount(sizeof(indices));

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
	Eigen::Matrix4f persp = perspective(static_cast<float>(45.0f * M_PI / 180.0f), 1.0f, 1.0f, 100.0f);

	m_nodeObjectPairs.begin()->second->getShader()->setUniformM4f("persp", &persp(0));

	for (auto& nodeObjPair : m_nodeObjectPairs)
	{
		auto& node = nodeObjPair.first;
		auto& obj = nodeObjPair.second;

		Eigen::Matrix4f finalTransform = node->getTransform();
		obj->getShader()->setUniformM4f("transform", &finalTransform(0));
		obj->render();
	}
}