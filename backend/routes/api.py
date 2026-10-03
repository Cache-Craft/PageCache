from flask import Blueprint, request, jsonify
from services.engine import run_engine
from services.workloads import generate
from db.repository import save_simulation, save_workload, recent

api = Blueprint("api", __name__)


@api.post("/simulate")
def simulate():
    d = request.get_json(silent=True) or {}

    algorithm = str(d.get("algorithm", "LRU")).upper()
    cache_size = int(d.get("cache_size", 4))
    pages = d.get("pages")

    if algorithm not in {"FIFO", "LRU", "LFU", "CLOCK", "ADAPTIVE"}:
        return jsonify({"error": "Unsupported algorithm"}), 400

    if cache_size < 1 or not isinstance(pages, list) or not pages:
        return jsonify({
            "error": "Valid cache_size and non-empty pages are required"
        }), 400

    if len(pages) > 100000:
        return jsonify({
            "error": "Maximum 100000 page references"
        }), 400

    try:
        result = run_engine(algorithm, cache_size, pages)

        sid = save_simulation(
            result,
            algorithm,
            cache_size
        )

        result["simulation_id"] = sid

        return jsonify(result)

    except Exception as e:
        return jsonify({"error": str(e)}), 500


@api.post("/workload/generate")
def workload():
    d = request.get_json(silent=True) or {}

    try:
        kind = str(d.get("type", "locality"))

        count = min(
            max(int(d.get("count", 100)), 1),
            100000
        )

        unique_pages = min(
            max(int(d.get("unique_pages", 20)), 1),
            10000
        )

        pages, score = generate(
            kind,
            count,
            unique_pages
        )

        wid = save_workload(
            kind,
            pages,
            score
        )

        return jsonify({
            "workload_id": wid,
            "pages": pages,
            "count": len(pages),
            "unique_pages": len(set(pages)),
            "locality_score": score
        })

    except Exception as e:
        return jsonify({"error": str(e)}), 400


@api.get("/history")
def history():
    try:
        return jsonify(recent())

    except Exception as e:
        return jsonify({"error": str(e)}), 500