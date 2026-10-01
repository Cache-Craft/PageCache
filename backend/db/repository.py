from .connection import connect

def save_simulation(result, algorithm, cache_size, workload_id=None):
    c = connect()
    cur = c.cursor()
    m = result["metrics"]
    cur.execute("""
        INSERT INTO simulations
        (algorithm,selected_algorithm,cache_size,workload_id,total_requests,
         hit_count,miss_count,eviction_count,hit_ratio,miss_ratio,
         avg_latency_ms,working_set_size,pollution_detected,admission_rejections)
        VALUES (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)
    """, (
        algorithm,result["selected_algorithm"],cache_size,workload_id,
        m["total_requests"],m["hits"],m["misses"],m["evictions"],
        m["hit_ratio"],m["miss_ratio"],m["avg_latency_ms"],
        m["working_set_size"],m["pollution_detected"],m["admission_rejections"]
    ))
    
    
    sid = cur.lastrowid
    for a in result["access_log"]:
        cur.execute("""
            INSERT INTO access_logs
            (simulation_id,sequence_no,page_number,hit,admitted,evicted_page)
            VALUES (%s,%s,%s,%s,%s,%s)
        """,(sid,a["sequence"],a["page"],a["hit"],a["admitted"],a["evicted_page"]))
    c.commit(); cur.close(); c.close()
    return sid