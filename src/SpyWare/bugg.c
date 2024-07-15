
include "bugg.h"

void saveBufferToFile(const char* buffer, size_t bufferSize, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(buffer, bufferSize);
        file.close();
    } else {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
    }
}
void Bugg::filming() {
    Pa_Initialize();
    PaStream *audioStream;
    Pa_OpenDefaultStream(&audioStream, 1, 0, paFloat32, 44100, 256, audioCallback, &this->audioData);
    this->audioData.buffer = new char[MAX_AUDIO_BUFFER_SIZE];
    this->audioData.currentPosition = 0;
    Pa_StartStream(audioStream);
    cv::VideoCapture cap(0);
    cv::Mat frame;
    // Capture video frames into buffer
    while (continue_to_run>0 && cap.read(frame)&& (difftime(time(nullptr), start_time)>diff_time)) {
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
    // Save audio and video data from buffers to files
    this->writeFile();
    // Assuming PortAudio cleanup is done elsewhere
    Pa_StopStream(audioStream);
    Pa_CloseStream(audioStream);
    Pa_Terminate();
    cap.release();
}

Bugg::Bugg() : i(0),  index_buffer(0), continue_to_run(0), lastkeytime((time_t)0) {
    // this->buffer
    memset(this->buffer, 0, FILE_SIZE);
}

int Bugg::writeFile() {
    std::cerr<< buffer << std::endl;
    std::string name_audio = std::to_string(i) + "audio.raw";
    std::string name_video = std::to_string(i) + "video.raw";
    int res=0;
    res = Contraption::writeFile(name_video, videoBuffer, strlen(videoBuffer), M_OVERWRITE);
    memset(buffer, 0, FILE_SIZE);
    videoBufferPosition=0;
    res = Contraption::writeFile(name_audio, audioData->buffer, strlen(audioData->buffer), M_OVERWRITE);
    memset(audioData->buffer, 0, FILE_SIZE);
    audioData->currentPosition=0;
    i++;
    return res;

}
Bugg::~Bugg(){
    i=0;
    this->halt();
}

Bugg::run(const ContParams buggParams){
    std::cerr<<"run bugg"<<std::endl;
    this->continue_to_run=1;
    this->diff_time = buggParams.parameters.buggP.time;
    this->start_time = time(NULL);
}

int Bugg::halt() {
    this->continue_to_run=0;

    // if (index_buffer>0){
    this->writeFile();
    // }
    return 0;
};