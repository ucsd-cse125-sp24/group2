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
#include <deque>
#include "Event.hpp"

#define SONG_SAMPLE_RATE 48000

class AudioManager {

public:
    AudioManager();
    ~AudioManager();
    static AudioManager& instance() {
        static AudioManager a;
        return a;
    }
    EventHandler<EventArgs> Beat;
    void AddNote(const char* filename, char key);
    void AddPhase(const char* filename);
    void SetBpm(int b);
    void SetOffFirst(int off);
    void Play();
    void Update();
    void GoToNextAudioPhase();
    static FMOD_RESULT F_CALLBACK
    ChannelControlCallback(FMOD_CHANNELCONTROL* channelcontrol,
                           FMOD_CHANNELCONTROL_TYPE controltype,
                           FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
                           void* commanddata1, void* commanddata2);
    void CheckPhase(int syncPoint);

private:
    FMOD_SYSTEM* system;
    FMOD_CHANNEL* mainChannel = nullptr;
    std::vector<FMOD_CHANNEL*> musicPhaseChannels;
    std::vector<FMOD_SOUND*> musicPhases;
    FMOD_CHANNEL* noteChannel = nullptr;
    FMOD_RESULT result;
    std::unordered_map<char, FMOD_SOUND*> noteMap;

    int currentPhase = 0;
    int nextPhase = 0;
    int bpm;
    int interval;
    int offset_first_beat = 0;
    bool isRunning = true;
    unsigned int position = 0;
    unsigned int lastPosition = 0;
    bool pressed = false;
    bool waspressed = false;
    bool game_started = false;

    void FMODErrorCheck(FMOD_RESULT result, std::string s = "") {
        if (result != FMOD_OK) {
            std::cerr << "FMOD error " << result << ": "
                      << FMOD_ErrorString(result) << "in: " << s << std::endl;
            exit(-1);
        }
    }
};

#endif // AUDIO_MANAGER_HPP