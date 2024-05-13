#include "AudioManager.hpp"
#include <chrono>
#include "InputManager.h"
#include "ColorCodes.hpp"
#include "GameManager.hpp"

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
    result =
        FMOD_System_CreateSound(system, filename, FMOD_DEFAULT, nullptr, &main);
    FMODErrorCheck(result);
    FMOD_Channel_SetVolume(mainChannel, volume);
}

void AudioManager::addNote(const char* filename, char key) {
    FMOD_SOUND* note;
    result =
        FMOD_System_CreateSound(system, filename, FMOD_DEFAULT, nullptr, &note);
    FMODErrorCheck(result);
    noteMap[key] = note;
}

void AudioManager::setBpm(int b) {
    bpm = b;
    interval = 60000 / b;
}

void AudioManager::setOffFirst(int off) { offset_first_beat = off; }

int occupied_beat = 0;
auto startTime = std::chrono::steady_clock::now();
void AudioManager::update() {
    FMOD_System_Update(system);
    auto msSinceStart = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - startTime)
                            .count();
    msSinceStart = msSinceStart - offset_first_beat;

    FMOD_SOUND* selectedSound = nullptr;
    Packet* pkt = new Packet();
    pkt->write_int((int)PacketType::PLAYER_ATTACK);
    if (InputManager::IsKeyPressed(GLFW_KEY_J)) {
        pkt->write_int(GLFW_KEY_J);
        selectedSound = noteMap.at('i');
    } else if (InputManager::IsKeyPressed(GLFW_KEY_K)) {
        pkt->write_int(GLFW_KEY_K);
        selectedSound = noteMap.at('j');
    } else if (InputManager::IsKeyPressed(GLFW_KEY_I)) {
        pkt->write_int(GLFW_KEY_I);
        selectedSound = noteMap.at('l');
    } else if (InputManager::IsKeyPressed(GLFW_KEY_L)) {
        pkt->write_int(GLFW_KEY_L);
        selectedSound = noteMap.at('k');
    } else {
        return;
    }

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
    pkt->write_int(off);

    if (selectedSound != nullptr) {
        GameManager::instance().client.send(pkt);
        // Check if the channel is currently playing
        FMOD_BOOL isPlaying;
        if (noteChannel != nullptr) {
            FMOD_Channel_IsPlaying(noteChannel, &isPlaying);
        } else {
            isPlaying = false;
        }

        // Important: If it is playing, stop it before playing the
        // new sound
        if (isPlaying) {
            FMOD_Channel_Stop(noteChannel);
        }

        result = FMOD_System_PlaySound(system, selectedSound, nullptr, false,
                                       &noteChannel);
        FMODErrorCheck(result);
        result = FMOD_Channel_SetVolume(noteChannel, 0.2f);
        FMODErrorCheck(result);
    }
}

void AudioManager::play() {
    int occupied_beat = 0; // used to exclude other inputs at the same beat
    result = FMOD_System_PlaySound(system, main, nullptr, false, &mainChannel);
    FMODErrorCheck(result);
    startTime = std::chrono::steady_clock::now();
}
