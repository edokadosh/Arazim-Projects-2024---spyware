#include <opencv2/opencv.hpp>
#include <portaudio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include "Contraption.h"

#define MAX_AUDIO_BUFFER_SIZE 44100 * 120 // Example: 10 seconds at 44100Hz
#define MAX_VIDEO_BUFFER_SIZE 1024 * 1024 * 100 // Example: 100MB

// COMPILATION FLAG NEEDED: -ltins
// USAGE EXAMPLE: g++ -o Sniffer Sniffer.cpp -ltins
class Bugg : public Contraption
{
private:
//to make shore it cant be run twiise??
    struct AudioData {
    char* buffer;
    size_t currentPosition;
    };
    int i;
    char videoBuffer[MAX_VIDEO_BUFFER_SIZE];
    int videoBufferPosition;
    AudioData audioData;
    int continue_to_run;
    time_t start_time;
    time_t diff_time;
    int i;
    char buffer[FILE_SIZE];
    int index_buffer;
    int continue_to_run;
    void filming();
    int writeFile();
    //void runTime(KilgParams kilgParams, int t);

public:
    Bugg(/* args */);
    ~Bugg();

    int halt() override;
    void run(const ContParams buggParams) override;
};