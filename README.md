# OS Page‑Replacement and Scheduling Algorithms

## Description
A C++ simulation framework for evaluating page‑replacement strategies across varying RAM capacities and process counts. The tool:

- **Generates** synthetic page‑reference streams based on user‑specified RAM size and number of processes.
- **Evaluates** four algorithms (OPT, FIFO, LRU, MRU) for every feasible page‑size, producing hit‑rate comparisons.
- **Employs** clean, object‑oriented design with modular, loosely coupled classes.
- **Incorporates** the Singleton pattern for efficient state management.

---

## Features
- **Synthetic Stream Generator**  
  Dynamically creates sequences of page requests per process.
- **Algorithm Evaluations**  
  Runs OPT, FIFO, LRU, and MRU on each stream, tracking misses and total references.
- **Comprehensive Reporting**  
  Outputs hit‑rate tables for all page sizes.
- **Modular Design**  
  Easily extendable architecture—add new algorithms or change parameters with minimal code impact.

---

## Code Flow

```text
main()
└─► handler::createHandler()
    ├─ input::getInput()       → User enters (noOfProcess, RAMSize, processSize)
    ├─ output::getOutput()
    └─ history::updateHistory(input, output)

└─► handler::analyzeOnAllPageSize()
    └─ for each pageSize ∈ [0 … min(RAMSize, processSize)]
        ├─ analyze = createAnalyze(...)
        └─ analyze->runProcesses()
            └─ for each process ∈ [0 … noOfProcess−1]
                ├─ proc    = createProcess(...)
                ├─ results = proc->runProcess()
                │   └─ for each algorithm ∈ {OPT, FIFO, LRU, MRU}
                │       instantiate RAM subclass → processRAM() → {miss, total}
                └─ mergeOutput(results)

        └─ output->mergeOutput(analyze.curOutput)

└─► handler::printAnalyzedData()
    └─ history::printCurrentStats() → Build & print hit‑rate table

