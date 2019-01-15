# Polygons

Program determines if polygons' convex hulls intersect.

### Program flow

Basic flow (no arguments):
1. Get points for first polygon
1. Get points for second polygon
2. Create convex hulls for both
3. Look for intersections

Alternative flow (--random <polygons>)
1. for <polygon> times
    1.1. Generate random points (in (-10, -10) to (10, 10) square)
    1.2. Create convex hull from points
2. Check all polygons intersections

### Algorithm

Polygons' intersections are checked as following:
1. Check if any point from polygon A is inside polygon B
    1.1. Is in -> A and B intersect. Return.
2. Check if any line segments of A intersect any line segment of B
    1.1. Intersect -> A and B intersect. Return.
    1.1. Otherwise -> A and B DON'T intersect. Return.

### Results:

When polygons's count exceeds around 80-100 elements, parallel implementation becomes more efficient.

Result example:

```
Total 5000:
Sequential computation took 7165 ms.
Intersections: 10663274
Parallel computation took 3437 ms.
Intersections: 10022803
Numeric instability: 0.0600633
```

Please note that results may differ due to numerical instability.