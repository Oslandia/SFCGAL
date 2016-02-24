# Issue

SFCGAL actualy works with Epeck even if it's not required by algorithms.

# Ongoing refactoring (branch experimental_kernels)

The idea is to extend coordinate storage (```Coordinate::_storage```) to be able to store multiple kernel objects.

* [x] Explicit kernel usage in algorithm
* [x] Remove dangerous constructors (such as Coordinate(double,double)) (because Epick::FT == double)
* [x] Explicit convertion from coordinates to Point_2/Point_3 in order to prepare Epick introduction

Before :

```
Kernel::Point_2 a = point.toPoint_2();
```

After :

```
Kernel::Point_2<Epeck> a = point.toPoint_2<Epeck>();
```

* [x] Change coordinate storage to allow double (Epick) and exact (Epeck) number representation

Before :

```
boost::variant< Empty, Kernel::Point_2, Kernel::Point_3 >
```

After :

```
boost::variant< Empty, Epick::Point_2, Epick::Point_3, Epeck::Point_2, Epeck::Point_3 >
```

* [ ] Restore constructor from doubles (Epick::FT) and Epick::Point

Note : Clarify the role of std::is_finite controls before removing them


# Changes that could break behavior (i.e. regress-test)

* [ ] SLoriot : As missing Z is considered as zero, always store Point_3 and tag "is3D"?

Note : could break some low level unit tests on coordinate?

* [ ] Restore Coordinate/Point constructors from double as deprecated Epick constructors (tests checks that exceptions are thrown)
* [ ] Ensure that Coordinate creation from double creates Epick coordinates


```
boost::variant< Empty, Epick::Point_2, Epick::Point_3, Epeck::Point_2, Epeck::Point_3 >
```

* [ ] Use Epick when possible in algorithm and compare results
    * [ ] Point set triangulation
    * [ ] ...

Lead to questionning on the best way to deal with exact computation in SFCGAL. Should high level function produced clean results "rounded" to a precision model? How to do that?

* [ ] Use approxiate computation when it's possible (length, area, etc. computation) as proposed by Sebastien Loriot

* [ ] Replace Coordinate class by a typedef on ```boost::variant< Empty, Epick::Point_2, Epick::Point_3, Epeck::Point_2, Epeck::Point_3 >``` in order to externalize visitors?


# Changes that lead to massive changes

* [ ] depreciate SFCGAL::Kernel in order to review/explicit Epick/Epeck usage everywhere


# Perf checks

## Check overhead due to new variant variables

* Before Coordinate variant on Epeck/Epick

Running tests...
Test project /home/mickael/workspace_sfcgal/SFCGAL/Release
    Start 1: unit-test
1/3 Test #1: unit-test ........................   Passed   20.44 sec
    Start 2: standalone-regress-test
2/3 Test #2: standalone-regress-test ..........   Passed   79.15 sec
    Start 3: garden-test
3/3 Test #3: garden-test ......................   Passed  202.61 sec

* With Epick/Epeck as coordinate variant, without behavior change

Test project /home/mickael/workspace_sfcgal/SFCGAL/Release
    Start 1: unit-test
1/3 Test #1: unit-test ........................***Failed   20.09 sec
    Start 2: standalone-regress-test
2/3 Test #2: standalone-regress-test ..........   Passed   76.89 sec
    Start 3: garden-test
3/3 Test #3: garden-test ......................   Passed  206.86 sec

=> No significative change as expected
