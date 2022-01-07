# Compiler-Projects

110-01 NCU CE3006 Compiler

## HW1 & BA1

- Scanner
- Parser

## HW2 & BA2 & BA2_EX

- Lex, Regular Expression
- First Set
- Follow Set

## HW3 & BA3

- lex, yacc
- stack, matrix calculator, list

## Final Project

### test_data, hidden_data

LISP that need to process

ps. hidden_data is from demo on 2021/01/07

### withAST

this can only solve sub-problems Q1 ~ Q8 and advanced-problem-set Q1 ~ Q2.

#### Successful

##### Basic Features

| Feature              | Description                                      | Points |
|:-------------------- |:------------------------------------------------ |:------ |
| Syntax Validation    | Print “syntax error” when parsing invalid syntax | 10     |
| Print                | Implement print-num statement                    | 10     |
| Numerical Operations | Implement all numerical operations               | 25     |
| Logical Operations   | Implement all logical operations                 | 25     |
| if Expression        | Implement if expression                          | 8      |
| Variable Definition  | Able to define a variable                        | 8      |
| Function             | Able to declare and call an anonymous function   | 8      |
| Named Function       | Able to declare and call a named function        | 6      |

pass on both 2 public test cases (80%) and 2 hidden test cases (20%).

##### Bonus Features

| Feature              | Description                             | Points |
|:-------------------- |:--------------------------------------- |:------ |
| Recursion            | Support recursive function call         | 5      |
| Type Checking        | Print error messages for type errors    | 5      |
| Nested Function      | Nested function                         | 5      |
| First-class Function | Able to pass functions, support closure | 5      |

only pass on Q1's and Q2's 2 public test cases (0%) and 2 hidden test cases (100%).

and pass on Q3_1's and Q4_2's  public test cases (0%) and 2 hidden test cases (0%).

#### Fail

Q3_2 Segmentation Fault in [ast.cpp#L167 std::string copy](./Final_Project/withAST/ast.cpp#L167)

Q4_1 ids scope wrong

### withoutAST

this can only solve sub-problems Q1 ~ Q6.

see [withoutAST README.md](./Final_Project/withoutAST/README.md)

## Makefile

### `./Makefiles/lex/Makefile`

Makefile for lex

### `./Makefiles/yacc/Makefile`

Makefile for both lex and yacc

## License

plz follow MIT License
