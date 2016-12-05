# Yet another another PHP compiler
An implementation for a php java-like compiler for **Compiler Design** courses at **Damascus University**.   

# Getting Started
The final release of the project has the following structure:
* **src/lexer.l**: a lexer file which defines **php tokens**.
* **src/grammar.y**: a yacc/grammar file which defines **php syntax** and **AST-CG integration**.
* **src/AST**: an implementation for the **Abstract Syntax Tree**.
* **src/Code Generator**: an implementation for the **Code Generation** phase.
* **src/ErrorRecovery**: an implementation for error recovery tools.
* **src/SymbolTable**: an implementation for **Symbol Table**.
* **src/TypeSystem**: an implementation for the **Type System**.
* **src/DependencyGraph**: the structure of Dependency graph used in Syntactic-Semantic phases.
* **/samples**: the samples used in the demo of the project.

The first release of the project contains the following:
* **src/lexer.l**: a lexer file which defines **php tokens**.
* **src/grammar.y**: a yacc/grammar file which defines **php syntax**, **Symbol Table integration** and simple **Error Recovery rules**.
* **src/SymbolTable**: an implementation for **Symbol Table**.
* **src/DependencyGraph**: the structure of Dependency graph used in Syntactic phase.

# Versions
- The current project state is the last version developed at the end of the second semester.
- [The (60 Marks) version](https://github.com/yaapc/php_comp/archive/2.0.zip(Download))
- [The first semester version](https://github.com/yaapc/php_comp/archive/v1.0.zip(Download))

# Wiki
check the [Wiki](https://github.com/yaapc/php_comp/wiki) for some usefull stuff.

# Authors
* [Mohammed Ghanem](https://github.com/ghanem-mhd)
* [Mohammed Alow](https://github.com/m-alow)
* [M. Bassel Shmali](https://github.com/bass3l)

# License
GPL 2.0 - all books and articles are under copyrights of thier respectful owners.
