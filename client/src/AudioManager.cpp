#include "AudioManager.hpp"
#include "InputManager.h"
#include "ColorCodes.hpp"
#include "GameManager.hpp"
#include <functional>

AudioManager::AudioManager() {
    result = FMOD_System_Create(&system, FMOD_VERSION);
    FMODErrorCheck(result);
    result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, nullptr);
    FMODErrorCheck(result);
}

AudioManager::~AudioManager() {
    for (const auto& pair : noteMap) {
        FMOD_Sound_Release(pair.second);
    }
    FMOD_System_Close(system);
    FMOD_System_Release(system);
}

void AudioManager::AddPhase(const char* filename) {
    FMOD_SOUND* music;
    result = FMOD_System_CreateSound(system, filename, FMOD_LOOP_OFF, nullptr,
                                     &music);
    FMODErrorCheck(result);
    musicPhases.push_back(music);
    FMOD_CHANNEL* channel;
    musicPhaseChannels.push_back(channel);
}

void AudioManager::GoToNextAudioPhase() { nextPhase++; }

void AudioManager::AddNote(const char* filename, char key) {
    FMOD_SOUND* note;
    result =
        FMOD_System_CreateSound(system, filename, FMOD_DEFAULT, nullptr, &note);
    FMODErrorCheck(result);
    noteMap[key] = note;
}

void AudioManager::SetBpm(int b) {
    bpm = b;
    interval = (float)60000 / b;
}

void AudioManager::SetOffFirst(int off) { offset_first_beat = off; }

void AudioManager::Update() {
    if (!game_started) {
        return;
    }

    FMOD_System_Update(system);

    if (currentPhase == 2) {
        nextPhase = 3;
    }

    result = FMOD_Channel_GetPosition(musicPhaseChannels[0], &position,
                                      FMOD_TIMEUNIT_MS);
    if (result != FMOD_OK) {
        return;
    }
    // std::cout<<"position: "<< position << std::endl;
    // position = position - offset_first_beat;

    if (position - lastPosition >= interval) {
        EventArgs* e = new BeatEventArgs(position / interval);
        Beat.invoke(e);
        lastPosition = position;
    }

    FMOD_SOUND* selectedSound = nullptr;
    Packet* pkt = new Packet();
    pkt->write_int((int)PacketType::PLAYER_ATTACK);
    if (InputManager::IsKeyPressed(GLFW_KEY_J)) {
        pressed = true;
        pkt->write_int(GLFW_KEY_J);
        selectedSound = noteMap.at('j');
    } else if (InputManager::IsKeyPressed(GLFW_KEY_K)) {
        pressed = true;
        pkt->write_int(GLFW_KEY_K);
        selectedSound = noteMap.at('k');
    } else if (InputManager::IsKeyPressed(GLFW_KEY_I)) {
        pressed = true;
        pkt->write_int(GLFW_KEY_I);
        selectedSound = noteMap.at('i');
    } else if (InputManager::IsKeyPressed(GLFW_KEY_L)) {
        pressed = true;
        pkt->write_int(GLFW_KEY_L);
        selectedSound = noteMap.at('l');
    } else {
        pressed = false;
        return;
    }

    float normalizedOff = (position / interval - floor(position / interval));
    if (normalizedOff > 0.5) {
        normalizedOff -= 1;
    }

    // int off = (position % interval < interval / 2)
    //               ? position % interval
    //               : -(interval - (position % interval));
    if (abs(normalizedOff) <= 0.3f) {
        if (abs(normalizedOff) <= 0.1f) {
            text = "perfect";
            std::cout << "Perfect! ";
        } else if (abs(normalizedOff) <= 0.2f) {
            text = "great";
            std::cout << "Great! ";
        } else {
            text = "good";
            std::cout << "Good! ";
        }
        std::cout << std::to_string(normalizedOff) << std::endl;
    } else {
        text = "offbeat";
        std::cout << "Off Beat! " << std::to_string(normalizedOff) << std::endl;
    }
    pkt->write_float(normalizedOff);

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

void AudioManager::Play() {
    FMOD_Channel_SetVolume(musicPhaseChannels[0], 1.0f);
    result = FMOD_System_PlaySound(system, musicPhases[0], nullptr, false,
                                   &musicPhaseChannels[0]);
    FMODErrorCheck(result);
    result = FMOD_Channel_SetMode(musicPhaseChannels[0], FMOD_LOOP_NORMAL);
    FMODErrorCheck(result);

    FMOD_RESULT result = FMOD_Channel_SetCallback(
        musicPhaseChannels[0], AudioManager::ChannelControlCallback);
    FMODErrorCheck(result);

    game_started = true;
}

int AudioManager::getBpm() const { return bpm; }

bool AudioManager::isStarted() const { return game_started; }

unsigned int AudioManager::getPosition() const { return position; }

bool x = false;
void AudioManager::CheckPhase(int syncPoint) {
    if (syncPoint != (currentPhase + 1) % 7) {
        return;
    }

    if (currentPhase == 2 && !x) {
        x = true;
        return;
    }

    FMOD_SYNCPOINT* sp;
    unsigned int nextOffset;
    if (nextPhase > currentPhase) {
        // go to next phase
        FMOD_Sound_GetSyncPoint(musicPhases[0], currentPhase + 1, &sp);
    } else {
        FMOD_Sound_GetSyncPoint(musicPhases[0], currentPhase, &sp);
    }

    FMOD_Sound_GetSyncPointInfo(musicPhases[0], sp, nullptr, 0, &nextOffset,
                                FMOD_TIMEUNIT_PCM);
    position = nextOffset;
    lastPosition = position;
    FMOD_Channel_SetPosition(musicPhaseChannels[0], nextOffset,
                             FMOD_TIMEUNIT_PCM);

    if (nextPhase > currentPhase)
        currentPhase++;
}

FMOD_RESULT F_CALLBACK AudioManager::ChannelControlCallback(
    FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype,
    FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1,
    void* commanddata2) {
    if (callbacktype == FMOD_CHANNELCONTROL_CALLBACK_END) {
        printf("Channel ended\n");
    } else if (callbacktype == FMOD_CHANNELCONTROL_CALLBACK_SYNCPOINT) {
        int point = (intptr_t)commanddata1;
        AudioManager::instance().CheckPhase(point);
    }

    return FMOD_OK;
}

std::string AudioManager::GetText() const { return text; }