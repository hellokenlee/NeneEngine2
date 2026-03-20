"""
Dump .asset files to human-readable JSON.

Usage:
    python asset_dump.py <asset_file> [-o output.json]

Dependencies:
    pip install zstandard
"""

import argparse
import json
import struct
import sys
import uuid as _uuid
from pathlib import Path

import zstandard as zstd


# ---------------------------------------------------------------------------
# Header parser (asset_abstract binary format)
# ---------------------------------------------------------------------------

def parse_header(data: bytes) -> dict:
    off = 0
    raw_uuid = data[off:off + 16]
    off += 16

    type_name_len = struct.unpack_from("<I", data, off)[0]
    off += 4
    type_name = data[off:off + type_name_len].decode("utf-8")
    off += type_name_len

    file_name_len = struct.unpack_from("<I", data, off)[0]
    off += 4
    file_name = data[off:off + file_name_len].decode("utf-8")
    off += file_name_len

    return {
        "uuid": str(_uuid.UUID(bytes_le=raw_uuid)),
        "type_name": type_name,
        "file_name": file_name,
    }


# ---------------------------------------------------------------------------
# Minimal BJData / UBJSON parser (covers types emitted by nlohmann json)
# ---------------------------------------------------------------------------

class BJDataReader:
    def __init__(self, data: bytes):
        self._buf = data
        self._pos = 0

    def _read(self, n: int) -> bytes:
        chunk = self._buf[self._pos:self._pos + n]
        self._pos += n
        return chunk

    def _peek(self) -> int:
        return self._buf[self._pos]

    def _read_marker(self) -> str:
        return chr(self._read(1)[0])

    def _read_value_of_type(self, marker: str):
        if marker == "Z":
            return None
        if marker == "N":
            return None
        if marker == "T":
            return True
        if marker == "F":
            return False
        if marker == "i":
            return struct.unpack_from("<b", self._read(1))[0]
        if marker == "U":
            return struct.unpack_from("<B", self._read(1))[0]
        if marker == "I":
            return struct.unpack_from("<h", self._read(2))[0]
        if marker == "u":
            return struct.unpack_from("<H", self._read(2))[0]
        if marker == "l":
            return struct.unpack_from("<i", self._read(4))[0]
        if marker == "m":
            return struct.unpack_from("<I", self._read(4))[0]
        if marker == "L":
            return struct.unpack_from("<q", self._read(8))[0]
        if marker == "M":
            return struct.unpack_from("<Q", self._read(8))[0]
        if marker == "d":
            return struct.unpack_from("<f", self._read(4))[0]
        if marker == "D":
            return struct.unpack_from("<d", self._read(8))[0]
        if marker == "h":
            # float16 stored as 2 bytes
            import numpy as np
            return float(np.frombuffer(self._read(2), dtype="<f2")[0])
        if marker == "C":
            return chr(self._read(1)[0])
        if marker == "S":
            length = self._read_length()
            return self._read(length).decode("utf-8")
        if marker == "H":
            length = self._read_length()
            return self._read(length).decode("utf-8")
        if marker == "[":
            return self._read_array()
        if marker == "{":
            return self._read_object()
        raise ValueError(f"Unknown BJData marker: {marker!r} at pos {self._pos - 1}")

    def read_value(self):
        marker = self._read_marker()
        return self._read_value_of_type(marker)

    def _read_length(self) -> int:
        marker = self._read_marker()
        val = self._read_value_of_type(marker)
        return int(val)

    def _read_array(self):
        result = []
        m = self._read_marker()
        if m == "$":
            elem_type = self._read_marker()
            assert self._read_marker() == "#"
            count = self._read_length()
            for _ in range(count):
                result.append(self._read_value_of_type(elem_type))
        elif m == "#":
            count = self._read_length()
            for _ in range(count):
                result.append(self.read_value())
        else:
            # no count, read until ']'
            if m != "]":
                result.append(self._read_value_of_type(m))
                while True:
                    m = self._read_marker()
                    if m == "]":
                        break
                    result.append(self._read_value_of_type(m))
        return result

    def _read_object(self):
        result = {}
        m = self._read_marker()
        if m == "$":
            val_type = self._read_marker()
            assert self._read_marker() == "#"
            count = self._read_length()
            for _ in range(count):
                key_len = self._read_length()
                key = self._read(key_len).decode("utf-8")
                result[key] = self._read_value_of_type(val_type)
        elif m == "#":
            count = self._read_length()
            for _ in range(count):
                key_len = self._read_length()
                key = self._read(key_len).decode("utf-8")
                result[key] = self.read_value()
        else:
            # no count, read until '}'
            if m != "}":
                self._pos -= 1
                while True:
                    if chr(self._peek()) == "}":
                        self._pos += 1
                        break
                    key_len = self._read_length()
                    key = self._read(key_len).decode("utf-8")
                    result[key] = self.read_value()
        return result


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def dump_asset(filepath: str) -> dict:
    with open(filepath, "rb") as f:
        data = f.read()

    off = 0

    header_size = struct.unpack_from("<I", data, off)[0]
    off += 4
    header_info = parse_header(data[off:off + header_size])
    off += header_size

    original_size = struct.unpack_from("<I", data, off)[0]
    off += 4
    compressed = data[off:]

    decompressor = zstd.ZstdDecompressor()
    content_bytes = decompressor.decompress(compressed, max_output_size=original_size)

    reader = BJDataReader(content_bytes)
    content = reader.read_value()

    return {
        "header": header_info,
        "content": content,
    }


def main():
    parser = argparse.ArgumentParser(description="Dump .asset file to JSON")
    parser.add_argument("asset_file", help="Path to .asset file")
    parser.add_argument("-o", "--output", help="Output JSON file (default: stdout)")
    args = parser.parse_args()

    result = dump_asset(args.asset_file)
    json_str = json.dumps(result, indent=2, ensure_ascii=False)

    if args.output:
        Path(args.output).write_text(json_str, encoding="utf-8")
        print(f"Written to {args.output}")
    else:
        print(json_str)


if __name__ == "__main__":
    main()
