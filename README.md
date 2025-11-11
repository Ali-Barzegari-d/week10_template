# CSI2372 — Week 10 Lab Handout

**Topic:** Concurrency • Threads • Synchronization • File I/O • Build Graphs  
**Course:** Advanced Programming Concepts with C++ (Fall 2025, uOttawa)

---

## 1. Overview

In this lab you will apply concurrency, synchronization, safe resource management, and robust I/O parsing.  
You will implement modular components that illustrate thread lifecycle management, timed worker pools, safe file buffers, and simple build-graph analysis.

The lab consists of four tasks (A–D):

- **Task A — ThreadedAccumulator** — parallel sum using `std::thread` and `std::mutex`.
- **Task B — TimedWorkerPool** — worker pool with `condition_variable::wait_for` and graceful idle shutdown.
- **Task C — SafeFileBuffer** —  class that manages file I/O safely using **RAII** and **move semantics**
- **Task D — AsyncJobQueue** —  class that manages background task execution safely using **threads**, **RAII**

All work is autograded via CI/CTest. When the GitHub Actions build is green, you earn full marks.

---

## 2. Learning Objectives

By the end of this lab you should be able to:

- Create and manage threads safely (`std::thread`, join discipline).
- Use `std::mutex`, `std::lock_guard`, and `std::condition_variable` (including timed waits).
- Design graceful shutdown semantics for concurrent systems.
- Parse and validate text-based configuration / dependency files and report errors.
- Apply RAII and exception-safety principles to I/O and concurrency code.

---

## 3. Task A — ThreadedAccumulator

**You are given** a class `ThreadedAccumulator` to compute the sum of an `std::vector<int>` concurrently.

### Required Behavior

- Constructor: `ThreadedAccumulator(const std::vector<int>& data, size_t thread_count)`.
  - If `thread_count == 0` throw `std::invalid_argument("Invalid thread count")`.
- Method: `long compute_sum()`:
  - Partition the input fairly across threads.
  - Each thread computes a partial sum and updates a shared `total_sum` under `std::mutex` protection.
  - Use `std::lock_guard` (RAII) for updates.
  - All threads must be joined before returning the result.
- Deterministic behavior: same input → same sum.

### Concepts Covered

`std::thread`, `std::mutex`, `std::lock_guard`, thread join discipline, exception safety.

---

## 4. Task B — TimedWorkerPool

**You are given** a class `TimedWorkerPool` that accepts tasks and maintains a pool of workers which terminate after being idle for a configured timeout.

### Required Behavior

- Constructor: `TimedWorkerPool(size_t initial_workers, std::chrono::milliseconds idle_timeout)`.
  - If `initial_workers == 0` throw `std::invalid_argument`.
- `void submit(std::function<void()> task)`:
  - Thread-safe producer: pushes task into a protected queue and notifies a worker.
  - If pool is shutting down, throw `std::runtime_error("Pool is shutting down")`.
- `void shutdown()`:
  - Initiates graceful shutdown, notifies all workers, and blocks until threads are joined.
- Worker behavior:
  - Each worker waits on a `std::condition_variable` using `wait_for(lock, idle_timeout, predicate)`.
  - If timeout occurs and the queue is empty, the worker exits (idle shutdown).
  - If the pool is shutting down, workers finish remaining tasks then exit.
- Ensure no data races, and RAII-style cleanup (destructor must join threads or call `shutdown`).

### Concepts Covered

`std::condition_variable::wait_for`, producer-consumer queue, RAII for threads, graceful shutdown, atomic/lock patterns.

---

## 5. Task C — SafeFileBuffer

Implement a class `SafeFileBuffer` that manages file I/O safely using **RAII** and **move semantics**.

### Required Behavior
- Constructor opens a file; throws `std::runtime_error("Cannot open file")` if it fails.  
- Copying is disabled; moving is allowed (moved-from objects must be closed).  
- `write_bytes(const std::vector<char>&)` writes and flushes data; throws on error.  
- `read_bytes()` reads all bytes; throws on error.  
- `is_open() const noexcept` returns whether the file is open.  
- Destructor closes the file safely (no-throw).

### Concepts Covered
RAII • Move Semantics • File I/O • Exception Safety

---

## Task D — AsyncJobQueue

Implement a class `AsyncJobQueue` that manages background task execution safely using **threads**, **RAII**, and **move semantics**.

### Required Behavior

- Constructor starts a background worker thread that continuously processes queued jobs.  
- `enqueue(std::function<void()>)` safely adds a job to the queue and notifies the worker.  
- `stop()` stops the worker loop and notifies all waiting threads.  
- Destructor automatically calls `stop()` and joins the thread (no leaks or crashes).  
- Copying is disabled; moving is allowed — moved-to objects must spawn a valid worker thread.  
- Moved-from objects must be safely stopped and joined.  
- Worker loop must catch all exceptions thrown by jobs and continue running.  
- Implementation must guarantee **no resource leaks** and **safe concurrent access** to the queue.

### Concepts Covered

 RAII • Threads • Move Semantics • Condition Variables • Exception Safety • Concurrency Control

---

## 7. Building and Running

Recommended workflow (from repo root):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure

```

## 8. Grading

100% autograded: your score = proportion of passing tests (unit tests validate correctness and required guarantees).

No manual grading.
