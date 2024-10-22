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

    Light light;
    light.direction = vec3(0, 1, 0);
    light.colour = vec3(1, 1, 1);

    lights.push_back(&light);

    Mesh testMesh;
    //testMesh.CreateFromFile("Meshes\\soulspear.obj");
    testMesh.SetShape(ShapeType::Shape_Cube);
    testMesh.CreateShape();

    GameObject* obj = new GameObject();
    obj->mesh = &testMesh;

    obj->diffuseTexture = new Texture("Textures\\Soulspear\\soulspear_diffuse.tga");
    obj->normalTexture = new Texture("Textures\\Soulspear\\soulspear_normal.tga");
    obj->specularTexture = new Texture("Textures\\Soulspear\\soulspear_specular.tga");

    objects.push_back(obj);

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