CREATE DATABASE IF NOT EXISTS pagecache;
USE pagecache;

CREATE TABLE IF NOT EXISTS experiments (
    experiment_id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    experiment_name VARCHAR(150) NOT NULL,
    algorithm VARCHAR(20) NOT NULL,
    cache_capacity INT UNSIGNED NOT NULL,
    workload_type VARCHAR(30) NOT NULL,
    total_accesses INT UNSIGNED NOT NULL DEFAULT 0,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CHECK (cache_capacity > 0)
);

CREATE TABLE IF NOT EXISTS experiment_results (
    result_id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    experiment_id BIGINT UNSIGNED NOT NULL UNIQUE,
    total_accesses INT UNSIGNED NOT NULL DEFAULT 0,
    hits INT UNSIGNED NOT NULL DEFAULT 0,
    misses INT UNSIGNED NOT NULL DEFAULT 0,
    evictions INT UNSIGNED NOT NULL DEFAULT 0,
    working_set INT UNSIGNED NOT NULL DEFAULT 0,
    admission_rejections INT UNSIGNED NOT NULL DEFAULT 0,
    hit_ratio DOUBLE NOT NULL DEFAULT 0,
    miss_ratio DOUBLE NOT NULL DEFAULT 0,
    average_latency DOUBLE NOT NULL DEFAULT 0,
    pollution_detected BOOLEAN NOT NULL DEFAULT FALSE,
    selected_algorithm VARCHAR(20) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_results_experiment
        FOREIGN KEY (experiment_id)
        REFERENCES experiments(experiment_id)
        ON DELETE CASCADE,
    CHECK (hits + misses = total_accesses)
);

CREATE TABLE IF NOT EXISTS access_logs (
    access_id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    experiment_id BIGINT UNSIGNED NOT NULL,
    sequence_number INT UNSIGNED NOT NULL,
    page_number INT NOT NULL,
    cache_hit BOOLEAN NOT NULL,
    evicted_page INT NULL,
    admitted BOOLEAN NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_logs_experiment
        FOREIGN KEY (experiment_id)
        REFERENCES experiments(experiment_id)
        ON DELETE CASCADE,
    UNIQUE KEY uq_experiment_sequence (experiment_id, sequence_number),
    INDEX idx_experiment_page (experiment_id, page_number)
);
