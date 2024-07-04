import struct
from funCode import FunCode
from icecream import ic


class Command:
    FMT: str = "=IIII100s"
    sizeof: int = struct.calcsize("=IIII100s")

    def __init__(
        self,
        dataLen: int,
        funcode: FunCode,
        identifier: int,
        writeMod: int,
        strParam: str,
    ) -> None:
        self.dataLen = dataLen
        self.funCode = funcode
        self.identifier = identifier
        self.writeMod = writeMod
        self.strParam = strParam.encode("utf-8")

    def pack(self) -> bytes:
        return struct.pack(
            self.FMT,
            self.dataLen,
            self.funCode,
            self.identifier,
            self.writeMod,
            self.strParam,
        )

    # maybe add an unpacking method


if __name__ == "__main__":
    cmd = Command(44, FunCode.DELETE_FILE, 0, 1, "mooooo")
    print(cmd.pack())
