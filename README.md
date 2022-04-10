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

## Your last project was in C! Why are you using C++ now?

C is good for interfacing with the Operating System. C++ is good for.. writing actual programs (okay, if I had my say then I'd be using Rust, but that's out of the question) and I need it for stuff like vectors and other good STL things. If I was still using C in this project, I'd have to malloc and free stuff, and I don't wanna bother with that.

## [Data](src/data.txt) file format

Okay, so this is indeed those space separated values. However, I also have some neat headers on them.

You define a resource at the very top. Its position is significant, so you define A first, then B, and so on -- specifying a total amount of available instances for each resource. I'll convert the positions to letters at runtime, it's fine. So if I wanted 3 As, 2 Bs, and 5 Cs, I'd write `3 2 5`. Their name is *implied by their position*.

Anyway, that's kinda an over-complicated explanation. I'll see how easy that is to parse later. The position significance continues with the Allocated and Maximum lists, where the current line determines which process we're talking about.

I think that's intuitive enough. I don't include "Available" in the format because you can derive that from summing the Allocated resources and subtracting that from the total resources.
