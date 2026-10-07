import json, os, subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def run_engine(algorithm, cache_size, pages):

    configured = os.getenv("CPP_ENGINE_PATH")

    exe = (
        Path(configured)
        if configured
        else ROOT / "cpp-engine" / "build" / "page_cache_engine.exe"
    )

    if not exe.exists():
        raise FileNotFoundError(
            f"C++ engine not found: {exe}"
        )

    p = subprocess.run(
        [
            str(exe),
            "--algorithm",
            algorithm,
            "--cache-size",
            str(cache_size),
            "--pages",
            ",".join(map(str, pages))
        ],
        capture_output=True,
        text=True,
        timeout=30
    )

    if p.returncode != 0:
        raise RuntimeError(
            p.stderr.strip() or "C++ engine failed"
        )

    return json.loads(p.stdout)