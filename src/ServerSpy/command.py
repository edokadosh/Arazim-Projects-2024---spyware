import struct
from funCode import FunCode
from icecream import ic


class Command:
    FMT: str = "=III50s"
    sizeof: int = struct.calcsize("=III50s")

    def __init__(
        self, dataLen: int, funcode: FunCode, identifier: int, strParam: str
    ) -> None:
        self.dataLen = dataLen
        self.funCode = funcode
        self.identifier = identifier
        self.strParam = strParam.encode("utf-8")

    def pack(self) -> bytes:
        print(f"{self.strParam}")
        return struct.pack(
            self.FMT, self.dataLen, self.funCode, self.identifier, self.strParam
        )

    # maybe add an unpacking method


if __name__ == "__main__":
    cmd = Command(44, FunCode.DELETE_FILE, 0, "mooooo")
    print(cmd.pack())
