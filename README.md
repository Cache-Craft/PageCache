# PageCache — Adaptive Page Cache Management & Performance Analysis System

## 1. Project Overview

**PageCache** is a modular performance-analysis system that combines Operating Systems and Database Management Systems concepts to study data-access performance under different workloads.

The system provides a controlled environment for implementing and comparing cache replacement policies, generating memory-access workloads, collecting performance metrics, and storing experiment results in a database.

The project is being developed in multiple phases so that the cache engine, workload generation, database layer, backend, analytics, and user interface can be implemented and integrated incrementally.

---

## 2. Problem Statement

Modern computer systems frequently access large amounts of data from memory, storage, and databases. Since memory and cache space are limited, efficient cache/page replacement techniques are required to improve data-access performance.

Different cache replacement policies behave differently depending on the access pattern. Sequential, random, and mixed workloads can produce very different results for the same cache policy.

The problem addressed by PageCache is to provide a single experimental environment in which different cache replacement policies and workloads can be executed, measured, stored, and compared using real performance data.

---

## 3. Objectives

- Implement multiple cache replacement policies.
- Generate sequential, random, and mixed access workloads.
- Simulate cache/page replacement behavior.
- Measure cache hits, misses, hit ratio, evictions, and execution time.
- Compare the performance of different cache policies.
- Develop an adaptive cache mechanism capable of selecting a suitable policy based on workload characteristics.
- Integrate Operating System cache concepts with DBMS experiments.
- Store experiment configurations and results in a database.
- Provide backend APIs for experiment execution and result retrieval.
- Provide a dashboard for visualizing and comparing experiment results.
- Maintain a modular architecture so that each component can be developed and tested independently.

---

# 4. Team Members & Responsibilities

The project is developed by a three-member team. Responsibilities are distributed across Operating Systems, DBMS, algorithms, backend/API development, analytics, testing, and integration so that every member gains substantial experience in both major technical domains.

| Member                 | Role          | Primary Responsibilities                                                                                                                                                     |
| ---------------------- | ------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Abhinav Choudhary**  | **Team Lead** | Overall system architecture and coordination; cache-management/OS modules; database design and DBMS integration; backend/API coordination; code review and final integration |
| **Ansh Singh Negi**    | **Developer** | Cache replacement algorithms; cache simulation and OS-side metrics; database-related experiment/result modules; backend/API work; testing and documentation                  |
| **Yatharth Thapliyal** | **Developer** | Workload generation and OS-side simulation modules; database experiment/query modules; analytics and result processing; testing, validation, and integration                 |

### Shared Responsibilities

All three members contribute to:

- Operating System concepts
- DBMS concepts and database integration
- Algorithm implementation and analysis
- Testing and debugging
- Git/GitHub collaboration
- Documentation
- System integration
- Performance evaluation

---

# 5. Technologies & Tools

### Programming Languages

- C++17
- Python
- JavaScript
- SQL
- HTML
- CSS

### Core Technologies

- **C++17** — cache simulation engine and algorithms
- **Python / Flask** — backend and REST APIs
- **MySQL** — experiment/result persistence
- **HTML/CSS/JavaScript** — frontend/dashboard
- **Git & GitHub** — version control and collaboration

### Development Tools

- Visual Studio Code
- GCC / G++ or another C++17-compatible compiler
- Python 3.x
- MySQL Server
- MySQL Workbench
- Git

---

# 6. System Architecture

```text
                         ┌───────────────────────┐
                         │         USER          │
                         │ Experiment Parameters │
                         └───────────┬───────────┘
                                     │
                                     ▼
                         ┌───────────────────────┐
                         │       FRONTEND        │
                         │     HTML/CSS/JS       │
                         │                       │
                         │ • Configuration       │
                         │ • Simulation Control  │
                         │ • Results Dashboard   │
                         └───────────┬───────────┘
                                     │
                                  HTTP/REST
                                     │
                                     ▼
                         ┌───────────────────────┐
                         │    FLASK BACKEND      │
                         │       Python          │
                         │                       │
                         │ • API Routes          │
                         │ • Experiment Control  │
                         │ • Workload Services   │
                         │ • Result Processing   │
                         └───────┬───────┬───────┘
                                 │       │
                         subprocess      │ SQL
                                 │       │
                                 ▼       ▼
                    ┌────────────────┐  ┌──────────────────┐
                    │   C++ ENGINE   │  │      MySQL       │
                    │                │  │                  │
                    │ CacheManager   │  │ • workloads      │
                    │       │        │  │ • simulations    │
                    │       ▼        │  │ • access_logs    │
                    │ ┌────────────┐ │  │ • results        │
                    │ │ Algorithms │ │  └──────────────────┘
                    │ ├────────────┤ │
                    │ │ FIFO       │ │
                    │ │ LRU        │ │
                    │ │ LFU        │ │
                    │ │ CLOCK      │ │
                    │ └─────┬──────┘ │
                    │       │         │
                    │       ▼         │
                    │ Adaptive        │
                    │ Controller      │
                    │       │         │
                    │       ▼         │
                    │ Metrics /       │
                    │ Analytics       │
                    └────────┬────────┘
                             │
                         JSON Results
                             │
                             ▼
                    ┌──────────────────┐
                    │    DASHBOARD     │
                    │ • Hit Ratio      │
                    │ • Miss Ratio     │
                    │ • Execution Time │
                    │ • Comparisons    │
                    │ • History        │
                    └──────────────────┘
```

---

# 7. High-Level Data Flow

```text
User
  │
  ▼
Configure Experiment
  │
  ▼
Frontend
  │
  ▼
Flask REST API
  │
  ├──────────────► MySQL
  │
  ▼
C++ Cache Engine
  │
  ├── Workload
  ├── CacheManager
  ├── FIFO / LRU / LFU / CLOCK
  ├── Adaptive Controller
  └── Metrics / Analytics
  │
  ▼
JSON Result
  │
  ▼
Flask Backend
  │
  ├──────────────► MySQL
  │
  ▼
Frontend Dashboard
```

---

# 8. Major Features / Modules

## 8.1 Cache Replacement Algorithms

The project is designed to support:

- FIFO — First In First Out
- LRU — Least Recently Used
- LFU — Least Frequently Used
- CLOCK — Clock / Second-Chance replacement

Each policy can be evaluated using different cache sizes and workloads.

## 8.2 Workload Generator

Supported/planned workload types:

- Sequential workload
- Random workload
- Mixed workload

## 8.3 Cache Manager

Responsibilities include:

- Maintaining cache state
- Processing page/access requests
- Tracking hits and misses
- Handling evictions
- Selecting the active replacement policy
- Passing metrics to the analytics layer

## 8.4 Adaptive Cache Management

The adaptive module is designed to analyze workload characteristics and select an appropriate cache replacement policy.

Potential characteristics include:

- Access frequency
- Sequentiality
- Repeated accesses
- Working-set behavior

## 8.5 Metrics & Analytics

The system collects:

- Total accesses
- Cache hits
- Cache misses
- Hit ratio
- Miss ratio
- Evictions
- Execution time
- Access statistics
- Workload characteristics

## 8.6 Database Layer

MySQL is used for persistent storage of experiment-related information.

```text
page_cache_db
│
├── workloads
├── simulations
├── access_logs
└── algorithm_results
```

## 8.7 Flask Backend

The Flask backend integrates the frontend, C++ engine, and MySQL database.

Responsibilities include:

- Receiving experiment requests
- Validating parameters
- Generating/processing workloads
- Invoking the C++ engine
- Processing engine output
- Saving experiment results
- Returning results to the frontend

## 8.8 Frontend / Dashboard

The dashboard is designed to provide:

- Algorithm selection
- Cache-size configuration
- Workload selection
- Simulation controls
- Performance metrics
- Algorithm comparison
- Experiment history
- Graphical visualization

---

# 9. Project Structure

```text
PageCache/
│
├── backend/
│   ├── db/
│   ├── routes/
│   ├── services/
│   └── ...
│
├── cpp-engine/
│   ├── algorithms/
│   │   ├── FIFO.cpp
│   │   ├── FIFO.h
│   │   ├── LRU.cpp
│   │   ├── LRU.h
│   │   ├── LFU.cpp
│   │   └── LFU.h
│   │
│   ├── adaptive/
│   ├── analytics/
│   ├── cache/
│   ├── include/
│   └── workload/
│
├── database/
│   ├── schema.sql
│   └── ...
│
├── docs/
├── frontend/
├── scripts/
├── tests/
│   └── cpp/
└── README.md
```

> The structure may evolve as new modules are integrated.

---

# 10. Academic Project Note

PageCache is developed as an academic project to demonstrate the practical integration of Operating Systems and Database Management Systems concepts through a modular performance-analysis application.

The project is continuously evolving through multiple development phases. The progress checklist in this README is maintained as implementation continues.

---

## Project Status

**Current Phase:** Phase 2 — Cache Algorithms, Workloads & Initial Integration

**Overall Status:** In Progress

**Repository:** 'https://github.com/Cache-Craft/PageCache.git'
