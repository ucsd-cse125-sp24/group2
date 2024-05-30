#define STB_IMAGE_IMPLEMENTATION

#include "Window.h"
#include "Client.h"
#include "core.h"
#include "InputManager.h"

#include "GameManager.hpp"
#include <glm/gtx/string_cast.hpp>
#include "GameObject.hpp"
#include "Transform.hpp"
#include "ConcurrentQueue.hpp"
#include <thread>
#include "AudioManager.hpp"
#include "ColorCodes.hpp"
#include "AssetManager.hpp"
#include "components/RendererComponent.hpp"
#include "EntityBase.hpp"
#include "components/BeatSyncComponent.hpp"

ConcurrentQueue<std::function<void(void)>> task_queue;

void error_callback(int error, const char* description) {
    // Print error.
    std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window) {
    // Set the error callback.
    glfwSetErrorCallback(error_callback);
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window, Window::resizeCallback);

    // Set the key callback.
    // glfwSetKeyCallback(window, Window::keyCallback);
    InputManager::setUpCallback(window);
    // Set the mouse and cursor callbacks
    glfwSetMouseButtonCallback(window, Window::mouse_callback);
}

void setup_opengl_settings() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions() {
    // Get info of GPU and supported OpenGL version.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
              << std::endl;

    // If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: ./menv [IP] [PORT]\n");
        return 1;
    }

    // Create the GLFW window.
    float width = 800;
    float height = 600;
    GLFWwindow* window = Window::createWindow(width, height);
    if (!window)
        exit(EXIT_FAILURE);

    printf(YLW "Loading...\n" RST);
    GameManager::instance().Init();
    printf(YLW "Done loading!n" RST);

    // Setup camera
    GameManager::instance().cam = new Camera();
    GameManager::instance().cam->SetAspect(float(width) / float(height));

    // Print OpenGL and GLSL versions.
    print_versions();
    // Setup callbacks.
    setup_callbacks(window);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //  Setup OpenGL settings.
    setup_opengl_settings();

    // Initialize input
    InputManager::setDefaultKeys();

    // Initialize the shader program; exit if initialization fails.
    if (!Window::initializeProgram())
        exit(EXIT_FAILURE);

    GameManager::instance().client.setCallback([&](Packet* params) {
        task_queue.push_back(
            [&, params]() { GameManager::instance().handle_packet(params); });
    });
    GameManager::instance().client.connect(argv[1], atoi(argv[2]));

    AudioManager::instance().AddNote("../assets/audio/Fsharp.wav", 'i');
    AudioManager::instance().AddNote("../assets/audio/Gsharp.wav", 'j');
    AudioManager::instance().AddNote("../assets/audio/A.wav", 'k');
    AudioManager::instance().AddNote("../assets/audio/Csharp.wav", 'l');

    AudioManager::instance().AddPhase(
        "../assets/audio/futuristic02-116bpm-Gbm.wav");
    AudioManager::instance().SetBpm(232);

    std::cout << "Updating AssetManager" << std::endl;
    std::vector<std::string> modelPaths;
    modelPaths.push_back("../assets/male_basic_walk_30_frames_loop/scene.gltf");
    modelPaths.push_back("../assets/animation/model.gltf");
    modelPaths.push_back("../assets/robot/robot.gltf");
    for (std::string path : modelPaths) {
        std::cout << "  path: " << path << std::endl;
        Model* model = new Model(nullptr, path, true);
        AssetManager::Instance().AddMapping(path, model, {});

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);

        for (int i = 0; i < scene->mNumAnimations; ++i) {
            aiAnimation* animation = scene->mAnimations[i];
            AnimationClip* clip = new AnimationClip(animation, model, scene);
            // std::cout << "  Clip name: " << clip->getName() << std::endl;
            AssetManager::Instance().AddClipToMapping(path, clip);
        }
    }

    // ground
    EntityBase* go = new EntityBase();
    Model* model = new Model(go, "../assets/ground/plane.gltf", false);
    go->AddComponent(model);
    RendererComponent* renderer =
        new RendererComponent(go, ShaderType::STANDARD);
    go->AddComponent(renderer);
    GameManager::instance().scene.Instantiate(go);

    // bear
    EntityBase* bear = new EntityBase();
    Model* bearModel = new Model(bear, "../assets/Bear2/bear.gltf", true);
    bear->GetComponent<NetTransform>()->SetScale(glm::vec3(400.0f));
    bear->AddComponent(bearModel);
    AnimationPlayer* bearAnimationPlayer = new AnimationPlayer(bear, bearModel);
    bear->AddComponent(bearAnimationPlayer);
    std::vector<AnimationClip*> prefabClips =
        AssetManager::Instance().GetClips("../assets/Bear2/bear.gltf");
    for (int i = 0; i < prefabClips.size(); ++i) {
        AnimationClip* clip = new AnimationClip(prefabClips[i]);
        // std::cout << "Adding clip: " << clip->getName()
        //           << std::endl;
        bear->GetComponent<AnimationPlayer>()->AddClip(clip);
    }
    RendererComponent* bearRenderer =
        new RendererComponent(bear, ShaderType::ANIMATED);
    bear->AddComponent(bearRenderer);
    bear->GetComponent<AnimationPlayer>()->play("idle");
    GameManager::instance().scene.Instantiate(bear);
    std::cout << "  Finished updating AssetManager" << std::endl;

    GameObject* x = new GameObject();
    IComponent* beat = new BeatSyncComponent();
    x->AddComponent(beat);
    GameManager::instance().scene.Instantiate(x);

    // Loop while GLFW window should stay open.
    float deltaTime = 0;
    float timer = 0;
    float currentTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        // Calculate deltaTime
        float newTime = glfwGetTime();
        deltaTime = newTime - currentTime;
        currentTime = newTime;

        AudioManager::instance().Update();

        // send input
        timer += deltaTime;
        if (timer > 0.025f) {
            Packet* pkt = new Packet();
            pkt->write_int((int)PacketType::PLAYER_INPUT);
            uint8_t* buf = new uint8_t[5];
            buf[0] = InputManager::IsKeyDown(GLFW_KEY_W);
            buf[1] = InputManager::IsKeyDown(GLFW_KEY_A);
            buf[2] = InputManager::IsKeyDown(GLFW_KEY_S);
            buf[3] = InputManager::IsKeyDown(GLFW_KEY_D);
            buf[4] = InputManager::IsKeyDown(GLFW_KEY_LEFT_SHIFT) &&
                     (buf[0] || buf[1] || buf[2] || buf[3]);
            for (int i = 0; i < 5; i++) {
                pkt->write_byte(buf[i]);
            }
            GameManager::instance().client.send(pkt);
            delete[] buf;
            timer = 0;
        }

        // Main render display callback. Rendering of objects is done here.
        GameManager::instance().cam->Update();
        Window::Render(window, &GameManager::instance().scene,
                       GameManager::instance().cam, deltaTime);

        // Update loop
        while (!task_queue.empty()) {
            task_queue.pop_front()();
        }

        GameManager::instance().scene.Update(deltaTime);
    }

    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(window);
    // Terminate GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}