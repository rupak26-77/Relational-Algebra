# Relational Algebra Interpreter in C++

## Project Overview
This project is a **Relational Algebra Interpreter** implemented in **C++**.  
It allows users to define relational tables and perform **Relational Algebra operations** such as:

- **Selection (σ)**  
- **Projection (π)**  
- **Union (∪)**  
- **Difference (−)**  
- **Cross Product (×)**  
- **Rename (ρ)**  

The program parses queries written in a **custom relational algebra syntax** and executes them on in-memory relations, simulating how queries are processed in databases.

---

## Features
- Define custom relations with attributes and datatypes  
- Insert tuples (rows) into relations  
- Perform **selection** with logical and comparison operators  
- Perform **projection** on specific attributes  
- Compute **union** and **set difference** between compatible relations  
- Compute **cross product** of two relations  
- Support for **renaming relations and attributes**  
- Displays query results in a tabular format  
- Handles invalid queries gracefully with error messages  

---

## Example Queries

### 1. Selection (σ)
```text
S [ pnum > 3 & dnum < 3 ] { proj }
```
_Selects all tuples from `proj` where `pnum > 3` and `dnum < 3`._

---

### 2. Projection (π)
```text
P [ ssn fname ] { employee }
```
_Projects only the `ssn` and `fname` attributes from `employee` relation._

---

### 3. Union (∪)
```text
U { employee } { employee }
```
_Returns the union of two `employee` relations._

---

### 4. Difference (−)
```text
D { proj } { works }
```
_Returns tuples in `proj` that are not in `works`._

---

### 5. Cross Product (×)
```text
C { employee } { dept }
```
_Returns the Cartesian product of `employee` and `dept`._

---

### 6. Rename (ρ)
```text
R [ works2 ] [ essn2 pno2 ] { works }
```
_Renames relation `works` as `works2` and attributes as `(essn2, pno2)`._

---

## Sample Output
```
ALL TABLES
    employee
fname            | ssn            
----------------------------------
a                | 1              
b                | 2              
c                | 3              
...

RESULTS

QUERY-2
pnum             | dnum           
----------------------------------
4                | 2              
5                | 3              
```

---

## How to Run

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/Relational-Algebra-Interpreter.git
   ```

2. Navigate to the folder:
   ```bash
   cd Relational-Algebra-Interpreter
   ```

3. Compile the program:
   ```bash
   g++ Relational_Algebra.cpp -o relational_algebra
   ```

4. Run the program:
   ```bash
   ./relational_algebra
   ```

---

## Project Structure
```
Relational_Algebra.cpp   # Main source code (interpreter + examples)
README.md                # Documentation file
```

---

## Learning Goals
This project is designed to:
- Understand how **Relational Algebra** forms the theoretical foundation of SQL  
- Learn parsing techniques for query interpretation  
- Implement database operations from scratch in C++  
- Strengthen knowledge of **Data Structures** (sets, maps, vectors)  

---

## Workflow Diagram
Below is a simple representation of how queries are processed:

```
 User Query  →  Parser  →  Relational Algebra Operations  →  Output Table
```

---

## Contribution
Contributions are welcome!  
- Fork the repo  
- Create a new branch  
- Commit your changes  
- Submit a Pull Request  

---

## License
This project is licensed under the **MIT License** – you are free to use, modify, and distribute it.

---
