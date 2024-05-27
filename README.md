# NQueens Problem Solver

Welcome to the NQueens Problem Solver repository! This project contains a C++ implementation to solve the NQueens problem, a classic algorithmic challenge in computer science.

## Table of Contents

- [NQueens Problem Solver](#nqueens-problem-solver)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Features](#features)
  - [Installation](#installation)
  - [Usage](#usage)
    - [Usage for big inputs](#usage-for-big-inputs)
  - [License](#license)

## Introduction

The NQueens problem involves placing N chess queens on an N×N chessboard so that no two queens threaten each other. This means no two queens can share the same row, column, or diagonal. The challenge is to find all possible solutions for a given N.

## Features

- **Efficient Algorithm**: Utilizes the min-conflict heuristic to efficiently explore possible queen placements.
- **Customizable Board Size**: Allows users to specify the size of the board (N).
- **Detailed Output**: Displays a valid solution and execution time for the given N.

## Installation

To use this solver, you need to have a C++ compiler installed on your system. Follow these steps to set up the project:

1. Clone the repository:
    ```sh
    git clone https://github.com/your-username/NQueensSolver.git
    cd Nqueens
    ```

2. Compile the program:
    ```sh
    clang++ -03 Nqueens.cpp -o Nqueens
    ```

## Usage

Run the compiled executable and provide the board size as an input:

```sh
./Nqueens 
[N]
```
Replace [N] with the size of the board. For example, to solve the 8-Queens problem:

```sh
./Nqueens 
8
```
Output:
``` _ _ * _ _ _ _ 
_ _ _ _ _ _ * _ 
_ _ _ _ * _ _ _ 
_ _ * _ _ _ _ _ 
* _ _ _ _ _ _ _ 
_ _ _ _ _ * _ _ 
_ _ _ _ _ _ _ * 
_ * _ _ _ _ _ _ 
```
### Usage for big inputs
An input larger than 101 will result in an output which states the time it took for the algorithm to finish.
```sh
./Nqueens 
1000
```
Output:
```
Time taken by function: 0.016 seconds
```
## License

This project is licensed under the MIT License. See the LICENSE file for details.
