#include "Application.h"
#include "GLFWCallbacks.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <sstream>

// This overwrites `LuaController.GetObject()`, please fix!
//#define MINIAUDIO_IMPLEMENTATION
//#include "miniaudio.h"

using namespace LuaInCPP;

Application::Application(int argc, char* argv[])
{
    windowInfo = WindowInfo(Resolution);
    windowInfo.SetWindowFramerate(60);

    shader = new ShaderProgram("Shaders\\_Default\\Shader.vert", "Shaders\\_Default\\Shader.frag");
    shader->UseShader();

    glfwSetWindowUserPointer(windowInfo.window, (void*)this);

    glfwSetWindowSizeCallback(windowInfo.window, ResizeCallback);
    glfwSetMouseButtonCallback(windowInfo.window, MouseButtonCallback);
    glfwSetScrollCallback(windowInfo.window, MouseWheelCallback);
    glfwSetKeyCallback(windowInfo.window, KeyPressCallback);

    float xScale, yScale;
    glfwGetWindowContentScale(windowInfo.window, &xScale, &yScale);
    float pixelDensityScale = sqrtf(xScale * yScale);

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(windowInfo.window, true);
    ImGui_ImplOpenGL3_Init();

    if (pixelDensityScale != -1.0f) ImGui::GetIO().FontGlobalScale = pixelDensityScale;

    // LUA FILES NOT LOADING PROPERLY, PLEASE FIX!
    LuaObject* testObj = LuaController.CreateObject();
    testObj->LoadScript("Scripts\\Test.lua", "OnLoad");

    std::cout << "\nCurrent Lua Count: " << LuaController.Count() << "\n";
    std::cout << "File Path: " << LuaController.GetObject(0)->GetFilePath() << "\n";
    std::cout << "Is Loaded: " << std::boolalpha << LuaController.GetObject(0)->IsLoaded() << "\n";

    camera.CameraType = CameraType::Free;
    camera.Position = vec3(0, 0, 0);

    //Light light;
    //light.direction = vec3(0, 1, 0);
    //light.colour = vec3(1, 1, 1);

    //lights.push_back(&light);

    //Mesh testMesh;
    ////testMesh.CreateFromFile("Meshes\\soulspear.obj");
    //testMesh.SetShape(ShapeType::Shape_Cube);
    //testMesh.CreateShape();

    //GameObject* obj = new GameObject();
    //obj->mesh = &testMesh;

    //obj->diffuseTexture = new Texture("Textures\\Soulspear\\soulspear_diffuse.tga");
    //obj->normalTexture = new Texture("Textures\\Soulspear\\soulspear_normal.tga");
    //obj->specularTexture = new Texture("Textures\\Soulspear\\soulspear_specular.tga");

    //objects.push_back(obj);

    Run();
}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(windowInfo.window);
    glfwTerminate();
}

bool Application::IsRunning() const
{
    return (glfwWindowShouldClose(windowInfo.window) == false && glfwGetKey(windowInfo.window, GLFW_KEY_ESCAPE) != GLFW_PRESS) && windowInfo.ContinueRunning == true;
}

void Application::Run()
{
    double fixedDelta = 1.0 / windowInfo.GetWindowFramerate();
    double currentTime = glfwGetTime();
    double accumulator = 0.0;

    while (IsRunning())
    {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= fixedDelta)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float deltaTime = (float)fixedDelta;
            accumulator -= fixedDelta;

            Update(deltaTime);

            ImDrawData* data = ImGui::GetDrawData();
            if (data) ImGui_ImplOpenGL3_RenderDrawData(data);
        }

        glfwSwapBuffers(windowInfo.window);
        glfwPollEvents();
    }
}

void Application::Update(float deltaTime)
{
    double xpos, ypos;
    glfwGetCursorPos(windowInfo.window, &xpos, &ypos);
    cursorPos = vec2(xpos, ypos);

    mat4 vpMatrix = camera.Draw(windowInfo.GetAspectRatio());
    shader->SetVec3Uniform("cameraPos", camera.Position);

    std::vector<vec3> lightDirections;
    std::vector<vec3> lightColours;

    for (Light* light : lights)
    {
        lightDirections.push_back(light->direction);
        lightColours.push_back(light->colour);
    }

    shader->SetVec3VArrayUniform("lightDirections", lightDirections);
    shader->SetVec3VArrayUniform("lightColours", lightColours);

    // Object Updating
    for (GameObject* object : objects) 
        object->Update(deltaTime);

    // Object Rendering
    for (GameObject* object : objects) 
        object->Draw(shader, vpMatrix, camera.Position);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DebugInterface(deltaTime);

    ImGui::Render();
}

void Application::OnMouseClick(int mouseButton)
{
    if (ImGuiWantsMouse) return;
    std::cout << "Clicked: " << mouseButton << "\n";
}

void Application::OnMouseRelease(int mouseButton)
{
    if (ImGuiWantsMouse) return;
    std::cout << "Released: " << mouseButton << "\n";
}

void Application::OnMouseScroll(bool positive)
{
    if (ImGuiWantsMouse) return;
    // SCROLLBACK GOES HERE!
}

void Application::OnKeyPress(Key key)
{
    if (ImGuiWantsMouse) return;
    std::cout << "Pressed: " << (int)key << "\n";
}

void Application::OnKeyRelease(Key key)
{
    if (ImGuiWantsMouse) return;
    std::cout << "Released: " << (int)key << "\n";
}

void Application::DebugInterface(float deltaTime)
{
    ImGuiWantsMouse = ImGui::GetIO().WantCaptureMouse;

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 50, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    ImGui::Begin("Typical Custom Engine - Debug Display");

    ImGui::Text("> Hello World! This is my 'Typical Custom Engine' programmed entirely within C++ and with OpenGL!");
    ImGui::Text("> This menu specifically exists just to display debug information and allow basic editing of objects, lighting and lua objects.");
    ImGui::Text("> A more sophisticated menu system will be implemented soon that will allow you to manage scenes and the data within them!");

    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Generic Information", ImGuiTreeNodeFlags_FramePadding))
    {
        std::stringstream str;
        str << "OpenGL Version: " << glfwGetVersionString();

        std::stringstream str2;
        str2 << "Lua Version: " << LUA_VERSION;

        std::stringstream str3;
        str3 << "Target Framerate: " << windowInfo.GetWindowFramerate();

        std::stringstream str4;
        str4 << "Delta Time: " << deltaTime;

        std::stringstream str5;
        str5 << "Elapsed Time: " << glfwGetTime();

        ImGui::BulletText(str.str().c_str());
        ImGui::BulletText(str2.str().c_str());
        ImGui::BulletText(str3.str().c_str());
        ImGui::BulletText(str4.str().c_str());
        ImGui::BulletText(str5.str().c_str());
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Game Objects", ImGuiTreeNodeFlags_FramePadding))
    {
        if (!objects.empty())
        {
            for (int i = 0; i < objects.size(); i++)
            {
                std::stringstream branchName;
                branchName << "Objects[" << std::to_string(i) << "]";

                if (ImGui::TreeNode(branchName.str().c_str()))
                {
                    if (ImGui::TreeNode("Information"))
                    {
                        ImGui::Text("Object:");

                        std::stringstream objectPosition;
                        objectPosition << "Position: (" << objects[i]->transform.position.x << ", " << objects[i]->transform.position.y << ", " << objects[i]->transform.position.z << ")";
                        ImGui::BulletText(objectPosition.str().c_str());

                        std::stringstream objectRotation;
                        objectRotation << "Eular Rotation: (" << objects[i]->transform.rotation.x << ", " << objects[i]->transform.rotation.y << ", " << objects[i]->transform.rotation.z << ")";
                        ImGui::BulletText(objectRotation.str().c_str());

                        std::stringstream objectScale;
                        objectScale << "Scale: (" << objects[i]->transform.size.x << ", " << objects[i]->transform.size.y << ", " << objects[i]->transform.size.z << ")";
                        ImGui::BulletText(objectScale.str().c_str());

                        ImGui::Spacing();

                        std::stringstream gloss;
                        gloss << "Glossiness: " << objects[i]->glossiness;
                        ImGui::BulletText(gloss.str().c_str());

                        std::stringstream light;
                        light << "Ambient Light: (" << objects[i]->ambientLight.x << ", " << objects[i]->ambientLight.y << ", " << objects[i]->ambientLight.z << ")";
                        ImGui::BulletText(light.str().c_str());

                        ImGui::Spacing();

                        ImGui::Text("Mesh:");

                        std::stringstream meshName;
                        meshName << "Name: " << objects[i]->mesh->GetFileName();
                        ImGui::BulletText(meshName.str().c_str());

                        ImGui::Spacing();

                        ImGui::Text("Textures:");

                        int nullCounter = 0;

                        if (objects[i]->diffuseTexture != nullptr)
                        {
                            nullCounter++;

                            if (ImGui::TreeNode("Diffuse"))
                            {
                                std::stringstream textureName;
                                textureName << "Name: " << objects[i]->diffuseTexture->GetFileName();

                                std::stringstream textureId;
                                textureId << "Texture ID: " << objects[i]->diffuseTexture->GetTextureID();

                                std::stringstream textureFilter;
                                textureFilter << "Filter Type: ";

                                switch (objects[i]->diffuseTexture->filterType)
                                {
                                case TextureFilteringType::Nearest: textureFilter << "Nearest"; break;
                                case TextureFilteringType::Bilinear: textureFilter << "Bilinear"; break;
                                }

                                std::stringstream textureColor;
                                textureColor << "Colour Mode: ";

                                switch (objects[i]->diffuseTexture->filterMode)
                                {
                                case TextureColorMode::RGB: textureColor << "RGB"; break;
                                case TextureColorMode::RGBA: textureColor << "RGBA"; break;
                                }

                                std::stringstream textureDepth;
                                textureDepth << "Depth: " << objects[i]->diffuseTexture->GetDepth();

                                std::stringstream textureWidth;
                                textureWidth << "Width: " << objects[i]->diffuseTexture->GetWidth() << "px";

                                std::stringstream textureHeight;
                                textureHeight << "Height: " << objects[i]->diffuseTexture->GetHeight() << "px";

                                ImGui::BulletText(textureName.str().c_str());
                                ImGui::BulletText(textureId.str().c_str());
                                ImGui::BulletText(textureFilter.str().c_str());
                                ImGui::BulletText(textureColor.str().c_str());
                                ImGui::BulletText(textureDepth.str().c_str());
                                ImGui::BulletText(textureWidth.str().c_str());
                                ImGui::BulletText(textureHeight.str().c_str());

                                ImGui::TreePop();
                            }

                            ImGui::Spacing();
                        }

                        if (objects[i]->normalTexture != nullptr)
                        {
                            nullCounter++;

                            if (ImGui::TreeNode("Normal"))
                            {
                                std::stringstream textureName;
                                textureName << "Name: " << objects[i]->normalTexture->GetFileName();

                                std::stringstream textureId;
                                textureId << "Texture ID: " << objects[i]->normalTexture->GetTextureID();

                                std::stringstream textureFilter;
                                textureFilter << "Filter Type: ";

                                switch (objects[i]->normalTexture->filterType)
                                {
                                case TextureFilteringType::Nearest: textureFilter << "Nearest"; break;
                                case TextureFilteringType::Bilinear: textureFilter << "Bilinear"; break;
                                }

                                std::stringstream textureColor;
                                textureColor << "Colour Mode: ";

                                switch (objects[i]->normalTexture->filterMode)
                                {
                                case TextureColorMode::RGB: textureColor << "RGB"; break;
                                case TextureColorMode::RGBA: textureColor << "RGBA"; break;
                                }

                                std::stringstream textureDepth;
                                textureDepth << "Depth: " << objects[i]->normalTexture->GetDepth();

                                std::stringstream textureWidth;
                                textureWidth << "Width: " << objects[i]->normalTexture->GetWidth() << "px";

                                std::stringstream textureHeight;
                                textureHeight << "Height: " << objects[i]->normalTexture->GetHeight() << "px";

                                ImGui::BulletText(textureName.str().c_str());
                                ImGui::BulletText(textureId.str().c_str());
                                ImGui::BulletText(textureFilter.str().c_str());
                                ImGui::BulletText(textureColor.str().c_str());
                                ImGui::BulletText(textureDepth.str().c_str());
                                ImGui::BulletText(textureWidth.str().c_str());
                                ImGui::BulletText(textureHeight.str().c_str());

                                ImGui::TreePop();
                            }

                            ImGui::Spacing();
                        }

                        if (objects[i]->specularTexture != nullptr)
                        {
                            nullCounter++;

                            if (ImGui::TreeNode("Specular"))
                            {
                                std::stringstream textureName;
                                textureName << "Name: " << objects[i]->specularTexture->GetFileName();

                                std::stringstream textureId;
                                textureId << "Texture ID: " << objects[i]->specularTexture->GetTextureID();

                                std::stringstream textureFilter;
                                textureFilter << "Filter Type: ";

                                switch (objects[i]->specularTexture->filterType)
                                {
                                case TextureFilteringType::Nearest: textureFilter << "Nearest"; break;
                                case TextureFilteringType::Bilinear: textureFilter << "Bilinear"; break;
                                }

                                std::stringstream textureColor;
                                textureColor << "Colour Mode: ";

                                switch (objects[i]->specularTexture->filterMode)
                                {
                                case TextureColorMode::RGB: textureColor << "RGB"; break;
                                case TextureColorMode::RGBA: textureColor << "RGBA"; break;
                                }

                                std::stringstream textureDepth;
                                textureDepth << "Depth: " << objects[i]->specularTexture->GetDepth();

                                std::stringstream textureWidth;
                                textureWidth << "Width: " << objects[i]->specularTexture->GetWidth() << "px";

                                std::stringstream textureHeight;
                                textureHeight << "Height: " << objects[i]->specularTexture->GetHeight() << "px";

                                ImGui::BulletText(textureName.str().c_str());
                                ImGui::BulletText(textureId.str().c_str());
                                ImGui::BulletText(textureFilter.str().c_str());
                                ImGui::BulletText(textureColor.str().c_str());
                                ImGui::BulletText(textureDepth.str().c_str());
                                ImGui::BulletText(textureWidth.str().c_str());
                                ImGui::BulletText(textureHeight.str().c_str());

                                ImGui::TreePop();
                            }
                        }

                        if (nullCounter == 0) ImGui::BulletText("There's no Texture information associated with this GameObject to display.");

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Options"))
                    {
                        if (ImGui::CollapsingHeader("Transformation", ImGuiTreeNodeFlags_Bullet))
                        {
                            float* data = Vector3ToFloat(objects[i]->transform.position);
                            ImGui::DragFloat3("Position", data, 0.1f * deltaTime);
                            objects[i]->transform.position = Float3ToVector3(data);

                            data = Vector3ToFloat(objects[i]->transform.rotation);
                            ImGui::DragFloat3("Rotation", data, 0.1f * deltaTime);
                            objects[i]->transform.rotation = Float3ToVector3(data);

                            data = Vector3ToFloat(objects[i]->transform.size);
                            ImGui::DragFloat3("Scale", data, 0.1f * deltaTime);
                            objects[i]->transform.size = Float3ToVector3(data);
                        }

                        ImGui::Spacing();

                        if (ImGui::CollapsingHeader("Miscellaneous", ImGuiTreeNodeFlags_Bullet))
                        {
                            ImGui::InputFloat("Glossiness", &objects[i]->glossiness);

                            float* data = Vector3ToFloat(objects[i]->ambientLight);
                            ImGui::InputFloat3("Ambient Light", data);
                            objects[i]->ambientLight = Float3ToVector3(data);
                        }

                        ImGui::Spacing();

                        if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_Bullet))
                        {
                            if (objects[i]->diffuseTexture != nullptr)
                            {
                                ImGui::BulletText("TODO: ADD CONTROLS HERE!");
                            }

                            if (objects[i]->normalTexture != nullptr)
                            {
                                ImGui::BulletText("TODO: ADD CONTROLS HERE!");
                            }

                            if (objects[i]->specularTexture != nullptr)
                            {
                                ImGui::BulletText("TODO: ADD CONTROLS HERE!");
                            }
                        }

                        ImGui::TreePop();
                    }

                    ImGui::TreePop();
                }
            }
        }

        else ImGui::TextWrapped("There are no Game Objects currently within the Scene! Please insert some to view debug information about them here!");
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Lights", ImGuiTreeNodeFlags_FramePadding))
    {
        if (!lights.empty())
        {
            for (int i = 0; i < lights.size(); i++)
            {
                std::stringstream branchName;
                branchName << "Lights[" << std::to_string(i) << "]";

                if (ImGui::TreeNode(branchName.str().c_str()))
                {
                    if (ImGui::TreeNode("Information"))
                    {
                        std::stringstream direction;
                        direction << "Direction: (" << lights[i]->direction.x << ", " << lights[i]->direction.y << ", " << lights[i]->direction.z << ")";
                        ImGui::BulletText(direction.str().c_str());

                        std::stringstream colour;
                        colour << "Colour: (" << lights[i]->colour.x << ", " << lights[i]->colour.y << ", " << lights[i]->colour.z << ")";
                        ImGui::BulletText(colour.str().c_str());

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Options"))
                    {
                        float* data = Vector3ToFloat(lights[i]->colour);
                        ImGui::ColorEdit3("Colour", data);
                        lights[i]->colour = Float3ToVector3(data);

                        data = Vector3ToFloat(lights[i]->direction);
                        ImGui::InputFloat3("Direction", data);
                        lights[i]->direction = Float3ToVector3(data);

                        ImGui::TreePop();
                    }

                    ImGui::TreePop();
                }
            }
        }
        
        else ImGui::TextWrapped("There are no Lights currently within the Scene! Please insert some to view debug information about them here!");
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Lua", ImGuiTreeNodeFlags_FramePadding))
    {
        // TODO: ADD CONTENTS HERE!
    }

    ImGui::End();
}