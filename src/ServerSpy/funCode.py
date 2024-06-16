from enum import IntFlag

# Maybe us aenum libary to make construction better


class FunCode(IntFlag):

    WRITE_FILE = 1
    DELETE_FILE = 2
    RUN_BASH = 3
    HIDER_SETUP = 4
    SUICIDE = 5

    HIDDEN_OPRATION = 1 << 6
    HIDDEN_UPLOAD = 1 << 1
    HIDDEN_RUN = 1 << 2
    HIDDEN_DELETE = 1 << 3
    HIDDEN_LIST = 1 << 4
    HIDDEN_RETREIVE_FILE = 1 << 5


if __name__ == "__main__":
    print(FunCode.WRITE_FILE)
    print(FunCode.HIDDEN_OPRATION | FunCode.HIDDEN_UPLOAD | FunCode.HIDDEN_RUN)
    print(FunCode(64 | 1 | 2))  # be cotious
