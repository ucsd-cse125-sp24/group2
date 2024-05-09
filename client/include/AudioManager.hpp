#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <fmod.hpp>
#include <fmod_errors.h>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <thread>

class AudioManager {

public:
    AudioManager();
    ~AudioManager();
    static AudioManager& instance() {
        static AudioManager a;
        return a;
    }
    void setMain(const char* filename, double volume);
    void addNote(const char* filename, char key);
    void setBpm(int b);
    void setOffFirst(int off);
    void play();
    void update();

private:
    FMOD::System* system;
    FMOD::Sound* main;
    FMOD::Channel* mainChannel = nullptr;
    FMOD::Channel* noteChannel = nullptr;
    FMOD_RESULT result;
    std::unordered_map<char, FMOD::Sound*> noteMap;

    int bpm;
    int interval;
    int offset_first_beat = 0;
    bool isRunning = true;

    void FMODErrorCheck(FMOD_RESULT result) {
        if (result != FMOD_OK) {
            std::cerr << "FMOD error " << result << ": "
                      << FMOD_ErrorString(result) << std::endl;
            // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
            exit(-1);
        }
    }
};

#endif // AUDIO_MANAGER_HPP