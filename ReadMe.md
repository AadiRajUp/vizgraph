# VIZGRAPH

VIZGRAPH is a C++ data-visualization project built from scratch using pixel-level drawing plus SFML rendering.  
It includes six chart types rendered on a shared 800×600 canvas:

- Line Graph
- Scatter Plot
- Pie Chart
- Donut Chart
- Bar Graph
- Histogram

The project was created as a graphics-focused academic exercise, so most primitives (line, circle, rectangle) are implemented manually in `utils.cpp`.

## Tech Stack

- C++17
- SFML 3.0.2 (`graphics`, `window`, `system`)

## Project Structure

- `main.cpp` — window/event loop, sample datasets, chart switching via number keys
- `utils.hpp` / `utils.cpp` — low-level drawing primitives and shared font
- `linegraph.*` — line graph plotting + point sorting by x-axis
- `scatterplot.*` — scatter plotting with axis scaling
- `piechart.*` — sector fill algorithm and legend/percentage labels
- `donutchart.*` — ring-sector fill algorithm and center total label
- `bargraph.*` — categorical bars with values and labels
- `histogram.*` — touching bins with frequency axis
- `Makefile` — build rules for executable `visualizer`

## Features

- Manual pixel drawing on `sf::Image`
- Overlay text rendering using `sf::RenderTexture`
- Auto-scaling axes for Cartesian charts
- Slice/bin/category labels for categorical charts
- Keyboard-driven chart switching at runtime

## Runtime Controls

Run the program and press:

- `1` → Line Graph
- `2` → Scatter Plot
- `3` → Pie Chart
- `4` → Donut Chart
- `5` → Bar Graph
- `6` → Histogram

## Build and Run

### 1) Install SFML

Install SFML development libraries for your platform (must provide headers and linkable libs for graphics/window/system).

### 2) Check include/link flags in `Makefile`

Current `Makefile` uses Windows-style paths:

- `-IC:/SFML/include`
- `-LC:/SFML/lib`

If you are on Linux, update `SFML_FLAGS` to your local setup (example with system-installed SFML):

```make
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
```

If headers are not in default path, also add the include directory in `CXXFLAGS`.

### 3) Build

```bash
make
```

### 4) Run

```bash
./visualizer
```

## Font Requirement

`utils.cpp` loads a font using:

```cpp
sf::Font font("font1.ttf");
```

Make sure `font1.ttf` is available in the working directory when running the executable, otherwise text labels may fail to render.

## Chart API Reference

Each chart uses the same core pattern:

- `sf::Image& canvas` for pixel drawing
- `sf::RenderTexture& rt` for text overlays
- input arrays + `sampleSize`

### Line Graph

```cpp
void linegraph(sf::Image& canvas, sf::RenderTexture& tx, int* x, int* y, int sampleSize);
```

- Sorts points by `x` before plotting (`sortforlinegraph`)
- Draws axes, points, connecting lines, and numeric axis labels

### Scatter Plot

```cpp
void scatterplot(sf::Image& canvas, sf::RenderTexture& tx, int* x, int* y, int sampleSize);
```

- Plots unconnected points
- Auto-scales x/y ranges to chart area

### Pie Chart

```cpp
void piechart(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize);
```

- Uses `draw_sector(...)` to fill wedges by angle
- Adds percentage labels and left-side legend

### Donut Chart

```cpp
void donutchart(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize);
```

- Uses `draw_donut_sector(...)` between inner/outer radii
- Includes center `Total` text + percentage labels + legend

### Bar Graph

```cpp
void bargraph(sf::Image& canvas, sf::RenderTexture& rt, int* values, std::string* labels, int sampleSize);
```

- Categorical bars with value text above and label below each bar
- Y-axis ticks scaled to nearest power-of-10 bucket

### Histogram

```cpp
void histogram(sf::Image& canvas, sf::RenderTexture& rt, int* counts, std::string* binLabels, int sampleSize);
```

- Adjacent bars representing bin frequencies
- Bin labels on x-axis and scaled count axis on y-axis

## Utility Primitives

From `utils.hpp`:

```cpp
void draw_line(sf::Image& canvas, int x0, int y0, int x1, int y1, sf::Color color);
void draw_circle(sf::Image& canvas, int cx, int cy, int r, sf::Color color, bool fill = false);
void draw_rect(sf::Image& canvas, int x, int y, int w, int h, sf::Color color, bool fill = false);
void flood_fill(sf::Image& canvas, int x, int y, sf::Color fill_color, sf::Color target_color);
extern sf::Font font;
```

Implemented algorithms include:

- Bresenham-style line rasterization
- Midpoint-style circle drawing (outline/fill)
- Filled/outlined rectangle drawing

## Known Notes

- Cartesian chart scaling is based on powers of 10 and may be coarse for some datasets.
- `linegraph` sorts input arrays in place, so original x/y ordering is mutated.
- Negative values are clipped to 0 in bar/histogram rendering.

## How to Add a New Chart

1. Create `newchart.hpp/.cpp` with function signature compatible with current pattern.
2. Include it in `main.cpp`.
3. Add source file to `SRCS` in `Makefile`.
4. Add a keybinding branch in the `currentChart` switch logic.

## License / Usage

No explicit license file is included in this repository. If this project is to be shared publicly, add a license file (e.g., MIT) to clarify reuse terms.

