#include "AudioManager.hpp"
#include <chrono>
#include <conio.h>


AudioManger::AudioManger() {
    result = FMOD::System_Create(&system);
    FMODErrorCheck(result);
    result = system->init(32, FMOD_INIT_NORMAL, nullptr);
    FMODErrorCheck(result);
}

AudioManger::~AudioManger() {
    main->release();
    for (const auto& pair : noteMap) {
        pair.second->release();
    }
    system->close();
    system->release();
}

void AudioManger::setMain(const char* filename, double volume) {
    result = system->createSound(filename, FMOD_DEFAULT, nullptr, &main);
    FMODErrorCheck(result);
    mainChannel->setVolume(volume);
}

void AudioManger::addNote(const char* filename, char key) {
    FMOD::Sound* note;
    result = system->createSound(filename, FMOD_DEFAULT, nullptr, &note);
    FMODErrorCheck(result);
    noteMap[key] = note;
}

void AudioManger::setBpm(int b) {
    bpm = b;
    interval = 60000/b;
}

void AudioManger::setOffFirst(int off) {
    offset_first_beat = off;
}

void AudioManger::play() {
    result = system->playSound(main, nullptr, false, &mainChannel);
    auto startTime = std::chrono::steady_clock::now();
    FMODErrorCheck(result);
    int occupied_beat = 0; // used to exclude other inputs at the same beat
    
    while (isRunning) {
        system->update();

        if (_kbhit()) {
            int input = _getch();
            auto msSinceStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
            msSinceStart = msSinceStart - offset_first_beat;
            auto off = (msSinceStart % interval < interval - (msSinceStart % interval)) ? msSinceStart % interval : -(interval - (msSinceStart % interval));
            if (occupied_beat == (msSinceStart - off) / interval)
                continue;
            occupied_beat = (int)(msSinceStart - off) / interval;
            FMOD::Sound* selectedSound = nullptr;

            if (abs(off) <= 200) {
                if (abs(off) <= 75) {std::cout << "Perfect! ";}
                else if (abs(off) <= 150) {std::cout << "Great! ";}
                else {std::cout << "Good! ";}
                std::cout << std::to_string(off) << std::endl;
                switch (input) {
                    case 'j':
                        selectedSound = noteMap.at('j');
                        break;
                    case 'k':
                        selectedSound = noteMap.at('k');
                        break;
                    case 'i':
                        selectedSound = noteMap.at('i');
                        break;
                    case 'l':
                        selectedSound = noteMap.at('l');
                        break;
                    case 'q':
                        isRunning = false;
                }
                
                if (selectedSound != nullptr) {
                    // Check if the channel is currently playing
                    bool isPlaying;
                    if (noteChannel != nullptr) {
                        noteChannel->isPlaying(&isPlaying);
                    } else {
                        isPlaying = false;
                    }

                    // Important: If it is playing, stop it before playing the new sound
                    if (isPlaying) {
                        noteChannel->stop();
                    }

                    result = system->playSound(selectedSound, nullptr, false, &noteChannel);
                    FMODErrorCheck(result);
                }

            } else {
                std::cout << "Off Beat!" << std::to_string(off) << std::endl;
            }
        }

        bool isMainPlaying;
        if (mainChannel != nullptr) {
            mainChannel->isPlaying(&isMainPlaying);
            // repeat main music track and keep the beats
            if (!isMainPlaying && !(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count() % interval)) {
                result = system->playSound(main, nullptr, false, &mainChannel);
                startTime = std::chrono::steady_clock::now();
                FMODErrorCheck(result);
            }
        }
    }
}

