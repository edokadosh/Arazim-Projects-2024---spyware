import ctypes

MAX_ADAPTER_NAMELEN = 16


class SniffParams(ctypes.Structure):

    _pack_ = 1

    _fields_ = [
        ("time", ctypes.c_int),
        ("adapter", ctypes.c_char * MAX_ADAPTER_NAMELEN),
    ]


if __name__ == "__main__":
    sniffp = SniffParams(5, b"ens33")
    print(sniffp)
