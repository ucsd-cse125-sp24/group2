#include "AudioManager.hpp"
#include <chrono>
#ifdef _WIN32
#include <conio.h>
#endif
#include "InputManager.h"
#include "ColorCodes.hpp"

AudioManager::AudioManager() {
    result = FMOD::System_Create(&system);
    FMODErrorCheck(result);
    result = system->init(32, FMOD_INIT_NORMAL, nullptr);
    FMODErrorCheck(result);
}

AudioManager::~AudioManager() {
    main->release();
    for (const auto& pair : noteMap) {
        pair.second->release();
    }
    system->close();
    system->release();
}

void AudioManager::setMain(const char* filename, double volume) {
    result = system->createSound(filename, FMOD_DEFAULT, nullptr, &main);
    FMODErrorCheck(result);
    mainChannel->setVolume(volume);
}

void AudioManager::addNote(const char* filename, char key) {
    FMOD::Sound* note;
    result = system->createSound(filename, FMOD_DEFAULT, nullptr, &note);
    FMODErrorCheck(result);
    noteMap[key] = note;
}

void AudioManager::setBpm(int b) {
    bpm = b;
    interval = 60000 / b;
}

void AudioManager::setOffFirst(int off) { offset_first_beat = off; }

int occupied_beat = 0;
bool pressed = false;
bool waspressed = false;
auto startTime = std::chrono::steady_clock::now();
void AudioManager::update() {
    system->update();
    auto msSinceStart = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - startTime)
                            .count();
    msSinceStart = msSinceStart - offset_first_beat;

    FMOD::Sound* selectedSound = nullptr;
    if (InputManager::isKeyPressed(GLFW_KEY_J)) {
        pressed = true;
        selectedSound = noteMap.at('i');
    } else if (InputManager::isKeyPressed(GLFW_KEY_K)) {
        pressed = true;
        selectedSound = noteMap.at('j');
    } else if (InputManager::isKeyPressed(GLFW_KEY_I)) {
        pressed = true;
        selectedSound = noteMap.at('l');
    } else if (InputManager::isKeyPressed(GLFW_KEY_L)) {
        pressed = true;
        selectedSound = noteMap.at('k');
    } else {
        pressed = false;
        waspressed = false;
    }

    if (!pressed || waspressed)
        return;

    waspressed = true;

    auto off = (msSinceStart % interval < interval - (msSinceStart % interval))
                   ? msSinceStart % interval
                   : -(interval - (msSinceStart % interval));
    // TODO: Calibration
    off -= 60;
    occupied_beat = (int)(msSinceStart - off) / interval;
    if (abs(off) <= 100) {
        if (abs(off) <= 25) {
            std::cout << "Perfect! ";
        } else if (abs(off) <= 50) {
            std::cout << "Great! ";
        } else {
            std::cout << "Good! ";
        }
        std::cout << std::to_string(off) << std::endl;
    } else {
        std::cout << "Off Beat! " << std::to_string(off) << std::endl;
    }

    if (selectedSound != nullptr) {
        // Check if the channel is currently playing
        bool isPlaying;
        if (noteChannel != nullptr) {
            noteChannel->isPlaying(&isPlaying);
        } else {
            isPlaying = false;
        }

        // Important: If it is playing, stop it before playing the
        // new sound
        if (isPlaying) {
            noteChannel->stop();
        }

        result = system->playSound(selectedSound, nullptr, false, &noteChannel);
        FMODErrorCheck(result);
        result = noteChannel->setVolume(0.2f);
        FMODErrorCheck(result);
    }
}

void AudioManager::play() {
    result = system->playSound(main, nullptr, false, &mainChannel);
    startTime = std::chrono::steady_clock::now();
    FMODErrorCheck(result);
    int occupied_beat = 0; // used to exclude other inputs at the same beat

    bool isMainPlaying;
    if (mainChannel != nullptr) {
        mainChannel->isPlaying(&isMainPlaying);
        // repeat main music track and keep the beats
        if (!isMainPlaying &&
            !(std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::steady_clock::now() - startTime)
                  .count() %
              interval)) {
            result = system->playSound(main, nullptr, false, &mainChannel);
            startTime = std::chrono::steady_clock::now();
            FMODErrorCheck(result);
        }
    }
}
