import struct

FMT: str = "!II50s"


class Command:

    def __init__(self, dataLen: int, funCode: int, strParam: str) -> None:
        self.dataLen = dataLen
        self.funCode = funCode
        self.strParam = strParam.encode("utf-8")

    def pack(self) -> bytes:
        return struct.pack(FMT, self.dataLen, self.funCode, self.strParam)

    # maybe add an unpacking method
