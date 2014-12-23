Lexer & Parser
========================

## Regular Expressions Rules

* `digit` => `[0-9]`
* `digits` => `digit+`
* `number` as **n** => `digits(.digits)?(E[+-]?digits)?`
* `letter` => `[A-Z]`
* `id` as **i** => `letter(letter|digit)*`
* `declare` as **d** => `declare`
* `if` as **f** => `If`
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

* `Start` as **S** => `Stmts $`
* `Stmts` as **T** => `AssignStmts | ifStmt`
* `AssignStmts` as **A** => `Stmt AssignStmts | Stmt`
* `ifStmt` as **I** => `if (id > number) { AssignStmts }`
* `Stmt` as **M** => `declare id = expression;`
* `expression` as **E** => `expression + term | expression - term | term`
* `term` as **R** => `term*factor | term/factor | factor`
* `factor` as **F** => `(expression) | id | number`

> Original: `ifStmt` => `if (id > digit) { AssignStmts }`

### Grammer Rule without left recursion and factoring

* `S` => `T$`
* `T` => `A` | `T`
* `A` => `MB`
* `B` => `A` | `\0`
* `I` => `f(i>n){A}`
* `M` => `di=E;`
* `E` => `RG`
* `G` => `+RG` | `-RG` | `\0`
* `R` => `FQ`
* `Q` => `*FQ` | `/FQ` | `\0`
* `F` => `(E)` | `i` | `n`

> `\0` means ϵ

### First Set

* `S` = { `d`,`f` }
* `T` = { `d`,`f` }
* `A` = { `d` }
* `B` = { `d`,`\0` }
* `I` = { `f` }
* `M` = { `d` }
* `E` = { `(`,`i`,`n` }
* `G` = { `+`,`-`,`\0` }
* `R` = { `(`,`i`,`n` }
* `Q` = { `*`,`/`,`\0` }
* `F` = { `(`,`i`,`n` }

> `\0` means ϵ

### Follow Set

* `S` = {  }
* `T` = { `$` }
* `A` = { `$` }
* `B` = { `$`,`}` }
* `I` = { `$` }
* `M` = { `d`,`$`,`}` }
* `E` = { `;`,`)` }
* `G` = { `;`,`)` }
* `R` = { `+`,`-`,`;`,`)` }
* `Q` = { `+`,`-`,`;`,`)` }
* `F` = { `*`,`/`,`+`,`-`,`;`,`)` }

> `\0` means ϵ

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
