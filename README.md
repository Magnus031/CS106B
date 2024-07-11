# CS106B

建议时长 可能两天allin可以做完全部
> For reference only Do not Copy!

- CourseWeb

https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1228/#

由于代码Stanford是不提供的，所以我给了一个GitHub的仓库链接，里面有代码框架可以练手
https://github.com/BigWigImposter/CS106B-X-CS106L-
记得先初始化Library-Stanford2021

## Have Finished:

### Assignment1: Getting your  C++ Legs
- Perfect Numbers
- Soundex Search

> Assignment 1 I used **Stanford-lib-2023**!So it may cannot run and execute in this environment! 

### Assignment2: Fun with Collections

> Warning! This assignments can only execute in **Stanford-Lib2021**
> 
> The code can be referenced without any issues, but it cannot run in your 2023 environment
> 

- Warmup ! Learn to debug
- Maze

> 这个迷宫的实现可以有很多种方式，我采用的是最朴素的DFS 
- Search Engine
> 课上ADS讲了倒排索引，这次算是真正的尝试了一下 虽然是mini版的 但很妙！

I think Assignment2 is more interesting than 1!

### Assignment3:    Recursive Problem Solving

#### Part1: 
- Fundamental Recursion Warmup
- Balanced Operators
- Sierpinski Fractal
- Merging Sorted Sequences

>这几个练习可以很好的巩固一下递归算法，基本上做完之后都会比较明了
#### part2:
- Recursive Backtracking Warmup
- Scoring Boggle 

> 这个找word游戏很有趣 可以巩固一下DFS的使用
- Examining Voting Systems
> 这个题很有必要做一下，实现起来很简单，但是重在优化，如果是常规思路，有个测试会一直超时
因为这会遇到一个问题是： `给定一个set，需要你遍历所以的subset` 所以他的时间复杂度 $O(2^N*N)$ 是很麻烦 
所以需要回溯来优化 但事实上 我用回溯也跑了90s...

### Assignment4:
暂未找到相关的代码框架...GitHub上都是别人的solutions 懒得删掉重写了

### Assignment5:
- Memory Debugging Warmup!
> 这个part一定要做！会让你对指针的Memory Leak有别样的理解
- Labyrinth 送分题
- Sorting Linked List 

> 用链表实现快速排序 我感觉可以但没必要 用数组就行了
