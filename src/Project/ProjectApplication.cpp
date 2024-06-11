#define CGLTF_IMPLEMENTATION
#include <cgltf.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Project/ProjectApplication.hpp>

#include <Project/render_manager.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

#include <spdlog/spdlog.h>

#include <unordered_map>
#include <filesystem>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
#include <queue>
#include <set>

static std::string Slurp(std::string_view path)
{
    std::ifstream file(path.data(), std::ios::ate);
    std::string result(file.tellg(), '\0');
    file.seekg(0);
    file.read((char*)result.data(), result.size());
    return result;
}

namespace fs = std::filesystem;

static std::string FindTexturePath(const fs::path& basePath, const cgltf_image* image)
{
    std::string texturePath;
    if (!image->uri)
    {
        auto newPath = basePath / image->name;
        if (!newPath.has_extension())
        {
            if (std::strcmp(image->mime_type, "image/png") == 0)
            {
                newPath.replace_extension("png");
            }
            else if (std::strcmp(image->mime_type, "image/jpg") == 0)
            {
                newPath.replace_extension("jpg");
            }
        }
        texturePath = newPath.generic_string();
    }
    else
    {
        texturePath = (basePath / image->uri).generic_string();
    }
    return texturePath;
}

void ProjectApplication::AfterCreatedUiContext()
{
}

void ProjectApplication::BeforeDestroyUiContext()
{
}

#define HIGHWAY_FOV 50.0f
bool ProjectApplication::Load()
{
    if (!Application::Load())
    {
        spdlog::error("App: Unable to load");
        return false;
    }
    prepareBuf();


    {
        IO_Image image;
        IO_LoadImage("D:\\work12\\CMake-Glfw-OpenGL-Template\\data\\default.png", &image);
        m_fretboard.Create(&image, 3);
        m_fretboard.UpdateTexture(&image);
        IO_FreeImageData(&image);
    }

    UpdateTransformations();

    //glm::mat4x4 position = glm::mat4x4(1);
 
    //position = glm::translate(position, glm::vec3(0, 1.81f, -1.94f));
    //glm::quat rotation = glm::angleAxis(glm::radians(-60.0f), glm::vec3(1, 0, 0));

 

    //m_highwayProjection = glm::perspective(glm::radians(HIGHWAY_FOV), 1.0f, 0.01f, 13.0f);
    //m_highwayModelView = glm::mat4_cast(rotation) * position;


    RenderManager::Initialize();



    return true;
}

void ProjectApplication::prepareBuf()
{

    //float vertices[] = {
    //    -0.5f, -0.5f, 0.0f, // left  
    //     0.5f, -0.5f, 0.0f, // right 
    //     0.0f,  0.5f, 0.0f  // top   
    //};

  
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);
    //// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    //glBindVertexArray(VAO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    //// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    //// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    //glBindVertexArray(0);
}

void ProjectApplication::Update(float deltaTime)
{
    if (IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        Close();
    }

    _elapsedTime += deltaTime;
}

void ProjectApplication::RenderScene([[maybe_unused]] float deltaTime)
{
    //const auto projection = glm::perspective(glm::radians(80.0f), 1920.0f / 1080.0f, 0.1f, 256.0f);
    //const auto view = glm::lookAt(
    //    glm::vec3(3 * std::cos(glfwGetTime() / 4), 2, -3 * std::sin(glfwGetTime() / 4)),
    //    glm::vec3(0, 0, 0),
    //    glm::vec3(0, 1, 0));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glm::mat4x4 screen = glm::mat4x4(1.0f);
    //screen = glm::translate(screen, glm::vec3(pos + (((instrument->GetEffectBounce().GetPosition() + instrument->GetEffectWobble().GetPosition()) * scale * 20.0f) * glm::vec2(1.0f/IO_Window_GetWidth(), 1.0f/IO_Window_GetHeight())), 0));
    screen = glm::scale(screen, glm::vec3(glm::vec2(0.45, 0.45), 1));

    Rendering::Shader* fretboard = RenderManager::GetShader(SHADER_TYPE_DEFAULT);

    //auto* fretboard = &g_shader;

    fretboard->Use();
    fretboard->SetTexture(0);
    fretboard->SetProjection(m_highwayProjection);
    fretboard->SetModelView(m_highwayModelView);
    fretboard->SetScreenPos(screen);
    fretboard->SetFogPos(1.70f);
    //player.GetFretboardTexture()->Bind();
    m_fretboard.Bind();



    RenderManager::GetBatcher()->Use();
    RenderManager::GetBatcher()->Clear();

    static float time = 0.0f;
    time += deltaTime;

    float noteSpeed = 5.0f;
    float fretboardOffset = time / 4.0f * noteSpeed;

    fretboardOffset = 0.f;


    Rendering::VertexBatcher * batcher = RenderManager::GetBatcher();

    batcher->AddVertex(-1, -2.0f, 0, 0, 1 - fretboardOffset, 1, 1, 1, 1);
    batcher->AddVertex(1, -2.0f, 0, 1, 1 - fretboardOffset, 1, 1, 1, 1);
    batcher->AddVertex(1, 12.7f, 0, 1, -2.675f - fretboardOffset, 1, 1, 1, 1);
    batcher->AddVertex(1, 12.7f, 0, 1, -2.675f - fretboardOffset, 1, 1, 1, 1);
    batcher->AddVertex(-1, 12.7f, 0, 0, -2.675f - fretboardOffset, 1, 1, 1, 1);
    batcher->AddVertex(-1, -2.0f, 0, 0, 1 - fretboardOffset, 1, 1, 1, 1);

    // Update and render
    batcher->Update();
    batcher->Draw();




    //glUseProgram(_shaderProgram);
    //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 3);



}

void ProjectApplication::UpdateTransformations()
{
    glm::mat4x4 position = glm::mat4x4(1);
    position = glm::translate(position, translationVec);  // 使用 ImGui 修改后的变量
    glm::quat rotation = glm::angleAxis(rotationAngle, rotationAxis);


    m_highwayProjection = glm::perspective(glm::radians(HIGHWAY_FOV), 1.0f, 0.01f, 13.0f);
    
    m_highwayModelView = glm::mat4_cast(rotation) * position;
}
 
/*


    //glm::mat4x4 position = glm::mat4x4(1);

    //position = glm::translate(position, glm::vec3(0, 1.81f, -1.94f));
    //glm::quat rotation = glm::angleAxis(glm::radians(-60.0f), glm::vec3(1, 0, 0));



    //m_highwayProjection = glm::perspective(glm::radians(HIGHWAY_FOV), 1.0f, 0.01f, 13.0f);
    //m_highwayModelView = glm::mat4_cast(rotation) * position;
*/

void ProjectApplication::RenderUI(float deltaTime)
{
    ImGui::Begin("Window");
    {
        //ImGui::TextUnformatted("Hello World!");
        ImGui::Text("Time in seconds since startup: %f", _elapsedTime);
        ImGui::Text("The delta time between frames: %f", deltaTime);



        // 添加控件来修改 translationVec
        if (ImGui::SliderFloat3("Translation", &translationVec.x, -10.0f, 10.0f)) {
            // 这里可以加入响应代码，如果需要的话
            UpdateTransformations();
        }

        // 添加控件来修改 rotationAngle 和 rotationAxis
        if (ImGui::SliderAngle("Rotation Angle", &rotationAngle)) {
            // 这里可以加入响应代码，如果需要的话
            UpdateTransformations();
        }


        ImGui::End();
    }

    //ImGui::ShowDemoWindow();
}

//bool ProjectApplication::MakeShader(std::string_view vertexShaderFilePath, std::string_view fragmentShaderFilePath)
//{
//
//
//    g_shader.Use();
//    g_shader.SetModelView(glm::mat4x4(1.0f));
//    g_shader.SetScreenPos(glm::mat4x4(1.0f));
//    g_shader.SetCameraTransform(glm::mat4x4(1.0f));
//
//
//
//
//
//    return true;
//}



void ProjectApplication::LoadModel(std::string_view file)
{
  
}
