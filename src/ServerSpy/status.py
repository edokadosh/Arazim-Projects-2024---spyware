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

    INVALID_FUNCODE_ERROR = auto()

    FILE_WRITE_ERROR = auto()
    FILE_NOT_OPEN_ERROR = auto()
    FILE_DELETION_ERROR = auto()
    CHMOD_TO_EXE_ERROR = auto()

    HIDDEN_FILE_WRITE_ERROR = auto()
    HIDDEN_FILE_NOT_OPEN_ERROR = auto()
    HIDDEN_FILE_DELETION_ERROR = auto()

    HIDER_PIPE_ERROR = auto()
    HIDER_FORK_ERROR = auto()
    HIDER_NO_ARGUMENTS_ERROR = auto()

    WAIT_ERROR = auto()

    SPLICE_ERROR = auto()
