#include "AudioManager.hpp"
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

void AudioManager::update() {
    if (!game_started) {
        return;
    }
    FMOD_BOOL isMainPlaying;
    FMOD_Channel_IsPlaying(mainChannel, &isMainPlaying);
    // repeat main music track
    if (!isMainPlaying) {
        result = FMOD_System_PlaySound(system, main, nullptr, false, &mainChannel);
        FMODErrorCheck(result, "!isMainPlaying");
    }

    FMOD_System_Update(system);

    result = FMOD_Channel_GetPosition(mainChannel, &position, FMOD_TIMEUNIT_MS);
    if (result != FMOD_OK) {
        return;
    }
    position = position - offset_first_beat;

    FMOD_SOUND* selectedSound = nullptr;
    Packet* pkt = new Packet();
    pkt->write_int((int)PacketType::PLAYER_ATTACK);
    if (InputManager::IsKeyPressed(GLFW_KEY_J)) {
        pkt->write_int(GLFW_KEY_J);
        selectedSound = noteMap.at('j');
    } else if (InputManager::IsKeyPressed(GLFW_KEY_K)) {
        pkt->write_int(GLFW_KEY_K);
        selectedSound = noteMap.at('k');
    } else if (InputManager::IsKeyPressed(GLFW_KEY_I)) {
        pkt->write_int(GLFW_KEY_I);
        selectedSound = noteMap.at('i');
    } else if (InputManager::IsKeyPressed(GLFW_KEY_L)) {
        pkt->write_int(GLFW_KEY_L);
        selectedSound = noteMap.at('l');
    } else {
        return;
    }

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

        // Important: If playing, stop before playing new sound
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
    result = FMOD_System_PlaySound(system, main, nullptr, false, &mainChannel);
    FMODErrorCheck(result);
    game_started = true;
}
