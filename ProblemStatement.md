# Project Title: Using Multithreading in a 2D Physics Engine in the Context of Games



## Problem Statement

When it comes to accurately simulating physics within a game engine, the number of calculations required can dramatically increase with the number of objects that physics acts upon. One object could bounce into many objects, which themselves could bounce into many more. This has famously made physics-based simulations tricky to optimize and costly to run. However, this is precisely the kind of workload that parallelization is suited to.



## Goal: Increase Performance of the Game with the 2D Physics Engine

The objective is not to create a traditional "fun" game but rather to build a physics simulation within a game engine. This approach allows us to utilize pre-made assets from a popular game engine and include a variety of elements that can physically interact with each other in distinct ways, such as mechanisms and static objects for those mechanisms to act upon.


## Technique to Address the Problem

We plan to use **multithreading** to accelerate the calculations of object interactions, which are typically the most significant cause of slowdowns in these simulations. By distributing the workload across multiple threads, we aim to reduce bottlenecks and improve overall performance.



## Expected Outcomes

We expect our project to demonstrate a **significant improvement in performance** when multithreading is applied. The project will showcase two versions of the simulation:

- **Single-threaded Version:** Likely to encounter performance bottlenecks, especially as the complexity of calculations increases.
- **Multithreaded Version:** Should display a smoother experience by distributing the computational workload across multiple threads.

We will present performance improvements through **charts and graphs**, comparing key metrics between the two versions.



## Tools and Technology

- **Graphics Library:** Raylib, chosen for its modular architecture, allowing us to focus on building the physics engine.
- **Programming Language:** C++, enabling efficient use of multithreading to solve the problem outlined.

