from enum import IntEnum, auto
import struct


class Status(IntEnum):

    @classmethod
    def unpack(cls, raw_res: bytes) -> "Status":
        FMT = "!I"
        sizeof = struct.calcsize(FMT)
        assert sizeof == len(raw_res), "raw status size is invalid"
        status = struct.unpack(FMT, raw_res)
        return Status(status)

    def __repr__(self) -> str:
        return super().__repr__()

    SUCCSESS = 0

    FAIL = 1

    INVALID_FUNCODE_ERROR = 2

    FILE_WRITE_ERROR = 3
    FILE_NOT_OPEN_ERROR = 4
    FILE_DELETION_ERROR = 5
    CHMOD_TO_EXE_ERROR = 6

    RECV_FILE_CONTENT_ERROR = 7

    HIDDEN_FILE_WRITE_ERROR = 8
    HIDDEN_FILE_NOT_OPEN_ERROR = 9
    HIDDEN_FILE_DELETION_ERROR = 10

    HIDER_PIPE_ERROR = 11
    HIDER_FORK_ERROR = 12
    HIDER_NO_ARGUMENTS_ERROR = 13
    HIDER_SETUP_ERROR = 14

    WAIT_ERROR = 15

    SPLICE_ERROR = 16

    READ_FROM_HIDER_ERROR = 17

    IDENTITY_ALREADY_TAKEN = 18
    CONTRAPTION_NOT_FOUND = 19

    SUICIDE_SUCSESS = 20

    DID_NOTHING = 21

    INVALID_RUN_PARAMS_TYPE = 22

    FILENAME_TO_LONG = 23

    INVALID_WRITE_MODE = 24

    ERROR_RECVIVING_FROM_CONNECTION = 25

    ERROR_WRITING_TO_HIDER = 26

    HIDER_DIDNT_RECV_ENTIRE_UPLOAD = 27


if __name__ == "__main__":
    print(Status(18))
