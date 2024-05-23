
#ifndef ERRORS_ENUM_H
#define ERRORS_ENUM_H

typedef enum Status {
    SUCCSESS = 0,

    INVALID_FUNCODE_ERROR,

    FILE_WRITE_ERROR,
    FILE_NOT_OPEN_ERROR,
    FILE_DELETION_ERROR,

    HIDDEN_FILE_WRITE_ERROR,
    HIDDEN_FILE_NOT_OPEN_ERROR,
    HIDDEN_FILE_DELETION_ERROR,

    HIDER_FORK_ERROR,    
    HIDER_PIPE_ERROR,
    HIDER_FORK_ERROR,
    HIDER_NO_ARGUMENTS_ERROR,

} Status;

#define STATUS_SHIFT_AMOUT (6) // how many to shift 1 status when trying combine statuses toghether
                              // currently you can compress 5 stauses to 1 uint

#endif