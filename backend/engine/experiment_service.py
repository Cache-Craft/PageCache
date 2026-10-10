from backend.db.connection import get_connection
from backend.engine.runner import run_cache_experiment


def run_and_save_experiment(algorithm, cache_size, pages, name="Cache Experiment"):
    if algorithm not in {"FIFO", "LRU", "LFU", "CLOCK"}:
        raise ValueError("The existing database supports FIFO, LRU, LFU and CLOCK.")

    if not isinstance(name, str) or not name.strip():
        raise ValueError("Experiment name cannot be empty.")

    result = run_cache_experiment(algorithm, cache_size, pages)
    metrics = result["metrics"]
    connection = get_connection()

    try:
        cursor = connection.cursor()
        connection.start_transaction()

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
                (experiment_id, algorithm, workload_type, cache_size, total_accesses)
            VALUES (%s, %s, 'CUSTOM', %s, %s)
            """,
            (experiment_id, algorithm, cache_size, len(pages)),
        )

        cursor.execute(
            """
            INSERT INTO cache_results
                (experiment_id, hits, misses, hit_ratio, miss_ratio, execution_time_ms)
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
        if "cursor" in locals():
            cursor.close()
        connection.close()
