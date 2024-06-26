#pragma once

#include <Project.Library/Application.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <string_view>
#include <vector>
#include <memory>



#include <Project/vertex_batcher.hpp>
#include <Project/shader.hpp>
#include <Project/texture.hpp>


struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 Uv;
    glm::vec4 Tangent;
};

struct MeshIndirectInfo
{
    uint32_t Count;
    uint32_t InstanceCount;
    uint32_t FirstIndex;
    int32_t BaseVertex;
    uint32_t BaseInstance;
};

struct MeshCreateInfo
{
    std::vector<Vertex> Vertices;
    std::vector<uint32_t> Indices;
    uint32_t TransformIndex;
    uint32_t BaseColorTexture;
    uint32_t NormalTexture;
    size_t VertexOffset;
    size_t IndexOffset;
    uint32_t VertexBuffer;
    uint32_t IndexBuffer;
};

struct Mesh
{
    uint32_t IndexCount = 0;
    int32_t VertexOffset = 0;
    uint32_t indexOffset = 0;
    // NOT OpenGL handles, just indices
    uint32_t TransformIndex = 0;
    uint32_t BaseColorTexture = 0;
    uint32_t NormalTexture = 0;
};

struct Model
{
    std::vector<Mesh> Meshes;
    std::vector<uint32_t> Textures;
    std::vector<glm::mat4> Transforms;
    uint32_t InputLayout;
    uint32_t VertexBuffer;
    uint32_t IndexBuffer;
    std::vector<uint32_t> Commands;
    std::vector<uint32_t> ObjectData;
    uint32_t TransformData;
};

class ProjectApplication final : public Application
{
protected:
    void AfterCreatedUiContext() override;
    void BeforeDestroyUiContext() override;
    bool Load() override;
    void prepareBuf();
    void RenderScene(float deltaTime) override;
    void RenderUI(float deltaTime) override;
    void Update(float deltaTime) override;


    void UpdateTransformations();

private:
    //Model _cubes;
    //uint32_t _shaderProgram;

    //Rendering::Shader g_shader;

    Rendering::Texture	 m_fretboard;

    //unsigned int VBO;
    //unsigned int VAO;

    //Rendering::VertexBatcher g_batcher;

    glm::mat4x4			m_highwayProjection;
    glm::mat4x4			m_highwayModelView;

    float _elapsedTime = 0.0f;



    glm::vec3 translationVec = glm::vec3(0, 1.81f, -1.94f);
    float rotationAngle = glm::radians(-60.0f);  // ����
    glm::vec3 rotationAxis = glm::vec3(1, 0, 0);



    bool MakeShader(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath);
    void LoadModel(std::string_view filePath);
};