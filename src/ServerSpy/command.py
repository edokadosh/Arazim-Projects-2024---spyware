import struct
from funCode import FunCode


class Command:
    FMT: str = "=II50s"
    sizeof: int = struct.calcsize("!II50s")

    def __init__(self, dataLen: int, funcode: FunCode, strParam: str) -> None:
        self.dataLen = dataLen
        self.funCode = funcode
        self.strParam = strParam.encode("utf-8")

    def pack(self) -> bytes:
        return struct.pack(self.FMT, self.dataLen, self.funCode, self.strParam)

    # maybe add an unpacking method


if __name__ == "__main__":
    cmd = Command(44, FunCode.DELETE_FILE, "mooooo")
    print(cmd.pack())
