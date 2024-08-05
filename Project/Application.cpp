#include "Application.h"
#include <sstream>

// MOVE THESE INTO THEIR OWN FILES!!!

static void ResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void MessageCallback()
{

}

static void AngelScriptPrint()
{

}

Application::Application(int argc, char* argv[])
{
    windowInfo = WindowInfo(Resolution);

    shader = new ShaderProgram("shaders\\Shader.vert", "shaders\\Shader.frag");
    shader->UseShader();

    glGenBuffers(1, &vertexBufferID);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * someFloats.size(), someFloats.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glEnable(GL_DEPTH_TEST);
    glfwSetWindowSizeCallback(windowInfo.window, ResizeCallback);

    float xScale, yScale;
    glfwGetWindowContentScale(windowInfo.window, &xScale, &yScale);
    float pixelDensityScale = sqrtf(xScale * yScale);

    ui->Initalize(windowInfo.window, true, pixelDensityScale);

    //scriptEngine = asCreateScriptEngine();
    //int result = scriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL); 
    //assert(result >= 0);

    // run RegisterStdString(scriptEngine); here!

    //result = scriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(AngelScriptPrint), asCALL_CDECL);
    //assert(result >= 0);

    Run();
}

Application::~Application()
{
    delete ui;

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
            //DictateKeyboardInput();
            ui->Render();
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

	glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime(), { 0.0f, 1.0f, 0.0f });

	glm::mat4 projectionMatrix = glm::perspective(PI / 4.0f, windowInfo.GetAspectRatio(), 0.1f, 50.0f);

	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, someFloats.size() / 3);

	glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    shader->SetMat4Uniform("modelMatrix", &modelMatrix[0][0]);
    shader->SetMat4Uniform("viewMatrix", &viewMatrix[0][0]);
    shader->SetMat4Uniform("projectionMatrix", &projectionMatrix[0][0]);
	shader->SetMat4Uniform("mvpMatrix", &mvpMatrix[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, someFloats.size() / 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    ui->Update(this, deltaTime);
}

// REPLACE REGULAR IMGUI WITH DOCKING BRANCH VARAINT!
void Application::ManageUI(float deltaTime)
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 50, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    ImGui::Begin("Typical Custom Engine - Main");

    ImGui::Text("> Hello World! This is my 'Typical Custom Engine' programmed entirely within C++ and with OpenGL!");
    
    ImGui::Spacing();
    
    if (ImGui::CollapsingHeader("Generic Information", ImGuiTreeNodeFlags_FramePadding))
    {
        std::vector<std::stringstream> strs = {};

        std::stringstream str;
        str << "OpenGL Version: " << glfwGetVersionString();

        std::stringstream str2;
        str2 << "AngelScript Version: " << ANGELSCRIPT_VERSION_STRING;

        std::stringstream str3;
        str3 << "Target Framerate: " << windowInfo.GetWindowFramerate();

        std::stringstream str4;
        str4 << "DeltaTime: " << deltaTime;

        ImGui::BulletText(str.str().c_str());
        ImGui::BulletText(str2.str().c_str());
        ImGui::BulletText(str3.str().c_str());
        ImGui::BulletText(str4.str().c_str());
    }

    ImGui::End();
}