let currentSimulation = null;
let accessLog = [];
let currentStep = 0;
let isPlaying = false;
let playTimer = null;
let playSpeed = 300;
let currentCacheSize = 5;

// Workload Preset Configurations
const PRESETS = {
    oltp_zipfian: {
        algorithm: "ADAPTIVE",
        cacheSize: 5,
        writeRatio: "0.25",
        storageProfile: "nvme",
        count: 120,
        unique: 25,
        type: "zipfian"
    },
    table_scan_pollution: {
        algorithm: "ADAPTIVE",
        cacheSize: 6,
        writeRatio: "0.15",
        storageProfile: "nvme",
        count: 100,
        unique: 40,
        type: "table_scan"
    },
    cyclic_thrashing: {
        algorithm: "ADAPTIVE",
        cacheSize: 5,
        writeRatio: "0.25",
        storageProfile: "nvme",
        count: 90,
        unique: 6,
        type: "cyclic"
    },
    web_microservice: {
        algorithm: "LRU",
        cacheSize: 6,
        writeRatio: "0.10",
        storageProfile: "nvme",
        count: 120,
        unique: 30,
        type: "locality"
    },
    media_streaming: {
        algorithm: "FIFO",
        cacheSize: 4,
        writeRatio: "0.0",
        storageProfile: "nvme",
        count: 80,
        unique: 80,
        type: "sequential"
    }
};

let activePresetKey = "oltp_zipfian";

function applyPreset(presetKey) {
    activePresetKey = presetKey;
    const cfg = PRESETS[presetKey];
    if (!cfg) return;

    // Update UI highlights
    document.querySelectorAll(".preset-card").forEach(el => el.classList.remove("active"));
    const selectedPresetCard = Array.from(document.querySelectorAll(".preset-card"))
        .find(el => el.getAttribute("onclick") && el.getAttribute("onclick").includes(presetKey));
    if (selectedPresetCard) selectedPresetCard.classList.add("active");

    // Populate controls
    document.getElementById("algorithm").value = cfg.algorithm;
    document.getElementById("cacheSize").value = cfg.cacheSize;
    document.getElementById("writeRatio").value = cfg.writeRatio;
    document.getElementById("storageProfile").value = cfg.storageProfile;
    document.getElementById("workloadCount").value = cfg.count;
    document.getElementById("uniquePages").value = cfg.unique;

    // Automatically generate the workload sequence for this preset
    generateWorkload(cfg.type);
}

async function generateWorkload(overrideType) {
    const type = overrideType || PRESETS[activePresetKey]?.type || "zipfian";
    const count = Number(document.getElementById("workloadCount").value) || 100;
    const unique_pages = Number(document.getElementById("uniquePages").value) || 25;

    try {
        const res = await fetch("/api/workload/generate", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ type, count, unique_pages })
        });
        const data = await res.json();
        if (!res.ok) throw new Error(data.error || "Failed to generate workload");

        document.getElementById("pagesInput").value = data.pages.join(", ");
    } catch (err) {
        alert("Workload Generation Error: " + err.message);
    }
}

async function runSimulation() {
    pause();

    const algorithm = document.getElementById("algorithm").value;
    const cache_size = Number(document.getElementById("cacheSize").value);
    const write_ratio = Number(document.getElementById("writeRatio").value);
    const storage_profile = document.getElementById("storageProfile").value;
    const pagesStr = document.getElementById("pagesInput").value;

    const pages = pagesStr.split(",")
        .map(x => Number(x.trim()))
        .filter(x => Number.isFinite(x) && x >= 0);

    if (pages.length === 0) {
        alert("Please enter a valid comma-separated list of page numbers");
        return;
    }

    try {
        const res = await fetch("/api/simulate", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                algorithm,
                cache_size,
                write_ratio,
                storage_profile,
                pages,
                workload_type: activePresetKey
            })
        });

        const data = await res.json();
        if (!res.ok) throw new Error(data.error || "Simulation failed");

        currentSimulation = data;
        currentCacheSize = cache_size;
        accessLog = data.access_log || [];

        // Update KPIs
        const m = data.metrics;
        document.getElementById("kpiHitRatio").textContent = (m.hit_ratio * 100).toFixed(1) + "%";
        document.getElementById("kpiHitMiss").textContent = `${m.hits} Hits / ${m.misses} Misses (Total: ${m.total_requests})`;

        document.getElementById("kpiAvgLatency").textContent = m.avg_latency_ms.toFixed(4) + " ms";
        const baselineLatency = (storage_profile === "hdd" ? 8.5 : (storage_profile === "ebs" ? 1.2 : 0.08));
        const speedup = baselineLatency / Math.max(m.avg_latency_ms, 0.0001);
        document.getElementById("kpiSpeedup").textContent = `${speedup.toFixed(1)}x faster than direct disk`;

        document.getElementById("kpiDirtyEvictions").textContent = m.dirty_evictions;
        document.getElementById("kpiWritebacks").textContent = `Clean: ${m.clean_evictions} | Dirty Flushes: ${m.dirty_evictions}`;

        document.getElementById("kpiSsdWear").textContent = m.ssd_wear_saved_kb.toFixed(0) + " KB";
        document.getElementById("kpiIoSaved").textContent = `Time Saved: ${m.io_time_saved_ms.toFixed(2)} ms`;

        // Cache Pollution Alert
        const banner = document.getElementById("pollutionBanner");
        if (m.pollution_detected) {
            banner.style.display = "flex";
        } else {
            banner.style.display = "none";
        }

        // Render Benchmark Comparison Table
        renderBenchmarkTable(data.comparison_results, data.best_algorithm);

        // Initialize Step Visualizer
        const scrubber = document.getElementById("simScrubber");
        scrubber.max = Math.max(0, accessLog.length - 1);
        scrubber.value = 0;
        currentStep = 0;
        renderStep(0);

        // Refresh History from MySQL
        loadHistory();

    } catch (err) {
        alert("Simulation Error: " + err.message);
    }
}

function renderBenchmarkTable(comparisons, bestAlgorithm) {
    if (!comparisons || comparisons.length === 0) return;

    const tbody = document.getElementById("benchmarkTable");
    tbody.innerHTML = comparisons.map(c => {
        const isBest = c.algorithm === bestAlgorithm;
        const hitPercent = (c.hit_ratio * 100).toFixed(1);
        const missPercent = (c.miss_ratio * 100).toFixed(1);

        return `
            <tr class="${isBest ? 'winner-row' : ''}">
                <td>
                    <strong>${c.algorithm}</strong>
                    ${c.algorithm === 'ADAPTIVE' ? `<small style="color:var(--text-muted)"> (${c.selected_algorithm})</small>` : ''}
                    ${isBest ? `<span class="winner-badge">👑 BEST</span>` : ''}
                </td>
                <td>
                    <div class="progress-cell">
                        <div class="bar-container">
                            <div class="bar-fill" style="width: ${hitPercent}%; background: ${isBest ? 'var(--accent-emerald)' : 'var(--accent-cyan)'}"></div>
                        </div>
                        <span style="min-width:45px;font-weight:700;font-family:var(--font-mono)">${hitPercent}%</span>
                    </div>
                </td>
                <td style="font-family:var(--font-mono)">${missPercent}%</td>
                <td style="font-family:var(--font-mono)">${c.clean_evictions}</td>
                <td style="font-family:var(--font-mono);color:${c.dirty_evictions > 0 ? 'var(--accent-amber)' : 'inherit'}">${c.dirty_evictions}</td>
                <td style="font-family:var(--font-mono)">${c.avg_latency_ms.toFixed(4)} ms</td>
                <td style="font-family:var(--font-mono)">${c.p95_latency_ms.toFixed(4)} ms</td>
                <td style="font-family:var(--font-mono);color:var(--accent-emerald)">+${c.ssd_wear_saved_kb.toFixed(0)} KB</td>
            </tr>
        `;
    }).join("");
}

function renderStep(idx) {
    if (!accessLog || accessLog.length === 0 || idx < 0 || idx >= accessLog.length) {
        // Render empty frames
        renderEmptyFrames();
        return;
    }

    const entry = accessLog[idx];
    document.getElementById("simScrubber").value = idx;
    document.getElementById("stepCounter").textContent = `Step ${idx + 1} of ${accessLog.length}`;
    document.getElementById("stepOp").innerHTML = `
        Page <span style="color:var(--accent-cyan);font-size:16px;">[${entry.page}]</span> 
        <span style="font-size:12px;padding:2px 6px;border-radius:4px;background:#1e293b;border:1px solid var(--border)">
            ${entry.op === 'W' ? '✍️ WRITE' : '📖 READ'}
        </span>
    `;

    const hitBadge = document.getElementById("stepHitStatus");
    hitBadge.style.display = "inline-block";
    if (entry.hit) {
        hitBadge.className = "badge-hit";
        hitBadge.textContent = "CACHE HIT";
    } else {
        hitBadge.className = "badge-miss";
        hitBadge.textContent = "PAGE FAULT / MISS";
    }

    const evictInfo = document.getElementById("stepEvictionInfo");
    if (entry.evicted_page !== null && entry.evicted_page !== undefined) {
        if (entry.dirty_eviction) {
            evictInfo.innerHTML = `⚠️ Evicted: <strong>[${entry.evicted_page}]</strong> <span class="badge-dirty">DIRTY FLUSH TO STORAGE</span>`;
        } else {
            evictInfo.innerHTML = `Evicted: <strong>[${entry.evicted_page}]</strong> (Clean Frame Reclaimed)`;
        }
    } else {
        evictInfo.textContent = "No Eviction";
    }

    // Render Frames Grid
    const container = document.getElementById("framesContainer");
    const frames = entry.frames || [];

    let html = "";
    for (let f = 0; f < currentCacheSize; ++f) {
        const frameData = frames.find(x => x.frame === f);
        if (frameData && frameData.page !== -1) {
            const isTarget = (frameData.page === entry.page);
            let activeClass = "";
            if (isTarget) {
                activeClass = entry.hit ? "active-hit" : "active-miss";
            }

            html += `
                <div class="frame-slot ${activeClass}">
                    <div class="frame-id">Frame #${f}</div>
                    <div class="frame-page" style="color: ${frameData.dirty ? 'var(--accent-amber)' : 'var(--text-primary)'}">
                        ${frameData.page}
                    </div>
                    <div>
                        <span class="${frameData.dirty ? 'badge-dirty' : ''}" style="font-size:10px;font-weight:700">
                            ${frameData.dirty ? '🔥 DIRTY' : 'CLEAN'}
                        </span>
                    </div>
                    <div class="frame-meta">
                        <span>Hits: ${frameData.freq}</span>
                        <span>ref: ${frameData.ref ? '1' : '0'}</span>
                    </div>
                </div>
            `;
        } else {
            html += `
                <div class="frame-slot empty">
                    <div class="frame-id">Frame #${f}</div>
                    <div class="frame-page" style="color:var(--text-muted)">--</div>
                    <div style="font-size:10px;color:var(--text-muted)">EMPTY</div>
                    <div class="frame-meta">
                        <span>--</span>
                        <span>--</span>
                    </div>
                </div>
            `;
        }
    }

    container.innerHTML = html;
}

function renderEmptyFrames() {
    const container = document.getElementById("framesContainer");
    let html = "";
    for (let f = 0; f < currentCacheSize; ++f) {
        html += `
            <div class="frame-slot empty">
                <div class="frame-id">Frame #${f}</div>
                <div class="frame-page" style="color:var(--text-muted)">--</div>
                <div style="font-size:10px;color:var(--text-muted)">UNINITIALIZED</div>
            </div>
        `;
    }
    container.innerHTML = html;
}

// Visualizer Playback Controls
function simNext() {
    if (currentStep < accessLog.length - 1) {
        currentStep++;
        renderStep(currentStep);
    } else {
        pause();
    }
}

function simPrev() {
    if (currentStep > 0) {
        currentStep--;
        renderStep(currentStep);
    }
}

function simFirst() {
    currentStep = 0;
    renderStep(0);
}

function simLast() {
    if (accessLog.length > 0) {
        currentStep = accessLog.length - 1;
        renderStep(currentStep);
    }
}

function onScrub(val) {
    currentStep = Number(val);
    renderStep(currentStep);
}

function togglePlay() {
    if (isPlaying) {
        pause();
    } else {
        play();
    }
}

function play() {
    if (!accessLog || accessLog.length === 0) return;
    if (currentStep >= accessLog.length - 1) {
        currentStep = 0;
    }
    isPlaying = true;
    document.getElementById("playBtn").textContent = "⏸ Pause";
    clearInterval(playTimer);
    playTimer = setInterval(simNext, playSpeed);
}

function pause() {
    isPlaying = false;
    document.getElementById("playBtn").textContent = "▶ Play";
    clearInterval(playTimer);
}

function changeSpeed() {
    playSpeed = Number(document.getElementById("simSpeed").value);
    if (isPlaying) {
        play();
    }
}

// Load MySQL Simulation History
async function loadHistory() {
    try {
        const res = await fetch("/api/history");
        if (!res.ok) return;
        const rows = await res.json();
        const tbody = document.getElementById("historyTable");
        if (!rows || rows.length === 0) {
            tbody.innerHTML = `<tr><td colspan="7" style="text-align:center;color:var(--text-muted)">No past experiments found in database.</td></tr>`;
            return;
        }

        tbody.innerHTML = rows.map(r => `
            <tr>
                <td style="font-family:var(--font-mono)">#${r.simulation_id}</td>
                <td><strong>${r.algorithm}</strong></td>
                <td style="color:var(--text-muted)">${r.selected_algorithm || r.algorithm}</td>
                <td style="font-family:var(--font-mono)">${r.cache_size}</td>
                <td style="font-family:var(--font-mono);font-weight:700">${(r.hit_ratio * 100).toFixed(1)}%</td>
                <td style="font-family:var(--font-mono)">${Number(r.avg_latency_ms).toFixed(4)} ms</td>
                <td style="font-size:12px;color:var(--text-muted)">${new Date(r.created_at).toLocaleTimeString()}</td>
            </tr>
        `).join("");
    } catch (err) {
        console.error("Could not load history:", err);
    }
}

// Initialize on page load
window.addEventListener("DOMContentLoaded", () => {
    applyPreset("oltp_zipfian");
    loadHistory();
});
