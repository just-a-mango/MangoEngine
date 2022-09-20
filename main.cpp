#include <iostream>
#include <windows.h>
#include <vector>
#include <tuple>
#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

vector<tuple<int, int, int>> pixels;
char null = ' ';
char full = '+';
int canvas_width = 120;
int canvas_height = 30;
bool finished_init = false;


void refresh_canvas(int columns, int rows, char null_, char full_) {
    int lr_margin = (columns - canvas_width) / 2;
    int td_margin = (rows - canvas_height) / 2;
    system("cls");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (j+1 > lr_margin && j+1 < columns - lr_margin && i+1 > td_margin && i+1 < rows - td_margin) {
                if (get<2>(pixels[(i - td_margin) * canvas_width + (j - lr_margin)]) == 1) {
                    cout << full_;
                } else {
                    cout << null_;
                }
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

void force_refresh_canvas() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    refresh_canvas(columns, rows, null, full);
}

void draw_pixel(int x, int y, int enabled) {
    pixels[y * canvas_width + x] = make_tuple(x, y, enabled);
}

void remove_pixel(int x, int y) {
    pixels[y * canvas_width + x] = make_tuple(x, y, 0);
    force_refresh_canvas();
}

/**
 * If the line is more horizontal than vertical, then move in the x direction, otherwise move in the y direction.
 *
 * @param x1 The x coordinate of the first point
 * @param y1 The y coordinate of the first point
 * @param x2 The x coordinate of the end point of the line
 * @param y2 The y coordinate of the second point.
 */
void draw_line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;
    while (true) {
        draw_pixel(x1, y1, 1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

/**
 * It draws a triangle by drawing three lines
 *
 * @param x1 x coordinate of the first point
 * @param y1 y-coordinate of the first point
 * @param x2 x coordinate of the second point
 * @param y2 the y coordinate of the second point
 * @param x3 x coordinate of the first point
 * @param y3 the y coordinate of the third point
 */
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x3, y3);
    draw_line(x3, y3, x1, y1);
}

/**
 * It draws a rectangle around the canvas
 */
void draw_canvas_outline() {
    draw_line(0, 0, canvas_width-2, 0);
    draw_line(canvas_width-2, 0, canvas_width-2, canvas_height-2);
    draw_line(canvas_width-2, canvas_height-2, 0, canvas_height-2);
    draw_line(0, canvas_height-2, 0, 0);
    force_refresh_canvas();
}

void draw_circle(int x, int y, int radius_x, int radius_y, bool fill) {
    for (int i = 0; i < canvas_height; i++) {
        for (int j = 0; j < canvas_width; j++) {
            if (pow((j - x), 2) / pow(radius_x, 2) + pow((i - y), 2) / pow(radius_y, 2) <= 1) {
                if (fill) {
                    draw_pixel(j, i, 1);
                } else {
                    if (pow((j - x), 2) / pow(radius_x, 2) + pow((i - y), 2) / pow(radius_y, 2) <= 0.9) {
                        draw_pixel(j, i, 1);
                    }
                }
            }
        }
    }
    force_refresh_canvas();
}

void draw_ellipse(int x, int y, int radius_x, int radius_y, bool fill) {
    for (int i = 0; i < canvas_height; i++) {
        for (int j = 0; j < canvas_width; j++) {
            if (pow((j - x), 2) / pow(radius_x, 2) + pow((i - y), 2) / pow(radius_y, 2) <= 1) {
                if (fill) {
                    draw_pixel(j, i, 1);
                } else {
                    if (pow((j - x), 2) / pow(radius_x, 2) + pow((i - y), 2) / pow(radius_y, 2) <= 0.9) {
                        draw_pixel(j, i, 1);
                    }
                }
            }
        }
    }
    force_refresh_canvas();
}

void draw_rectangle(int x, int y, int width, int height, bool fill) {
    if (fill) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                draw_pixel(x + j, y + i, 1);
            }
        }
    } else {
        draw_line(x, y, x + width, y);
        draw_line(x + width, y, x + width, y + height);
        draw_line(x + width, y + height, x, y + height);
        draw_line(x, y + height, x, y);
    }
    force_refresh_canvas();
}

void draw_square(int x1, int y1, int x2, int y2, bool fill) {
    if (fill) {
        for (int i = x1; i < x2; i++) {
            for (int j = y1; j < y2+1; j++) {
                draw_pixel(i, j, 1);
            }
        }
    } else {
        draw_line(x1, y1, x2, y1);
        draw_line(x2, y1, x2, y2);
        draw_line(x2, y2, x1, y2);
        draw_line(x1, y2, x1, y1);
    }
    force_refresh_canvas();
}

/**
 * It draws a letter on the screen
 *
 * @param letter the letter to draw
 * @param x x coordinate of the top left corner of the letter
 * @param y the y coordinate of the top left corner of the letter
 *
 * @return The width of the letter.
 */
int draw_letter(char letter, int x, int y) {
    switch (letter) {
        case 'A':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x + 5, y, x + 5, y + 5);
            draw_line(x, y + 2, x + 5, y + 2);
            return 5;
        case 'B':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x, y + 2, x + 5, y + 2);
            draw_line(x, y + 5, x + 5, y + 5);
            draw_line(x + 5, y, x + 5, y + 2);
            draw_line(x + 5, y + 2, x + 2, y + 2);
            draw_line(x + 5, y + 2, x + 5, y + 5);
            return 5;
        case 'C':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x, y + 5, x + 5, y + 5);
            return 5;
        case 'D':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x, y + 5, x + 5, y + 5);
            draw_line(x + 5, y, x + 5, y + 5);
            draw_line(x+6, y+1, x+6, y+4);
            return 5;
        case 'E':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x, y + 2, x + 5, y + 2);
            draw_line(x, y + 5, x + 5, y + 5);
            return 5;
        case 'F':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x, y + 2, x + 5, y + 2);
            return 5;
        case 'G':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x, y + 5, x + 5, y + 5);
            draw_line(x + 5, y + 2, x + 5, y + 5);
            return 5;
        case 'H':
            draw_line(x, y, x, y + 5);
            draw_line(x, y + 2, x + 5, y + 2);
            draw_line(x + 5, y, x + 5, y + 5);
            return 5;
        case 'I':
            draw_line(x, y, x + 5, y);
            draw_line(x + 2, y, x + 2, y + 5);
            draw_line(x, y + 5, x + 5, y + 5);
            return 5;
        case 'J':
            draw_line(x, y, x + 5, y);
            draw_line(x + 3, y, x + 3, y + 5);
            draw_line(x + 3, y + 5, x, y + 5);
            draw_line(x, y+5, x, y + 3);
            return 5;
        case 'K':
            draw_line(x, y, x, y + 5);
            draw_line(x, y + 2, x + 5, y);
            draw_line(x, y + 2, x + 5, y + 5);
            return 5;
        case 'L':
            draw_line(x, y, x, y + 5);
            draw_line(x, y + 5, x + 5, y + 5);
            return 5;
        case 'M':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y + 2);
            draw_line(x + 5, y, x + 5, y + 5);
            return 5;
        case 'N':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y + 5);
            draw_line(x + 5, y, x + 5, y + 5);
            return 5;
        case 'O':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x, y + 5, x + 5, y + 5);
            draw_line(x + 5, y, x + 5, y + 5);
            return 5;
        case 'P':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x, y + 2, x + 5, y + 2);
            draw_line(x + 5, y, x + 5, y + 2);
            return 5;
        case 'Q':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x, y + 5, x + 5, y + 5);
            draw_line(x + 5, y, x + 5, y + 5);
            draw_line(x + 3, y + 3, x + 6, y + 6);
            return 6;
        case 'R':
            draw_line(x, y, x, y + 5);
            draw_line(x, y, x + 5, y);
            draw_line(x, y + 2, x + 5, y + 2);
            draw_line(x + 5, y, x + 5, y + 2);
            draw_line(x, y + 2, x + 5, y + 5);
            return 5;
        case 'S':
            draw_line(x, y, x + 5, y);
            draw_line(x, y, x, y + 2);
            draw_line(x, y + 2, x + 5, y + 2);
            draw_line(x + 5, y + 2, x + 5, y + 5);
            draw_line(x + 5, y + 5, x, y + 5);
            return 5;
        case 'T':
            draw_line(x, y, x + 5, y);
            draw_line(x + 2, y, x + 2, y + 5);
            return 5;
        case 'U':
            draw_line(x, y, x, y + 5);
            draw_line(x, y + 5, x + 5, y + 5);
            draw_line(x + 5, y, x + 5, y + 5);
            return 5;
        case 'V':
            draw_line(x, y, x + 2, y + 5);
            draw_line(x + 2, y + 5, x + 5, y);
            return 5;
        case 'W':
            draw_line(x, y, x, y + 5);
            draw_line(x, y + 5, x + 5, y + 2);
            draw_line(x + 5, y + 2, x + 10, y + 5);
            draw_line(x + 10, y, x + 10, y + 5);
            return 10;
        case 'X':
            draw_line(x, y, x + 5, y + 5);
            draw_line(x + 5, y, x, y + 5);
            return 5;
        case 'Y':
            draw_line(x, y, x + 5, y + 2);
            draw_line(x + 5, y, x + 5, y + 5);
            return 5;
        case 'Z':
            draw_line(x, y, x + 5, y);
            draw_line(x + 5, y, x, y + 5);
            draw_line(x, y + 5, x + 5, y + 5);
            return 5;
        case ' ': return 3;
        default: return 0;
    }
    force_refresh_canvas();
}

string print(const string& s) {
    cout << s << endl;
}

char wait_for_any_key()
{
    return getch();
}

/**
 * It draws a string of text on the screen
 *
 * @param text The text to draw.
 * @param x The x coordinate of the text
 * @param y The y coordinate of the text.
 */
void draw_text(const basic_string<char>& text, int x, int y)
{
    for (char i : text)
    {
        x += (draw_letter(toupper(i), x, y)+2);
    }
}

void init() {
    force_refresh_canvas();
    draw_canvas_outline();
//    draw_text(string("tuvwxyz"), 5, 5);
//    draw_ellipse(10, 10, 10, 10, true);
    draw_square(10, 5, 20, 15, true);
    draw_square(30, 5, 40, 15, true);
    draw_square(10, 20, 40, 21, true);
    //        draw_triangle(50, 5, 60, 5, 55, 15);
}

void tick() {
    int old_columns, old_rows;
    int is_init = 0;
    while(true) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        if (columns != old_columns || rows != old_rows) {
            refresh_canvas(columns, rows, null, full);
        }
        old_columns = columns;
        old_rows = rows;
        if (__builtin_expect(!is_init, 0)) {
            init();
            is_init = 1;
        }
    }
}

void init_engine() {
    std::ios_base::sync_with_stdio(false);
    for (int i = 0; i < canvas_height; i++) {
        for (int j = 0; j < canvas_width; j++) {
            pixels.emplace_back(i, j, 0);
        }
    }
    tick();
}

int main() {
    init_engine();
    return 0;
}
