Lexer & Parser
========================

## Regular Expressions Rules

* `digit` => `[0-9]`
* `digits` => `digit+`
* `number` => `digits(.digits)?(E[+-]?digits)?` => `n`
* `letter` => `[A-Z]`
* `id` => `letter(letter|digit)*` => `i`
* `declare` => `declare` => `d`
* `if` => `If` => `f`
* `=` => `=`
* `+` => `+`
* `-` => `-`
* `*` => `*`
* `/` => `/`
* `;` => `;`
* `)` => `)`
* `(` => `(`
* `>` => `>`
* `{` => `{`
* `}` => `}`

## Grammar Rules

* `Start` =>`Stmts $` >> `S`
* `Stmts` =>`AssignStmts | ifStmt` >> `T`
* `AssignStmts` =>`Stmt AssignStmts | Stmt` >> `A`
* `ifStmt` => `if (id > number) { AssignStmts }` >> `I`
* `Stmt` => `declare id = expression;` >> `M`
* `expression` => `expression + term | expression - term | term` >> `E`
* `term` => `term*factor | term/factor | factor` >> `R`
* `factor` => `(expression) | id | number` >> `F`

> `ifStmt` => `if (id > digit) { AssignStmts }` >> `I`

## Parser table

|   | n  | i  | f         | d     | "=" | +   | -   | *   | /   | ;  | (   | )  | { | }  | > | $  |
|---|----|----|-----------|-------|-----|-----|-----|-----|-----|----|-----|----|---|----|---|----|
| S |    |    | T$        | T$    |     |     |     |     |     |    |     |    |   |    |   |    |
| T |    |    | I         | A     |     |     |     |     |     |    |     |    |   |    |   |    |
| A |    |    |           | MB    |     |     |     |     |     |    |     |    |   |    |   |    |
| B |    |    |           | A     |     |     |     |     |     |    |     |    |   | \0 |   | \0 |
| I |    |    | f(i>n){A} |       |     |     |     |     |     |    |     |    |   |    |   |    |
| M |    |    |           | di=E; |     |     |     |     |     |    |     |    |   |    |   |    |
| E | RG | RG |           |       |     |     |     |     |     |    | RG  |    |   |    |   |    |
| G |    |    |           |       |     | +RG | -RG |     |     | \0 |     | \0 |   |    |   |    |
| R | FQ | FQ |           |       |     |     |     |     |     |    | FQ  |    |   |    |   |    |
| Q |    |    |           |       |     | \0  | \0  | *FQ | /FQ | \0 |     | \0 |   |    |   |    |
| F | n  | i  |           |       |     |     |     |     |     |    | (E) |    |   |    |   |    |
