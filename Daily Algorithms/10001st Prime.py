import time

def find_neighbours(x_coord, y_coord):
    neighbours = 0
    for i in range(-1, 2):
        x = (x_coord + i) % width
        for j in range(-1, 2):
            if i == 0 and j == 0:
                continue
            y = (y_coord + j) % height
            if grid[y][x] == "#":
                neighbours += 1
    return neighbours

raw_data = """32 17 17
.................
.................
....###...###....
.................
..#....#.#....#..
..#....#.#....#..
..#....#.#....#..
....###...###....
.................
....###...###....
..#....#.#....#..
..#....#.#....#..
..#....#.#....#..
.................
....###...###....
.................
................."""

top_line, grid = raw_data.split("\n")[0], raw_data.split("\n")[1:]
iterations, width, height = [int(x) for x in top_line.split()]

for i in range(iterations):
    new_grid = []
    for y in range(height):
        new_line = []
        for x in range(width):
            num_neighbours = find_neighbours(x, y)
            if grid[y][x] == "." and num_neighbours == 3:
                new_line.append("#")
            elif grid[y][x] == "#" and num_neighbours < 2:
                new_line.append(".")
            elif grid[y][x] == "#" and num_neighbours > 3:
                new_line.append(".")
            else:
                new_line.append(grid[y][x])
        new_grid.append("".join(new_line))
    grid = new_grid
    print("\n"*15)
    print("\n".join(grid))
    time.sleep(0.5)

print("\n".join(grid))