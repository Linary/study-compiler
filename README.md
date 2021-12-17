# study-compiler

变量声明的语法规则的形式化方法表达。
它的左边是一个非终结符（Non-terminal），右边是它的产生式（Production Rule）。
在语法解析的过程中，左边会被右边替代。如果替代之后还有非终结符，那么继续这个替代过程，直到最后全部都是终结符（Terminal），也就是 Token。
只有终结符才可以成为 AST 的叶子节点。这个过程，也叫做推导（Derivation）过程：

```
intDeclaration : Int Identifier ('=' additiveExpression)?;
```

你可以看到，int 类型变量的声明，需要有一个 Int 型的 Token，加一个变量标识符，后面跟一个可选的赋值表达式。

伪代码：

```
MatchIntDeclare() { 
    MatchToken(Int)； // 匹配 Int 关键字 
    MatchIdentifier(); // 匹配标识符 
    MatchToken(equal); // 匹配等号 
    MatchExpression(); // 匹配表达式
}
```

additiveExpression
    : IntLiteral 
    | additiveExpression Plus IntLiteral ;

在解析 2 + 3 这样一个最简单的加法表达式的时候，我们直观地将其翻译成算法，结果出现了如下的情况：

左递归的无穷递归是如何产生的？additive 可以被匹配为 “Int” 或者 “additive Plus Int”，有两种选择，
先选择第一种：匹配为 “Int” ，当前 token：2，是 Int，将此 token 消费后，发现 token 2 后面还有 token +，所以 additive 直接匹配为 “Int” 是失败的。
那么我们回溯，尝试将 additive 匹配为 “additive Plus Int”，因为发生回溯，所以已经消费的 token 2 再次变成当前 token，现在变成使用 “additive Plus Int” 匹配 tokens 了。
按照最左推导规则，从左到右依次将非终结符匹配为终结符，那么我们应该先匹配 “additive Plus Int” 中的非终结符”additive“。
至此又变成使用 "additive" 匹配 tokens。而 tokens 中也还没有任何一个 token 被消费呐（token：2 被回溯了不算已消费）。所以发生了无穷递归。这就是左递归的问题