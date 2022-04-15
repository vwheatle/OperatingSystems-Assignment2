V Wheatley  
Operating Systems  
Assignment 2

# Banker's Algorithm (for Deadlock Avoidance)

This consists of one program that implements the Banker's Algorithm with an example data set. This data set will be read from a file in the "space-separated value" format, which is a less practical and more silly version of the "comma-separated value" format supported by many applications.

## Requirements

Here's the requirements doc, converted to Markdown for easy reading within editors:

> In this programming assignment, you are required to provide the source codes, documents, and examples in detail on Github.
>
> Requirements:
> - You are required to use C/C++ in Linux/UNIX. Consider installing VirtualBox in your non-Linux environment.
> - Programs must succeed in compilation and execute as required. (80 points)
> - Readme, documentation, and example are all required. (20 points)
>
> ### Banker's Algorithm for deadlock avoidance.
>
> Consider a system with five processes numbered P0 to P4, and three resources enumerated A, B, and C. Resource A has 10 instances, Resource B has 5 instances, and Resource C has 7 instances. Suppose at time t0, the following snapshot of the system has been taken:

<table>
<thead>
<tr><th rowspan=2>Process<th colspan=3>Allocation<th><th colspan=3>Maximum
<tr><!--------><th>A<th>B<th>C<th><!----><th>A<th>B<th>C
<tbody>
<tr><th>P<sub>0<td>0<td>1<td>0<td><!----><td>7<td>5<td>3
<tr><th>P<sub>1<td>2<td>0<td>0<td><!----><td>3<td>2<td>2
<tr><th>P<sub>2<td>3<td>0<td>2<td><!----><td>9<td>0<td>2
<tr><th>P<sub>3<td>2<td>1<td>1<td><!----><td>2<td>2<td>2
<tr><th>P<sub>4<td>0<td>0<td>2<td><!----><td>4<td>3<td>3
</table>

<table>
<thead>
<tr><th colspan=3>Available<th><th colspan=3>Total
<tr><!----><th>A<th>B<th>C<th><!----><th>A<th>B<th>C
<tbody>
<tr><!----><td>3<td>3<td>2<td><!----><td>10<td>5<td>7
</table>

> Implement the Banker's algorithm to answer the following question: is the system in a safe state? If so, what's the safe sequence?
>
> Include these in your GitHub submission:
> - source code
> - an input file (readable by your code)
> - the safe sequence, printed by your code.
> - documentation and a README

## So what actually is the [Banker's Algorithm](https://en.wikipedia.org/wiki/Banker%27s_algorithm)?

uhhhhhhhhhhhhhhhhhhhhhhhhhh

So there's a matrix of processes and their resource usage. Processes have a current acquired resources and a wanted resources. You have to see if there's enough for at least 1 process to terminate satisfied, and if none are satisfied after one sweep through the rows, then this is not satisfiable and therefore unsafe.

## Your last project was in C! Why are you using C++ now?

C is good for interfacing with the Operating System. C++ is good for.. writing actual programs (okay, if I had my say then I'd be using Rust, but that's out of the question) and I need it for stuff like vectors and other good STL things. If I was still using C in this project, I'd have to malloc and free stuff, and I don't wanna bother with that.

## [Data](data1.txt) file format

Here's an example to check against, if you need:

```
Resources
Total
10 5 7

Allocated
0 1 0
2 0 0

Maximum
7 5 3
3 2 2
```

You define a resource at the very top, and you can either specify it in "total instances of the resource" *or* "remaining instances of the resource" with `Total` and `Available` respectively. Its position is significant, so you define A first, then B, and so on -- specifying a total amount of available instances for each resource.

I convert the resources' positions to letters at runtime. For example, if I wanted 3 As, 2 Bs, and 5 Cs, I'd write `3 2 5`. Their name is implied by their position.

The position significance continues with the `Allocated` and `Maximum` matrices, where the current line determines which process we're talking about. Obviously, both the Allocated and Maximum must match in length.
