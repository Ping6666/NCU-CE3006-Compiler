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

### `./test_data/*.lsp`, `./hidden_data/*.lsp`

LISP that need to process

ps. hidden_data is from demo on 2021/01/07

### withAST

this can only (fully) solve basic features Q1 ~ Q8 and bonus features Q1 ~ Q2.

#### Successful

##### Basic Features

| Feature              | Description                                      | Points | Public1 | Public2 | Hidden1 | Hidden2 |
|:-------------------- |:------------------------------------------------ |:------ |:------- |:------- |:------- |:------- |
| Syntax Validation    | Print “syntax error” when parsing invalid syntax | 10     | ◯       | ◯       | ◯       | ◯       |
| Print                | Implement print-num statement                    | 10     | ◯       | ◯       | ◯       | ◯       |
| Numerical Operations | Implement all numerical operations               | 25     | ◯       | ◯       | ◯       | ◯       |
| Logical Operations   | Implement all logical operations                 | 25     | ◯       | ◯       | ◯       | ◯       |
| if Expression        | Implement if expression                          | 8      | ◯       | ◯       | ◯       | ◯       |
| Variable Definition  | Able to define a variable                        | 8      | ◯       | ◯       | ◯       | ◯       |
| Function             | Able to declare and call an anonymous function   | 8      | ◯       | ◯       | ◯       | ◯       |
| Named Function       | Able to declare and call a named function        | 6      | ◯       | ◯       | ◯       | ◯       |

Public test case points : Hidden test case points = 80% : 20% (pass partial get partial)

##### Bonus Features

| Feature              | Description                             | Points | Public1 | Public2 | Hidden1 | Hidden2 |
|:-------------------- |:--------------------------------------- |:------ |:------- |:------- |:------- |:------- |
| Recursion            | Support recursive function call         | 5      | ◯       | ◯       | ◯       | ◯       |
| Type Checking        | Print error messages for type errors    | 5      | ◯       | ◯       | ◯       | ◯       |
| Nested Function      | Nested function                         | 5      | ◯       | ✕       | ◯       | ✕       |
| First-class Function | Able to pass functions, support closure | 5      | ✕       | ◯       | ✕       | ◯       |

Public test case points : Hidden test case points = 0% : 100% (pass all get all)

#### Fail

Q3 test case 2: Segmentation Fault in [ast.cpp#L167 std::string copy](./Final_Project/withAST/ast.cpp#L167)

Q4 test case 1: ids scope wrong

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
