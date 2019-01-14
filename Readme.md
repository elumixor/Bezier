## Introduction
This program is about creating 
[bezier](https://en.wikipedia.org/wiki/B%C3%A9zier_curve) intersections
curves and solving their intersections.

Further on, bezier curve will be referred to as `Bezier`


### Bezier curve representation
`Bezier` is represented in [polynomial form](https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Polynomial_form)
as an array of coefficients.
A generic curve is created from a two or more `Points` - 
generic objects, that support addition and float multiplication:
```c++
Point + Point -> Point
Point * float -> Point
```

### Constexpr
`Bezier` class supports constexpr.

### Program flow
Basic program flow is described as follows:
1. Get control points
2. (sequentially)
    1. Calculate bezier curves
    2. Calculate intersections
3. (parallel)
    1. Calculate bezier curves
    2. Calculate intersections
4. Print results and exit


## Usage and syntax
Launch program: `./Bezier` with following arguments:
1. `-help` - display information about how to use the program
2. `-test` - interactive mode to test the algorithm:
    1. Manually enter curves and points.
    2. Get a point at percentage.
    3. See if curves intersect. 
    
3. Random points generation:

`./Bezier -count n -min l -max k` 

where `n`, `l`, `k` are positive integer numbers and:
* `n > 0`,  (default `n = 1000`)
* `l > 1`,  (default `l = 2`)
* `k < 10`, (default `k = 6`)

Generates from `n * l` to `n * k` random_float control points, 
calculate polynomial coefficients and calculate intersections.

Either or all arguments can be omitted, i.e.:

`./Bezier`

is equal to

`./Bezier -count 1000 -min 2 -max 6`

### Measurement

// todo

