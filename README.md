# game-of-life

Game of life toy project using SDL2

## Build
`gcc main.c -lSDL2`

## Ideas

### Command Line Interface

* Set speed
* Print usage
* Pass in a pre configured board
* User controlled stepping

### Color
* Select color of cells
* Instantiate cells with different colors on same board
* Merge colors when cells of different colors collide

### Initial condition editor

* Draw an inital board before starting
* Save board to data format (First iteration done)
* Load board from data format (First iteration done)
* Library of common game of life sprites
* Pack board tighter when serializing

### Different Neighbourhood Mechanisms
* Moore Neighbourhood has been implemented
* Generalize and try with differen neighbourhoods
