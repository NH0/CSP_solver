# CSP_solver : A constraint programming solver

Implementation of a Constraint Satisfaction Problem (CSP) solver using backtracking and arc consistency.  
The solver was made for a class project of the Parisian Master of Operations Research (MPRO).  

## Problems solved
The solver contains class for the following problems :
- coloration
- queens

It is also possible to solve any binary CSP by defining all the variables, domains and constraints on the command-line interface (in french).

## Algorithms
The solver uses a backtracking algorithms, with many heuristics.  
For choosing a variable, the available heuristics are :
- largest domain
- smallest domain
- with most constraints
- with least constraints
- linked to previous variable
- random  

For choosing its value, the available heuristics are :
- largest
- smallest
- most supported (most present in constraints pairs)
- least supported
- most filtration (similar to least supported, with the difference that it counts the unique occurences of the value in the set of pairs of the constraints)
- fewest filtration
- random

## Remarks
The solver cannot handle two instances of a problem at the same time, due to some shared variables.  
Moreover, due to the representation of the constraints (by using sets of pairs of acceptable values), large problems use a lot of memory.
