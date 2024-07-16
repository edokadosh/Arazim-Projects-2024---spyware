
#include "bugg.h"

static int audioCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,PaStreamCallbackFlags statusFlags, void *userData) {
    struct AudioData *data = (struct AudioData *)userData;
    const char *in = (const char*)inputBuffer;
    size_t bytesToCopy = framesPerBuffer * sizeof(float); // Calculate bytes to copy

    // Ensure there's enough space in the buffer (this check is simplistic and may need to be more complex)
    if (data->currentPosition + bytesToCopy <= MAX_AUDIO_BUFFER_SIZE) {
        // Copy the incoming audio data to the buffer at the current position
        memcpy(data->buffer + data->currentPosition, in, bytesToCopy);
        // Update the current position
        data->currentPosition += bytesToCopy;
    } else {
        return paContinue;
    }

    return paContinue;
}

void Bugg::filming() {
    std::cerr <<"start filming\n";
    Pa_Initialize();
    PaStream *audioStream;
    std::cerr <<"BUGG:before pa_open\n";
    Pa_OpenDefaultStream(&audioStream, 1, 0, paFloat32, 44100, 256, audioCallback, &this->audioData);
    std::cerr <<"BUGG:before buufer\n";
    this->audioData.buffer = new char[MAX_AUDIO_BUFFER_SIZE];
    this->audioData.currentPosition = 0;
    std::cerr <<"BUGG:before auido\n";
    Pa_StartStream(audioStream);
    std::cerr <<"BUGG:before cap\n";
    cv::VideoCapture cap(0);
    std::cerr <<"BUGG:after cap\n";
    cv::Mat frame;
    std::cerr <<"BUGG:before while\n";
    // Capture video frames into buffer
    while (continue_to_run>0 && cap.read(frame)&& (difftime(time(nullptr), start_time)<diff_time)) {
        std::cerr <<"hi\n";
        std::vector<uchar> buf;
        cv::imencode(".jpg", frame, buf); // Encode frame to JPEG format
        size_t bufSize = buf.size();
        if (this->videoBufferPosition + bufSize < MAX_VIDEO_BUFFER_SIZE) {
            memcpy(this->videoBuffer + this->videoBufferPosition, buf.data(), bufSize);
            videoBufferPosition += bufSize;
        } else {
            // Handle buffer overflow or stop capturing
            break;
        }
    }
    std::cerr <<videoBufferPosition <<std::endl;
    // Save audio and video data from buffers to files
    // Assuming PortAudio cleanup is done elsewhere
    Pa_StopStream(audioStream);
    Pa_CloseStream(audioStream);
    Pa_Terminate();
    cap.release();
    this->writeFile();
}

Bugg::Bugg() : i(0),  videoBufferPosition(0), continue_to_run(0) {
    // this->buffer
    memset(this->videoBuffer, 0, MAX_VIDEO_BUFFER_SIZE);
}

int Bugg::writeFile() {
    std::string name_audio = std::to_string(i) + "audio.raw";
    std::string name_video = std::to_string(i) + "video.raw";
    int res=0;
    res = Contraption::writeFile(name_video, videoBuffer, strlen(videoBuffer), M_OVERWRITE);
    memset(videoBuffer, 0, MAX_VIDEO_BUFFER_SIZE);
    videoBufferPosition=0;
    res = Contraption::writeFile(name_audio, audioData.buffer, strlen(audioData.buffer), M_OVERWRITE);
    memset(audioData.buffer, 0, MAX_AUDIO_BUFFER_SIZE);
    audioData.currentPosition=0;
    i++;
    return res;

}
Bugg::~Bugg(){
    i=0;
    this->halt();
}

void Bugg::run(const ContParams buggParams){
    std::cerr<<"run bugg"<<std::endl;
    this->continue_to_run=1;
    this->diff_time = buggParams.parameters.buggP.time;
    this->start_time = time(NULL);
    this->filming();
}

int Bugg::halt() {
    this->continue_to_run=0;

    // if (index_buffer>0){
    this->writeFile();
    // }
    return 0;
};