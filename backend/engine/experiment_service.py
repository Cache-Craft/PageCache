import json

from backend.db.connection import get_connection
from backend.engine.runner import run_cache_experiment


def run_and_save_experiment(algorithm, cache_size, pages, name="Cache Experiment"):
    if algorithm not in {"FIFO", "LRU", "LFU", "CLOCK"}:
        raise ValueError("Unsupported algorithm for the existing database.")

    if not isinstance(name, str) or not name.strip():
        raise ValueError("Experiment name cannot be empty.")

    result = run_cache_experiment(algorithm, cache_size, pages)
    metrics = result["metrics"]
    connection = get_connection()
    cursor = None

    try:
        connection.start_transaction()
        cursor = connection.cursor()

        cursor.execute(
            """
            INSERT INTO experiments (experiment_type, name, status)
            VALUES ('CACHE', %s, 'RUNNING')
            """,
            (name.strip(),),
        )
        experiment_id = cursor.lastrowid

        cursor.execute(
            """
            INSERT INTO cache_experiments
                (experiment_id, algorithm, workload_type,
                 cache_size, total_accesses)
            VALUES (%s, %s, 'CUSTOM', %s, %s)
            """,
            (experiment_id, algorithm, cache_size, len(pages)),
        )

        cursor.execute(
            """
            INSERT INTO cache_results
                (experiment_id, hits, misses, hit_ratio,
                 miss_ratio, execution_time_ms)
            VALUES (%s, %s, %s, %s, %s, %s)
            """,
            (
                experiment_id,
                metrics["hits"],
                metrics["misses"],
                metrics["hit_ratio"],
                metrics["miss_ratio"],
                metrics["avg_latency_ms"],
            ),
        )

        for access in result["access_log"]:
            payload = {"experiment_id": experiment_id, **access}
            cursor.execute(
                """
                INSERT INTO benchmark_records (category, value, payload)
                VALUES (%s, %s, %s)
                """,
                (
                    "CACHE_ACCESS",
                    1.0 if access["hit"] else 0.0,
                    json.dumps(payload),
                ),
            )

        cursor.execute(
            """
            UPDATE experiments
            SET status = 'COMPLETED', completed_at = CURRENT_TIMESTAMP
            WHERE id = %s
            """,
            (experiment_id,),
        )

        connection.commit()
        return {"experiment_id": experiment_id, "result": result}

    except Exception:
        connection.rollback()
        raise

    finally:
        if cursor is not None:
            cursor.close()
        connection.close()
