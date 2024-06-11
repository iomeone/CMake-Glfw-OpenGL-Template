#pragma once

#include <Project/shader.hpp>
#include <Project/vertex_batcher.hpp>




#define SHADER_TYPES 5




namespace RenderManager
{

	Rendering::VertexBatcher g_batcher;

	Rendering::Shader	g_shaders[SHADER_TYPES];

	Rendering::Shader* GetShader(unsigned int shaderType);
	Rendering::VertexBatcher* GetBatcher();


};
