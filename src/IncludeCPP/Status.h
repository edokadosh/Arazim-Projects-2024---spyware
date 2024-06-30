
#ifndef STATUS_ENUM_H
#define STATUS_ENUM_H

typedef enum Status  {
    SUCCSESS = 0,

    FAIL,

    INVALID_FUNCODE_ERROR,

    FILE_WRITE_ERROR,
    FILE_NOT_OPEN_ERROR,
    FILE_DELETION_ERROR,
    CHMOD_TO_EXE_ERROR,

    RECV_FILE_CONTENT_ERROR,

    HIDDEN_FILE_WRITE_ERROR,
    HIDDEN_FILE_NOT_OPEN_ERROR,
    HIDDEN_FILE_DELETION_ERROR,
    
    HIDER_PIPE_ERROR,
    HIDER_FORK_ERROR,
    HIDER_NO_ARGUMENTS_ERROR,
    HIDER_SETUP_ERROR,

    WAIT_ERROR,
    SPLICE_ERROR,

    READ_FROM_HIDER_ERROR,

    IDENTITY_ALREADY_TAKEN,
    CONTRAPTION_NOT_FOUND,

    SUICIDE_SUCSESS,

    DID_NOTHING,

    INVALID_RUN_PARAMS_TYPE,

    FILENAME_TO_LONG,

    INVALID_WRITE_MODE,


} Status;


#endif
