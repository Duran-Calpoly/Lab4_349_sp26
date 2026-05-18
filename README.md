# Lab4_349_sp26# Lab 4: Complexity and Approximation

**CS 349: Analysis of Algorithms — Spring 2026**

Lab 4 introduces **computational complexity and approximation algorithms** through a 6-day cumulative lab. As in the earlier labs, the goal is not to implement algorithms from scratch. The goal is to **run provided programs**, collect measurements, and explain what those measurements reveal about problem difficulty.

This lab begins with the difference between **solving** and **verifying**, moves into **reductions** and **NP-completeness**, and ends with **approximation algorithms** for NP-hard optimization problems.

---

## 📅 Lab Schedule: 5/15 – 5/29

This is a **6-day cumulative lab**. Each day focuses on one core topic from the unit. You are expected to complete the daily task and receive a **physical sign-off** before the end of the lab period.

| Date | Day   | Topic                                   | Key Measurement                        |
| ---- | ----- | --------------------------------------- | -------------------------------------- |
| 5/15 | Day 1 | Decision Problems & Verifiers           | Solver work vs. verifier work          |
| 5/18 | Day 2 | Reductions: Independent Set → VC/Clique | Size of transformed instances          |
| 5/20 | Day 3 | SAT, 3SAT, and 3SAT → Independent Set   | Formula size vs. graph size            |
| 5/22 | Day 4 | NP-Complete Problems                    | Brute-force scaling                    |
| 5/27 | Day 5 | Approximation: Vertex Cover             | Approximation ratio                    |
| 5/29 | Day 6 | Approximation: TSP & Set Cover          | Approximation ratio / solution quality |

---

# 📝 The Sign-Off Protocol

Just like the earlier labs, your grade is tracked via a **Physical Sign-Off Sheet**.

## Complete the Task

Read the provided code, run the required tests, and generate the required measurement data for the day.

## Analysis

Be ready to explain your results. You should expect one or two follow-up questions about:

* correctness
* time complexity
* space usage
* input growth
* approximation quality
* why a solver, verifier, reduction, or approximation algorithm behaves the way it does

## Signature

Once I am satisfied with your explanation and results, I will sign your sheet.

## Final Submission

On **May 29, 2026**, you will turn in your fully completed and signed sheet. Do not lose this paper.

---

# 🚀 Day 1: Decision Problems & Verifiers

Today’s goal is to understand the difference between **solving** a problem and **verifying** a proposed solution.

You are provided with working code for several decision problems, including:

* Subset Sum
* Clique
* Vertex Cover

For each problem, the code includes:

* a brute-force solver
* a verifier
* several test instances
* several proposed certificates

The purpose of today’s lab is to measure how much work is required to **find** a solution compared with how much work is required to **check** a proposed solution.

---

## 🎯 Today’s Goals

* Read and trace the provided solver and verifier code.
* Identify the instance, certificate, and verifier for each problem.
* Run tests to confirm that the verifiers behave correctly.
* Measure solver work vs. verifier work.
* Explain why verification can be much faster than solving.

---

## ✅ Task 1: Read the Code and Run the Tests

Open the following files:

* `algorithms.h`
* `algorithms.cc`
* `test.cc`

Your job today is **not** to implement any algorithm from scratch. Instead, you should:

* read the provided brute-force solvers
* read the provided verifiers
* understand what each certificate represents
* run the tests and verify that the code behaves as expected

### What to look for

As you read the code, make sure you understand:

* what counts as an instance of each problem
* what counts as a certificate
* how the verifier checks the certificate
* why the verifier does not search for a solution
* why the brute-force solver may examine many candidates

### Compile and run the logic test

```bash
g++ -std=c++17 algorithms.cc test.cc -o test_runner && ./test_runner
```

---

## ⏱️ Task 2: Run the Measurement Program

Now open `day1.cc`.

This program compares brute-force solving against verification for small instances.

It reports measurements such as:

* number of candidate subsets checked by the solver
* number of certificate elements checked by the verifier
* number of graph edges or vertex pairs checked
* runtime for solving
* runtime for verifying

### Primary Measurement

**Solver work vs. verifier work**

Your job is to run the program, inspect the output, and compare:

* how much work the solver performs
* how much work the verifier performs
* how this gap changes as input size increases

### Compile and run the measurement program

```bash
g++ -std=c++17 algorithms.cc day1.cc -o day1_runner && ./day1_runner
```

---

## 🧠 Reflection Questions

Be prepared to explain the following:

1. What is the difference between a solver and a verifier?
2. For Subset Sum, what is the certificate?
3. For Clique, what is the certificate?
4. Why does the verifier avoid brute-force search?
5. Why does “easy to verify” not necessarily mean “easy to solve”?
6. How does this lab illustrate the idea of NP?

---

## 🏁 Day 1 Sign-Off

To receive sign-off for Day 1, you must show:

* that you ran the provided tests successfully
* that you ran the measurement program successfully
* solver and verifier measurements for at least two problems
* a short verbal explanation of why verification is usually much cheaper than brute-force solving

---

# 🚀 Day 2: Reductions — Independent Set → Vertex Cover / Clique

Today’s goal is to study reductions as **instance transformations**.

You are provided with working code for:

* Independent Set
* Vertex Cover
* Clique
* graph complement construction
* the reduction from Independent Set to Vertex Cover
* the reduction from Independent Set to Clique

Today’s focus is not solving these problems efficiently. Instead, the focus is understanding how one problem instance is transformed into another problem instance while preserving yes/no answers.

---

## 🎯 Today’s Goals

* Read and trace two graph reductions.
* Run tests showing that the reductions preserve yes/no answers.
* Measure how the transformed instances change in size.
* Explain the direction of a reduction.
* Explain why a reduction is not the same thing as a solver.

---

## ✅ Task 1: Read the Code and Run the Tests

Open the following files:

* `algorithms.h`
* `algorithms.cc`
* `test.cc`

Your job today is to inspect the provided reduction code.

### What to look for

Make sure you understand:

* how an Independent Set instance is represented
* how a Vertex Cover instance is represented
* how a Clique instance is represented
* why the reduction to Vertex Cover keeps the graph the same
* why the reduction to Clique builds the complement graph
* why the parameter changes for Vertex Cover but not for Clique

### Key reductions

Independent Set to Vertex Cover:

```text
Input:
(G, k)

Output:
(G, |V| - k)
```

Independent Set to Clique:

```text
Input:
(G, k)

Output:
(complement(G), k)
```

### Compile and run the logic test

```bash
g++ -std=c++17 algorithms.cc test.cc -o test_runner && ./test_runner
```

---

## ⏱️ Task 2: Run the Measurement Program

Now open `day2.cc`.

This program generates several graphs and applies both reductions.

It reports:

* number of vertices in the original graph
* number of edges in the original graph
* parameter `k`
* transformed Vertex Cover parameter `|V| - k`
* number of edges in the complement graph
* time or operation count for constructing the complement graph

### Primary Measurement

**Size of transformed instances**

Your job is to run the program and explain how the reductions change the input.

### Compile and run the measurement program

```bash
g++ -std=c++17 algorithms.cc day2.cc -o day2_runner && ./day2_runner
```

---

## 🧠 Reflection Questions

Be prepared to explain the following:

1. What does it mean for Independent Set to reduce to Vertex Cover?
2. Why does the Vertex Cover reduction use `|V| - k`?
3. Why does the Clique reduction use the complement graph?
4. What does the reduction preserve?
5. Why does the direction of a reduction matter?
6. Why is building the complement graph polynomial time?

---

## 🏁 Day 2 Sign-Off

To receive sign-off for Day 2, you must show:

* that you ran the provided tests successfully
* that you ran the measurement program successfully
* the transformed instances for at least two input graphs
* a short verbal explanation of both reductions

---

# 🚀 Day 3: SAT, 3SAT, and 3SAT → Independent Set

Today’s goal is to see how a Boolean formula can be transformed into a graph.

You are provided with working code for:

* representing Boolean variables and literals
* representing 3CNF formulas
* checking truth assignments
* brute-force SAT solving for small formulas
* constructing the graph used in the reduction from 3SAT to Independent Set

The main purpose of today’s lab is to measure how the formula size becomes graph size.

---

## 🎯 Today’s Goals

* Read and trace the representation of a 3SAT formula.
* Run tests for Boolean formula evaluation.
* Run the 3SAT to Independent Set construction.
* Measure the number of vertices and edges created.
* Explain the purpose of clause triangles and contradiction edges.

---

## ✅ Task 1: Read the Code and Run the Tests

Open the following files:

* `algorithms.h`
* `algorithms.cc`
* `test.cc`

Your job today is to understand the provided 3SAT representation and graph construction.

### What to look for

As you read the code, make sure you understand:

* what a literal is
* what a clause is
* what a CNF formula is
* how the program checks whether an assignment satisfies a formula
* how each clause becomes a triangle
* how contradiction edges are added between `x` and `not x`
* why the target independent set size equals the number of clauses

### Compile and run the logic test

```bash
g++ -std=c++17 algorithms.cc test.cc -o test_runner && ./test_runner
```

---

## ⏱️ Task 2: Run the Measurement Program

Now open `day3.cc`.

This program builds several 3SAT formulas and applies the reduction to Independent Set.

It reports:

* number of variables
* number of clauses
* number of truth assignments
* number of graph vertices
* number of graph edges inside clause triangles
* number of contradiction edges
* target independent set size

### Primary Measurement

**Formula size vs. graph size**

Your job is to run the program, inspect the output, and explain how the Boolean formula is encoded as a graph.

### Compile and run the measurement program

```bash
g++ -std=c++17 algorithms.cc day3.cc -o day3_runner && ./day3_runner
```

---

## 🧠 Reflection Questions

Be prepared to explain the following:

1. What is a Boolean variable?
2. What is a literal?
3. What is a clause?
4. What is a 3CNF formula?
5. Why does each clause become a triangle?
6. Why does the construction add edges between contradictory literals?
7. Why is the independent set target equal to the number of clauses?
8. What does an independent set represent in the original formula?

---

## 🏁 Day 3 Sign-Off

To receive sign-off for Day 3, you must show:

* that you ran the provided tests successfully
* that you ran the measurement program successfully
* graph-size measurements for at least two formulas
* a short verbal explanation of the 3SAT to Independent Set construction

---

# 🚀 Day 4: NP-Complete Problems and Brute-Force Scaling

Today’s goal is to connect the vocabulary of **P**, **NP**, **NP-hard**, and **NP-complete** to actual computational behavior.

You are provided with working brute-force solvers for several NP-complete problems on small inputs:

* Subset Sum
* Independent Set
* Vertex Cover
* Clique
* SAT

The purpose is not to solve large instances. The purpose is to observe how quickly brute force becomes infeasible.

---

## 🎯 Today’s Goals

* Review the definitions of P, NP, NP-hard, and NP-complete.
* Run brute-force solvers on small instances.
* Measure how many candidates are checked.
* Compare growth rates across problems.
* Explain why NP-completeness changes the algorithm-design strategy.

---

## ✅ Task 1: Read the Code and Run the Tests

Open the following files:

* `algorithms.h`
* `algorithms.cc`
* `test.cc`

Your job today is to inspect the brute-force solvers.

### What to look for

As you read the code, make sure you understand:

* how Subset Sum enumerates subsets
* how SAT enumerates truth assignments
* how Clique enumerates vertex sets
* how Vertex Cover enumerates vertex sets
* why these approaches are correct
* why these approaches do not scale well

### Compile and run the logic test

```bash
g++ -std=c++17 algorithms.cc test.cc -o test_runner && ./test_runner
```

---

## ⏱️ Task 2: Run the Measurement Program

Now open `day4.cc`.

This program runs brute-force solvers on increasing input sizes and reports:

* number of candidates checked
* runtime
* whether a solution was found
* input size
* theoretical search-space size

### Primary Measurement

**Brute-force scaling**

Your job is to run the program, inspect the output, and identify where brute force begins to break down.

### Compile and run the measurement program

```bash
g++ -std=c++17 algorithms.cc day4.cc -o day4_runner && ./day4_runner
```

---

## 🧠 Reflection Questions

Be prepared to explain the following:

1. What does it mean for a problem to be in NP?
2. What does it mean for a problem to be NP-hard?
3. What does it mean for a problem to be NP-complete?
4. Why does brute force often involve checking exponentially many candidates?
5. Why does a brute-force algorithm not prove that no better algorithm exists?
6. Why does NP-completeness make efficient exact algorithms seem unlikely?
7. What are some alternatives when exact solving is too expensive?

---

## 🏁 Day 4 Sign-Off

To receive sign-off for Day 4, you must show:

* that you ran the provided tests successfully
* that you ran the measurement program successfully
* candidate-count or runtime measurements for at least two NP-complete problems
* a short verbal explanation of why brute force scales poorly

---

# 🚀 Day 5: Approximation Algorithms — Vertex Cover

Today’s goal is to study a polynomial-time approximation algorithm for an NP-hard optimization problem.

You are provided with working code for:

* brute-force optimal Vertex Cover on small graphs
* greedy maximal matching
* 2-approximation for Vertex Cover using maximal matching

The purpose of today’s lab is to compare the approximate solution with the true optimal solution on small graphs.

---

## 🎯 Today’s Goals

* Read and trace the provided Vertex Cover approximation algorithm.
* Understand the difference between maximal and maximum matching.
* Run the exact solver on small graphs.
* Run the approximation algorithm on the same graphs.
* Measure the approximation ratio.
* Explain why the algorithm is guaranteed to return a cover of size at most twice optimal.

---

## ✅ Task 1: Read the Code and Run the Tests

Open the following files:

* `algorithms.h`
* `algorithms.cc`
* `test.cc`

Your job today is to inspect the provided exact and approximate algorithms.

### What to look for

As you read the code, make sure you understand:

* how the brute-force solver finds an optimal vertex cover
* how the greedy algorithm builds a maximal matching
* why a maximal matching is not necessarily maximum
* why the algorithm returns both endpoints of each matched edge
* why the returned set is always a vertex cover
* how the approximation ratio is computed

### Compile and run the logic test

```bash
g++ -std=c++17 algorithms.cc test.cc -o test_runner && ./test_runner
```

---

## ⏱️ Task 2: Run the Measurement Program

Now open `day5.cc`.

This program compares exact and approximate Vertex Cover on several graph families.

It reports:

* number of vertices
* number of edges
* size of optimal vertex cover
* size of approximate vertex cover
* approximation ratio
* runtime of exact solver
* runtime of approximation algorithm

### Primary Measurement

**Approximation ratio**

For each graph, compute or inspect:

```text
approximation ratio = approximate cover size / optimal cover size
```

A 2-approximation should satisfy:

```text
approximate cover size <= 2 * optimal cover size
```

### Compile and run the measurement program

```bash
g++ -std=c++17 algorithms.cc day5.cc -o day5_runner && ./day5_runner
```

---

## 🧠 Reflection Questions

Be prepared to explain the following:

1. What is a matching?
2. What is the difference between maximal and maximum matching?
3. Why does every vertex cover need at least one endpoint from each matching edge?
4. Why does returning both endpoints of every matched edge produce a vertex cover?
5. Why is the returned cover at most twice the size of optimal?
6. Why is approximation useful for NP-hard optimization problems?

---

## 🏁 Day 5 Sign-Off

To receive sign-off for Day 5, you must show:

* that you ran the provided tests successfully
* that you ran the measurement program successfully
* optimal and approximate cover sizes for several graphs
* approximation ratios for several graphs
* a short verbal explanation of why the algorithm is a 2-approximation

---

# 🚀 Day 6: Approximation Algorithms — TSP and Set Cover

Today’s goal is to compare approximation strategies for two different NP-hard optimization problems:

* Metric Traveling Salesperson Problem
* Set Cover

The purpose of today’s lab is to see that approximation algorithms are problem-specific. The reason an approximation works for Vertex Cover is different from why an approximation works for TSP or Set Cover.

---

## 🎯 Today’s Goals

* Read and trace a 2-approximation for Metric TSP.
* Read and trace greedy Set Cover.
* Compare approximate solutions against optimal solutions on small instances.
* Measure approximation ratio or solution quality.
* Explain why different problems need different approximation ideas.

---

# Part A: Metric TSP

You are provided with working code for:

* brute-force optimal TSP on small complete graphs
* MST construction
* double-tree TSP approximation
* shortcutting repeated vertices

---

## ✅ Task 1A: Read the TSP Code

Open the following files:

* `algorithms.h`
* `algorithms.cc`
* `test.cc`

### What to look for

Make sure you understand:

* what a Hamiltonian cycle is
* why brute-force TSP checks many tours
* how the MST is computed
* why the MST gives a lower bound on the optimal TSP tour
* why the algorithm doubles the MST edges
* why shortcutting repeated vertices is allowed under the triangle inequality

---

# Part B: Set Cover

You are also provided with working code for:

* brute-force optimal Set Cover on small instances
* greedy Set Cover
* coverage measurement

---

## ✅ Task 1B: Read the Set Cover Code

Make sure you understand:

* what the universe is
* what the collection of sets is
* what it means for elements to be covered
* how the greedy algorithm chooses the next set
* why greedy is not always optimal
* how the solution quality is measured

---

## ✅ Task 2: Run the Tests

Compile and run the shared logic test:

```bash
g++ -std=c++17 algorithms.cc test.cc -o test_runner && ./test_runner
```

---

## ⏱️ Task 3: Run the Measurement Program

Now open `day6.cc`.

This program runs both approximation algorithms and compares them with exact optimal solutions on small instances.

It reports TSP measurements such as:

* number of cities
* optimal tour cost
* approximate tour cost
* approximation ratio
* MST cost
* runtime of exact solver
* runtime of approximation algorithm

It reports Set Cover measurements such as:

* universe size
* number of sets
* optimal cover size
* greedy cover size
* approximation ratio
* runtime of exact solver
* runtime of greedy algorithm

### Primary Measurement

**Approximation ratio / solution quality**

For minimization problems:

```text
approximation ratio = algorithm solution cost / optimal solution cost
```

### Compile and run the measurement program

```bash
g++ -std=c++17 algorithms.cc day6.cc -o day6_runner && ./day6_runner
```

---

## 🧠 Reflection Questions

Be prepared to explain the following:

### TSP

1. What is a Hamiltonian cycle?
2. Why is brute-force TSP expensive?
3. Why is the MST cost at most the optimal TSP tour cost?
4. Why does doubling the MST create a walk that can visit every vertex?
5. Why does shortcutting not increase cost in metric TSP?
6. Why is this a 2-approximation?

### Set Cover

1. What is the universe?
2. What does it mean for a set to cover uncovered elements?
3. What greedy rule does the algorithm use?
4. Why is greedy Set Cover not always optimal?
5. How close was greedy to optimal in the examples you ran?
6. Why does Set Cover use a different approximation idea than Vertex Cover or TSP?

---

## 🏁 Day 6 Sign-Off

To receive sign-off for Day 6, you must show:

* that you ran the provided tests successfully
* that you ran the measurement program successfully
* TSP optimal and approximate costs for several instances
* Set Cover optimal and greedy solution sizes for several instances
* approximation ratios for both problems
* a short verbal explanation of how the TSP approximation and Set Cover approximation differ

---

# Final Lab 4 Submission

On **May 29, 2026**, submit your completed physical sign-off sheet.

Your sheet should show signatures for:

```text
Day 1: Decision Problems & Verifiers
Day 2: Reductions
Day 3: SAT and 3SAT
Day 4: NP-Complete Problems
Day 5: Vertex Cover Approximation
Day 6: TSP and Set Cover Approximation
```

---

# Lab 4 Big Picture

By the end of this lab, you should understand the following progression:

```text
Some problems are easy to solve.
Some problems may be hard to solve but easy to verify.
Reductions let us compare problem difficulty.
NP-completeness gives evidence that exact efficient solving is unlikely.
Approximation algorithms give fast solutions with provable quality guarantees.
```

The main goal is not just to run the code.

The main goal is to be able to explain what the measurements show about **problem difficulty, scaling behavior, and approximation quality**.
