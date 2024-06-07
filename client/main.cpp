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
#include "SkyBox.hpp"

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
    float width = 1366;
    float height = 768;

    // set up camera
    GameManager::instance().cam = new Camera();
    GameManager::instance().cam->SetAspect(float(width) / float(height));
    GLFWwindow* window = Window::createWindow(width, height);
    if (!window)
        exit(EXIT_FAILURE);

    printf(YLW "Loading...\n" RST);
    GameManager::instance().Init();
    printf(YLW "Done loading!n" RST);

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

    AudioManager::instance().AddNote("../assets/audio/Fsharp.wav", 'j');
    AudioManager::instance().AddNote("../assets/audio/Gsharp.wav", 'k');
    AudioManager::instance().AddNote("../assets/audio/A.wav", 'l');
    AudioManager::instance().AddNote("../assets/audio/Csharp.wav", 'i');

    AudioManager::instance().AddNote("../assets/audio/NewAudio/newFs.wav", 'J');
    AudioManager::instance().AddNote("../assets/audio/NewAudio/newGs.wav", 'K');
    AudioManager::instance().AddNote("../assets/audio/NewAudio/newA.wav", 'L');
    AudioManager::instance().AddNote("../assets/audio/NewAudio/newCs.wav", 'I');

    AudioManager::instance().AddHit("../assets/audio/playertakedamage.wav");

    AudioManager::instance().AddPhase(
        "../assets/audio/futuristic02-116bpm-Gbm.wav");
    AudioManager::instance().SetBpm(232);

    std::cout << "Updating AssetManager" << std::endl;
    std::vector<std::pair<std::string, bool>> modelPaths;
    modelPaths.push_back(
        {"../assets/male_basic_walk_30_frames_loop/scene.gltf", true});
    modelPaths.push_back({"../assets/animation/model.gltf", true});
    modelPaths.push_back({"../assets/robot/robot.gltf", true});
    modelPaths.push_back({"../assets/sphere/sphere.gltf", false});
    modelPaths.push_back({"../assets/ground/plane.gltf", false});
    modelPaths.push_back({"../assets/floor/floor.gltf", false});
    // modelPaths.push_back({"../assets/Bear2/bear.gltf", true});
    modelPaths.push_back({"../assets/bear_v5/bear.gltf", true});
    modelPaths.push_back(
        {"../assets/laser-attack/laser-beam-model.gltf", true});
    modelPaths.push_back(
        {"../assets/projectile-attack/projectile-attack-bone.gltf", true});
    modelPaths.push_back({"../assets/wave-attack-v3/WaveAttack.gltf", true});
    modelPaths.push_back({"../assets/swipe-attack/swipe-attack.gltf", false});
    modelPaths.push_back(
        {"../assets/player-note-attack/player-note-attack.gltf", false});
    modelPaths.push_back({"../assets/arena/arena.gltf", false});

    for (std::pair<string, bool> kv : modelPaths) {
        std::cout << "  path: " << kv.first << std::endl;
        Model* model = new Model(nullptr, kv.first, kv.second);
        AssetManager::Instance().AddMapping(kv.first, model, {});

        Assimp::Importer importer;
        const aiScene* scene =
            importer.ReadFile(kv.first, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);

        for (int i = 0; i < scene->mNumAnimations; ++i) {
            aiAnimation* animation = scene->mAnimations[i];
            AnimationClip* clip = new AnimationClip(animation, model, scene);
            // std::cout << "  Clip name: " << clip->getName() << std::endl;
            AssetManager::Instance().AddClipToMapping(kv.first, clip);
        }
    }

    // ground
    GameObject* go = new GameObject();
    Model* model = new Model(
        AssetManager::Instance().GetModel("../assets/floor/floor.gltf"));
    go->AddComponent(model);
    RendererComponent* renderer =
        new RendererComponent(go, ShaderType::STANDARD);
    go->AddComponent(renderer);
    GameManager::instance().scene.Instantiate(go);
    go->GetComponent<Transform>()->SetScale(glm::vec3(0.0f));

    // arena
    GameObject* arena = new GameObject();
    Model* arenaModel = new Model(
        AssetManager::Instance().GetModel("../assets/arena/arena.gltf"));
    arena->AddComponent(arenaModel);
    RendererComponent* arenaRender =
        new RendererComponent(arena, ShaderType::STANDARD);
    arena->AddComponent(arenaRender);
    arena->GetComponent<Transform>()->SetScale(glm::vec3(2.0f));
    arena->GetComponent<Transform>()->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    GameManager::instance().scene.Instantiate(arena);

    for (int i = 0; i < 20; i++) {
        GameObject* sphereObject = new GameObject();
        Model* sphereModel = new Model(
            AssetManager::Instance().GetModel("../assets/sphere/sphere.gltf"));
        sphereObject->AddComponent(sphereModel);
        auto sphereRenderer =
            new RendererComponent(sphereObject, ShaderType::STANDARD);
        sphereObject->AddComponent(sphereRenderer);
        IComponent* beat = new BeatSyncComponent();
        sphereObject->AddComponent(beat);

        sphereObject->GetComponent<Transform>()->SetPosition(
            glm::vec3(glm::cos(2 * i * glm::pi<float>() / 20) * 2000, 500,
                      glm::sin(2 * i * glm::pi<float>() / 20) * 2000));
        GameManager::instance().scene.Instantiate(sphereObject);
    }

    GameManager::instance().client.connect(argv[1], atoi(argv[2]));

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
        GameManager::instance().cam->Update(deltaTime);
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
