#include "AudioManager.hpp"
#ifdef _WIN32
#include <conio.h>
#endif
#include "InputManager.h"
#include "ColorCodes.hpp"

AudioManager::AudioManager() {
    result = FMOD_System_Create(&system, FMOD_VERSION);
    FMODErrorCheck(result);
    result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, nullptr);
    FMODErrorCheck(result);
}

AudioManager::~AudioManager() {
    FMOD_Sound_Release(main);
    for (const auto& pair : noteMap) {
        FMOD_Sound_Release(pair.second);
    }
    FMOD_System_Close(system);
    FMOD_System_Release(system);
}

void AudioManager::setMain(const char* filename, double volume) {
    result = FMOD_System_CreateSound(system, filename, FMOD_DEFAULT, nullptr, &main);
    FMODErrorCheck(result);
    FMOD_Channel_SetVolume(mainChannel, volume);
}

void AudioManager::addNote(const char* filename, char key) {
    FMOD_SOUND* note;
    result = FMOD_System_CreateSound(system, filename, FMOD_DEFAULT, nullptr, &note);
    FMODErrorCheck(result);
    noteMap[key] = note;
}

void AudioManager::setBpm(int b) {
    bpm = b;
    interval = 60000 / b;
}

void AudioManager::setOffFirst(int off) { offset_first_beat = off; }

void AudioManager::update() {
    FMOD_BOOL isMainPlaying;
    if (mainChannel != nullptr) {
        FMOD_Channel_IsPlaying(mainChannel, &isMainPlaying);
        // repeat main music track
        if (!isMainPlaying) {
            result = FMOD_System_PlaySound(system, main, nullptr, false, &mainChannel);
            FMODErrorCheck(result, "!isMainPlaying");
        }
    }
    FMOD_System_Update(system);
    result = FMOD_Channel_GetPosition(mainChannel, &position, FMOD_TIMEUNIT_MS);
    FMODErrorCheck(result, "FMOD_Channel_GetPosition");
    position = position - offset_first_beat;

    FMOD_SOUND* selectedSound = nullptr;
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

    int off = (position % interval < interval - (position % interval))
                   ? position % interval
                   : -(interval - (position % interval));
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
        FMOD_BOOL isPlaying;
        if (noteChannel != nullptr) {
            FMOD_Channel_IsPlaying(noteChannel, &isPlaying);
        } else {
            isPlaying = false;
        }

        // Important: If playing, stop before playing new sound
        if (isPlaying) {
            FMOD_Channel_Stop(noteChannel);
        }

        result = FMOD_System_PlaySound(system, selectedSound, nullptr, false, &noteChannel);
        FMODErrorCheck(result);
        result = FMOD_Channel_SetVolume(noteChannel, 0.2f);
        FMODErrorCheck(result);
    }
}

void AudioManager::play() {
    result = FMOD_System_PlaySound(system, main, nullptr, false, &mainChannel);
    FMODErrorCheck(result);
}
