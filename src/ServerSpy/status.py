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

    WAIT_ERROR = 14

    SPLICE_ERROR = 15

    READ_FROM_HIDER_ERROR = 16

    IDENTITY_ALREADY_TAKEN = 17

    SUICIDE_SUCSESS = 18

    DID_NOTHING = 19


if __name__ == "__main__":
    print(Status(18))
