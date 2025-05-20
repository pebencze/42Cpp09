# 42Cpp09
STL in CPP.

## Containers used in the exercises
1. map
2. stack
3. vector or deque (or unordered set)

## Ex03 - Merge-insertion sort (Ford-Johnson algorithm)
The [merge-insertion](https://en.wikipedia.org/wiki/Merge-insertion_sort) sorting algorithm was developed by Lester Ford and Selmer Johnson in 1959. It minimizes the amount of comparisions during sorting and therefore it has the best worst-case number of comparisions for a small list. It is a combination of mergeing and insertion. The insertion happens via binary search and is optimized by the [Jacobsthal numbers](https://en.wikipedia.org/wiki/Jacobsthal_number).

The key point is that it costs the same to perform binary search on a list of `N = 2^K` as on a list of `N = 2^(K+1)-1` => e.g. inserting element 8 has the same cost as inserting elements 9 - 15. Because Jacobsthal numbers grow more slowly than powers of two, they give a spacing that allows binary-like insertion but with fewer comparisons.
jacobsthal Sequence:
```bash
	J0​=0, J1​=1, Jn​=Jn−1​+2Jn−2
```

### Steps
1. Given an unsorted list, group the list into n/2 pairs.
- sort them by the biggest number (rightmost)
- refer to the biggest element as `a` and to the smallest as `b`
- depending on the index call them b1 a1 b2 a2 b3 a3 ...
- if there is an odd element, leave it at the end

2. Create two sequences:
- `main`: b1 a1 a2 a3 a4 a5 ... an
- `pend`: b2 b3 ... bn + odd element

3. Binary insertion based on Jacobsthal numbers
- insert from the `pend` into the `main`
- insert starting from Jacobsthal number e.g. 11 and going in reverse order until the previous Jacobsthal number (11 -> 10 ... 6)
- if no Jacobsthal numbers are left -> insert in reverse order
- divide the elementSize by 2 each time


=> step 1 is recursive and is combined with step 2 and 3

Description of the algorithm can be found in "Art Of Computer Programming, Vol.3." on pages 184f.

