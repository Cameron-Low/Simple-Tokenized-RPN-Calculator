The calculator program evaluates an expression that is given to it.

It works by converting the expression string from infix notation to postfix notation using the Shunting Yard algorithm.
Then it evaluates this postfix expression to give a result.

Currently this program supports multiplication, division, addition, subtraction, exponentiation as well as brackets.

I hope to continue with this project next week. 
New features will include:
    Multi-digit numbers --- Completed
    Floating point numbers --- Completed
    Exponents --- Completed
    A lexer with useful error reporting --- Completed

The lexer converts the input expression into a token stream that is terminated by a null token.
This token stream can then be passed to the rpn converting function.
This function produces a second token stream that is now in rpn form.
Now the rpn expression can be evaluated.

I have also created a custom stack datatype that represents a stack of tokens. I may also improve on this slightly as well.
However this was not the main focus of the project, just a requirement of the algorithms.
