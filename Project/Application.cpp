/*
    General TODO's:
        - Fix LuaInCPP Library not currently assigning memory consistently
        - Finally implement MINIAUDIO as seen below (rename `LuaController.GetObject()` to get it to work)
        - Update `WindowInfo` to have Framerates, Resolutions, etc be easier to tinker with during Runtime
        - Update `File Configuration` Steps within `WindowInfo` to properly work without crashing GLFW when attempting to Initialize
        - Move ImGui Implementation and it's Header Definitions out of this file and into it's own UI File(s)
        - Replace ImGui Main branch variant with it's Docking branch counterpart!
        - Start developing the Engine Main Menu Interface as well as the rest of the UI that's meant to be used (I.e: When you're actually in the scene)
        - Write Scene Saving and Loading System
        - Create Load, Delete and New Scene Systems
        - Add more depth into Lighting and GameObject Systems
        - Actually introduce Namespaces into the Engine so as to simplify and unify code-design later on
        - Move GLFW configuration and Initialization into it's own file(s)
        - Use `EntryPoint.cpp` for more than just a simple pointer to the `Application` class!

    Research TODO's:
        - Look into how the following engines work around problems and how they introduce solutions (I.e; UI, Design, etc)
            > Godot Engine
            > Unity Engine
            > Unreal Engine
            > Roblox Studio
*/

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
    windowInfo = new WindowInfo(Resolution);
    GLFWwindow* window = windowInfo->window;

    shader = new ShaderProgram("Shaders\\_Default\\Shader.vert", "Shaders\\_Default\\Shader.frag");
    shader->UseShader();

    glfwSetWindowUserPointer(window, (void*)this);

    glfwSetWindowSizeCallback(window, ResizeCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, MouseWheelCallback);
    glfwSetKeyCallback(window, KeyPressCallback);

    float xScale, yScale;
    glfwGetWindowContentScale(window, &xScale, &yScale);
    float pixelDensityScale = sqrtf(xScale * yScale);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    if (pixelDensityScale != -1.0f) io.FontGlobalScale = pixelDensityScale;

    for (int i = (int)Key::Space; i < (int)Key::Invalid - 1; i++) keyCache.insert({ (int)(Key)i, false });

    Run();
}

Application::~Application()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool Application::IsRunning() const
{
    return (glfwWindowShouldClose(windowInfo->window) == false && glfwGetKey(windowInfo->window, GLFW_KEY_ESCAPE) != GLFW_PRESS) && windowInfo->ContinueRunning == true;
}

void Application::Run()
{
    double fixedDelta = 1.0 / windowInfo->GetWindowFramerate();
    double currentTime = glfwGetTime();
    double accumulator = 0.0;

    while (IsRunning())
    {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;

        currentTime = newTime;
        accumulator += frameTime;
        
        elapsedTime = (float)glfwGetTime();

        while (accumulator >= fixedDelta)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            deltaTime = (float)fixedDelta;
            accumulator -= fixedDelta;

            Update();

            ImDrawData* data = ImGui::GetDrawData();
            if (data) ImGui_ImplOpenGL3_RenderDrawData(data);
        }

        glfwSwapBuffers(windowInfo->window);
        glfwPollEvents();
    }
}

void Application::Update()
{
    double xpos, ypos;
    glfwGetCursorPos(windowInfo->window, &xpos, &ypos);
    cursorPos = vec2(xpos, ypos);

    for (auto& iterator : keyCache)
    {
        if (!iterator.second || ImGuiWantsMouse) continue;

        switch ((Key)iterator.first)
        {
            case Key::W: camera.Position += (camera.Speed * camera.GetLookVector(Front)) * deltaTime; break;
            case Key::S: camera.Position -= (camera.Speed * camera.GetLookVector(Front)) * deltaTime; break;
            case Key::A: camera.Position -= (glm::normalize(glm::cross(camera.GetLookVector(Front), camera.GetLookVector(Up))) * camera.Speed) * deltaTime; break;
            case Key::D: camera.Position += (glm::normalize(glm::cross(camera.GetLookVector(Front), camera.GetLookVector(Up))) * camera.Speed) * deltaTime; break;

            case Key::Q: camera.Position.y -= (camera.Speed * deltaTime); break;
            case Key::E: camera.Position.y += (camera.Speed * deltaTime); break;
        }
    }

    Render();
}

void Application::Render()
{
    mat4 vpMatrix = camera.Draw(windowInfo->GetAspectRatio());
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

    Debug();
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

void Application::OnMouseScroll(double xDelta, double yDelta)
{
    if (ImGuiWantsMouse) return;
    std::cout << "X Delta: " << xDelta << ", Y Delta: " << yDelta << "\n";
}

void Application::OnKeyPress(Key key)
{
    if (ImGuiWantsMouse) return;
    
    std::cout << "Pressed: " << (int)key << "\n";
    
    if (keyCache.find((int)key) != keyCache.end())
        keyCache[(int)key] = true;
    
    if (key == Key::LeftShift)
        camera.Speed = camera.HalfSpeed;
}

void Application::OnKeyRelease(Key key)
{
    if (ImGuiWantsMouse) return;

    std::cout << "Released: " << (int)key << "\n";
    
    if (keyCache.find((int)key) != keyCache.end())
        keyCache[(int)key] = false;

    if (key == Key::LeftShift)
        camera.Speed = camera.FullSpeed;
}

void Application::Debug()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    ImGuiWantsMouse = io.WantCaptureMouse;

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
   
    //ImGui::PopStyleVar(3);

    ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();

    ImGui::Begin("Typical Custom Engine - Debug Display", NULL, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene"))
            {
                std::cout << "New Scene | Currently not Implemented!\n";
            }

            if (ImGui::MenuItem("Delete Scene"))
            {
                std::cout << "Delete Scene | Currently not Implemented!\n";
            }

            if (ImGui::MenuItem("Load Scene"))
            {
                std::cout << "Load Scene | Currently not Implemented!\n";
            }

            if (ImGui::MenuItem("Quit (ESC)")) windowInfo->ContinueRunning = false;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo (Ctrl + Z)"))
            {
                if (!isSceneLoaded) std::cout << "There is no Scene loaded to invoke this action with!\n";
            }

            if (ImGui::MenuItem("Redo (Ctrl + Shift + Z)"))
            {
                if (!isSceneLoaded) std::cout << "There is no Scene loaded to invoke this action with!\n";
            }

            if (ImGui::MenuItem("Preferences"))
            {
                std::cout << "Preferences Selected > Open Preferences Window when programmed here!\n";
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About"))
            {
                std::cout << "Typical Custom Engine | Version: " << TCE_VERSION << " | Developed by: " << TCE_AUTHOR;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Text("> Hello World! This is my 'Typical Custom Engine' programmed entirely within C++ and with OpenGL!");
    ImGui::Text("> This menu specifically exists just to display debug information and allow basic editing of objects, lighting and lua objects.");
    ImGui::Text("> A more sophisticated menu system will be implemented soon that will allow you to manage scenes and the data within them!");

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Generic Information", ImGuiTreeNodeFlags_FramePadding))
    {
        std::stringstream str;
        str << "OpenGL Version: " << glfwGetVersionString();

        std::stringstream str2;
        str2 << "Lua Version: " << LUA_VERSION;

        std::stringstream str3;
        str3 << "Target Framerate: " << windowInfo->GetWindowFramerate();

        std::stringstream str4;
        str4 << "Delta Time: " << deltaTime;

        std::stringstream str5;
        str5 << "Elapsed Time: " << elapsedTime;

        ImGui::BulletText(str.str().c_str());
        ImGui::BulletText(str2.str().c_str());
        ImGui::BulletText(str3.str().c_str());
        ImGui::BulletText(str4.str().c_str());
        ImGui::BulletText(str5.str().c_str());
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Window Information", ImGuiTreeNodeFlags_FramePadding))
    {
        if (ImGui::TreeNode("Information"))
        {
            std::stringstream str1;
            str1 << "Title: " << windowInfo->GetWindowTitle();

            std::stringstream str2;
            str2 << "Background Color: (" << windowInfo->GetWindowColour()[0] << ", " << windowInfo->GetWindowColour()[1] << ", " << windowInfo->GetWindowColour()[2] << ")";

            std::stringstream str3;
            str3 << "Aspect Ratio: " << windowInfo->GetAspectRatio();

            std::stringstream str4;
            str4 << "Target Framerate: " << windowInfo->GetWindowFramerate();

            auto res = windowInfo->GetWindowResolution();

            std::stringstream str5;
            str5 << "(Starting) Window Resolution: " << res.first << "x" << res.second;

            int width, height;
            glfwGetWindowSize(windowInfo->window, &width, &height);

            std::stringstream str6;
            str6 << "(Current) Window Resolution: " << width << "x" << height;

            ImGui::BulletText(str1.str().c_str());
            ImGui::BulletText(str2.str().c_str());
            ImGui::BulletText(str3.str().c_str());
            ImGui::BulletText(str4.str().c_str());
            ImGui::BulletText(str5.str().c_str());
            ImGui::BulletText(str6.str().c_str());

            ImGui::TreePop();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::TreeNode("Controls"))
        {
            ImGui::TextWrapped("Insert Controls for GLFW Window Here!");
            ImGui::TreePop();
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_FramePadding))
    {
        std::stringstream str1;
        str1 << "Speed: " << camera.Speed;

        std::stringstream str2;
        str2 << "Field of View: " << camera.FOV;

        std::string camType;

        switch (camera.CameraType)
        {
            case CameraType::Free: camType = "Free"; break;
            case CameraType::Orbit: camType = "Orbit"; break;
            default: camType = "???"; break;
        }

        std::stringstream str3;
        str3 << "Type: " << camType;

        std::stringstream str4;
        str4 << "Position: (" << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << ")";

        std::stringstream str5;
        str5 << "Rotation: (" << camera.Direction.x << ", " << camera.Direction.y << ", " << camera.Direction.z << ")";

        ImGui::BulletText(str1.str().c_str());
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

    ImGui::Render();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}