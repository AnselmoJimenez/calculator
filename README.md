# Calculator
> An algebraic calculator

## Outline
I chose to create this project to explore Extended BNF Notation for grammars. This formal grammar defines the syntax rules that govern input on standard algebraic calculators.  
The expression structure goes as follows:

```
// Expression Structure    
<expression> ::= <term> | <expression> "+" <term> | <expression> "-" <term>
<term> ::= <factor> | <term> "*" <factor> | <term> "/" <factor>
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

## Usage
To use this calculator, compile the program, execute the output binary, and input a valid expression you would like computed.  
Some examples of this could be:
- (2 + 2) * (3 + 3)
- 9^(4 * sin(90))
- sin(45) * cos(45)

