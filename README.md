# Algebraic Calculator
> The classic first program everyone builds but with a twist

## Features
- Supports algebraic notation with implicit multiplication
- Built-in mathematical functions (sin, cos, tan, sqrt)
- Proper order of operations following mathematical conventions
- Handles negative numbers and parentheses correctly
- Recursive descent parser implementation

## Outline
I chose to create this project to explore **Extended BNF Notation** for grammars and **Recursive Descent Parsing** of strings. A recursive descent parser is a top-down parser that processes input based on a set of recursive functions, where each function corresponds to a grammar rule. It parses the input from left to right, constructing a parse tree by matching the grammar’s production rules.

Everyone creates a calculator to begin their programming journey, however I wanted my calculator to be one that understands algebraic notation. My calculator follows this grammar's production rules:

```
// Expression Structure    
<expression> ::= <term> | <expression> "+" <term> | <expression> "-" <term>
<term> ::= <factor> | <term> "*" <factor> | <term> "/" <factor> | <term> <factor>
<factor> ::= <power> | "-" <factor>
<power> ::= <atom> | <atom> "^" <factor>
<atom> ::= <number> | <function-call> | "(" <expression> ")"

// Numbers and Constants
<number> ::= <integer> | <decimal>
<integer> ::= <digit> | <integer> <digit>
<decimal> ::= <integer> "." <integer>
<digit> ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"

// Functions
<function-call> ::= <function-name> "(" <expression> ")"
<function-name> ::= "sin" | "cos" | "tan" | "log" | "ln" | "sqrt" | "abs" | "exp" | "floor" | "ceil"
```

After removing left-recursion and tokenization of the grammar, we are left with the following notation:
> ε (epsilon) means "Empty"

```
<expression> ::= <term> <expression-tail>
<expression-tail> ::= OPERATION(+) <term> <expression-tail> | OPERATION(-) <term> <expression-tail> | ε

<term> ::= <factor> <term-tail>
<term-tail> ::= OPERATION(*) <factor> <term-tail> | OPERATION(/) <factor> <term-tail> | <factor> <factor> | ε

<factor> ::= <power> | OPERATION(-) <factor>

<power> ::= <atom> <power-tail>
<power-tail> ::= EXPONENT <factor> | ε

<atom> ::= OPERAND | FUNCTION PARENTHESES(<) <expression> PARENTHESES(>) | PARENTHESES(<) <expression> PARENTHESES(>)
```

## Dependencies
* `gcc` (GNU Compiler Collection)
* `math.h` library (included with most C compilers)
* Linux/Unix-like environment (for compilation commands)

## Usage
To use this calculator follow the steps outlined below:
* Compile the program using `gcc ./source/main.c ./source/parser.c ./source/mylib.c -lm -Wno-builtin-declaration-mismatch`
* Execute the binary produced `./a.out`
* Finally, input a valid expression you would like computed. Some example may include:  
  - Basic arithmetic: `(2 + 2) * (3 + 3) = 24`
  - Exponents: `9^(4 * sin(90)) = 9^4 = 6561`
  - Trigonometry (In Radians): `sin(45)^2 * cos(45)^2 = 0.1998`
  - Implicit multiplication: `2sin(45) + 3cos(45) = 3.2778`
  - Nested functions: `sqrt(sin(45)^2 + cos(45)^2) = 1`

