#pragma once

#include <Project/shader.hpp>
#include <Project/vertex_batcher.hpp>




#define SHADER_TYPE_DEFAULT 0
#define SHADER_TYPE_BILLBOARD 1
#define SHADER_TYPE_SCREEN 2
#define SHADER_TYPE_FX_GAUSSIAN_9 3
#define SHADER_TYPE_TEST 4
#define SHADER_TYPES 5




namespace RenderManager
{



	Rendering::Shader* GetShader(unsigned int shaderType);
	Rendering::VertexBatcher* GetBatcher();

	bool Initialize();


};
