# Lab 1: Calculation of π Using the Monte Carlo Method

Concurrency and Parallelism 2022-23

Hervé Paulino <<herve.paulino@fct.unl.pt>>

Alex Davidson <<a.davidson@fct.unl.pt>>

March 2023

## Assignment

Task: Write sequential and parallel implementations of a Monte Carlo method simulation for estimating the size of π using C and `pthreads`.

Assignment PDF: <https://raw.githubusercontent.com/MEI-CP/lab-assignments>

### Running code

Compile the C code (e.g. using `gcc`):
```
gcc approxPi.c -o approxPi -lpthread
```

Run estimation of pi (output written to stdout and CSV):
```
sh run_tests.sh
```

## Performance profiling

Remember: to do more fine-grained performance testing of your code, you will need to profile individual pieces of your code. You should be able to set this up either in your IDE or in the command line (e.g. using perf/dtrace/gprof). Flame graphs provide a nice visualisations of CPU usage during a program execution, [this blog post](https://www.brendangregg.com/FlameGraphs/cpuflamegraphs.html) provides an example.