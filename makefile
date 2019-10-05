all: build/pixel.o build/pathfinding.o build/hexa_grid.o build/main.o build/interactions.o
	gcc -o hexa $^ -lSDL2 -lm -lSDL2_image

# ==============================================================================
build/pixel.o: pixel.c pixel.h
	gcc -c pixel.c -o $@
build/pathfinding.o: pathfinding_hex.c pathfinding_hex.h
	gcc -c pathfinding_hex.c -o $@
build/hexa_grid.o: HexagonalGrid.c HexagonalGrid.h
	gcc -c HexagonalGrid.c -o $@
build/main.o: main.c
	gcc -c main.c -o $@
build/interactions.o: grid_interactions.c grid_interactions.h
	gcc -c grid_interactions.c -o $@

# ==============================================================================
clean:
	rm build/*.o
	rm hexa
