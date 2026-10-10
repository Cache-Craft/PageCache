import json
import subprocess
from pathlib import Path

ENGINE_BINARY = Path("/tmp/pagecache_test")


def run_cache_experiment(algorithm, cache_size, pages):
    if not ENGINE_BINARY.exists():
        raise FileNotFoundError("Compile the C++ engine first.")

    if algorithm not in {"FIFO", "LRU", "LFU", "CLOCK", "ADAPTIVE"}:
        raise ValueError("Unsupported cache algorithm")

    if cache_size <= 0:
        raise ValueError("Cache size must be greater than zero")

    if not pages or any(not isinstance(page, int) for page in pages):
        raise ValueError("Pages must be a non-empty list of integers")

    command = [
        str(ENGINE_BINARY),
        "--algorithm", algorithm,
        "--cache-size", str(cache_size),
        "--pages", ",".join(map(str, pages)),
    ]

    result = subprocess.run(
        command,
        capture_output=True,
        text=True,
        timeout=30,
        check=True,
    )

    return json.loads(result.stdout)
