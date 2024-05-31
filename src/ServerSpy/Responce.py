import struct

FMT: str = "!II"


class Responce:

    def __init__(self, dataLen: int, status: int) -> None:
        self.dataLen = dataLen
        self.status = status

    @classmethod
    def unpack(cls, raw_res: bytes) -> "Responce":
        assert struct.calcsize(FMT) == len(raw_res), "raw responce size is invalid"
        dataLen, status = struct.unpack(FMT, raw_res)
        return Responce(dataLen, status)
