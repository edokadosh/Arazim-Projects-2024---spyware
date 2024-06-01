import struct
from status import Status


class Responce:

    FMT: str = "!II"
    sizeof: int = struct.calcsize("!II")

    def __init__(self, dataLen: int, status: Status) -> None:
        self.dataLen = dataLen
        self.status = status

    def __repr__(self) -> str:
        return f"<Responce(dataLen={self.dataLen}, status={self.status.name})"

    @classmethod
    def unpack(cls, raw_res: bytes) -> "Responce":
        assert cls.sizeof == len(raw_res), "raw responce size is invalid"
        dataLen, status = struct.unpack(cls.FMT, raw_res)
        return Responce(dataLen, Status(status))
