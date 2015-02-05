# basya-compiler
A primitive, unfinished compiler project started in 2nd course in university.
It translates source code written in a C-like language into x86 assembly.
Only basic expressions and some statements are implemented.

The parser transforms the source code into an AST, from which an IR (Intermediate Representation) tree is built
(a binary tree with opcode and arguments).

The code generator is based on the paper "Engineering a Simple, Efficient Code Generator Generator" by C. W. Fraser, D. R. Hanson, and T. A. Proebsting (1992).
Basically, it traverses the IR tree and attempts to cover/tile it with 'lowest-cost' instructions.
(The code generator itself is automatically generated from a machine description.)
