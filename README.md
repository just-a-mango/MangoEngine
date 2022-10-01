# Mango-Engine
A small ASCII graphics engine written in C++

## How it works
At startup, it generates a list of tuples which contain the x, y, and if enabled or not of each "pixel" in the terminal grid(which is 120x30). Then, it draws onto the "canvas" (the terminal) the contents of the list. There are multiple functions in the engine which help make the process of drawing and printing things onto the canvas easier:
```cpp
X=0, Y=0 -> top-left of the terminal canvas
- draw_pixel(int x, int y, int enabled)
- draw_line(int x1, int y1, int x2, int y2)
- draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3)
- draw_circle(int x, int y, int radius_x, int radius_y, bool fill) || WIP
- draw_ellipse(int x, int y, int radius_x, int radius_y, bool fill) || WIP
- draw_square(int x1, int y1, int x2, int y2, bool fill)
- draw_letter(char letter, int x, int y)
- draw_text(const basic_string<char>& text, int x, int y)
```
