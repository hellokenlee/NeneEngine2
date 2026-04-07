"""
Dump .asset files to human-readable JSON.

Usage:
    python asset_dump.py <asset_file> [-o output.json]

Dependencies:
    pip install zstandard bjdata
"""

import argparse
import json
import struct
import uuid as _uuid
from pathlib import Path

import bjdata
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
        "uuid": str(_uuid.UUID(bytes=raw_uuid)),
        "type_name": type_name,
        "file_name": file_name,
    }


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

    content = bjdata.loadb(content_bytes)

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
