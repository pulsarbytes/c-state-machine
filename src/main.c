/*
 * C State Machine
 * A simple State machine written in C, using ncurses
 *
 * v0.1
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 only,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (c) 2023 Yannis Maragos
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

#define FPS 24
#define LINECHARS 40
#define COLUMNWIDTH 5

// States enumeration
enum
{
    OFF,
    ON,
    MODE1,
    MODE2,
    SHUTDOWN
};

void off(void);
void on(void);
void mode1(void);
void mode2(void);
void shutdown(void);
void header(void);

// Initialize state to OFF
int state = OFF;

// Keep track of loop iterations
int iteration = 0;

int main(void)
{
    // Initialize ncurses
    initscr();

    // Define colors
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);

    // Set non-blocking mode
    nodelay(stdscr, TRUE);

    // Turn off echo, so user input is not displayed to the screen
    noecho();

    // A pointer to a function
    void (*fn)(void);

    // Array of pointers to functions
    void (*states[])(void) = {
        off,
        on,
        mode1,
        mode2,
        shutdown};

    // Calculate the time per frame in milliseconds
    const int frameTime = 1000 / FPS;

    // Get the time
    time_t previousTime = time(NULL);

    while (1)
    {
        iteration++;

        // Use a fixed time step approach to limit frames
        time_t currentTime = time(NULL);
        long elapsedTime = (currentTime - previousTime) * 1000;

        while (elapsedTime >= frameTime)
        {
            // Update game state (fixed time step)
            elapsedTime -= frameTime;

            previousTime = currentTime;
        }

        header();

        // Reassign fn to a new pointer to a function
        fn = states[state];

        fn();

        usleep((frameTime - elapsedTime) * 1000);
    }

    return 0;
}

void header(void)
{
    erase();

    printw("\n");
    attron(COLOR_PAIR(COLOR_BLACK));
    attron(A_DIM);
    printw("C State Machine\n");
    attrset(A_NORMAL);

    for (int i = 0; i < LINECHARS; i++)
    {
        attron(COLOR_PAIR(COLOR_BLACK));
        attron(A_DIM);
        printw("-");
        attrset(A_NORMAL);
    }

    printw("\n");
}

void off(void)
{
    printw("\n");
    printw("%*s", LINECHARS, "Machine is off\n");
    printw("\n");

    for (int i = 0; i < LINECHARS; i++)
    {
        attron(COLOR_PAIR(COLOR_BLACK));
        attron(A_DIM);
        printw("-");
        attrset(A_NORMAL);
    }

    printw("\n");
    printw("\n");
    printw("%-*s %*s", COLUMNWIDTH, "(i)", LINECHARS - COLUMNWIDTH, "Turn on\n");
    printw("%-*s %*s", COLUMNWIDTH, "(x)", LINECHARS - COLUMNWIDTH, "Exit\n");
    printw("\n");

    for (int i = 0; i < LINECHARS; i++)
    {
        attron(COLOR_PAIR(COLOR_BLACK));
        attron(A_DIM);
        printw("-");
        attrset(A_NORMAL);
    }

    printw("\n");

    refresh();

    // Set the timeout for the getch() function
    timeout(0);

    // Get user input
    int c = getch();

    if (c != ERR)
    {
        c = tolower(c);

        switch (c)
        {
        case 'i':
            state = ON;
            break;
        case 'x':
            state = SHUTDOWN;
            break;
        default:
            state = OFF;
        }
    }
}

void on(void)
{
    printw("\n");
    printw("%*s", LINECHARS, "Machine is running in Idle mode\n");
    printw("\n");

    int position = iteration % LINECHARS;

    for (int i = 0; i < position; i++)
    {
        attrset(COLOR_PAIR(1));
        printw("-");
        attrset(A_NORMAL);
    }

    for (int j = position; j < LINECHARS; j++)
    {
        attron(COLOR_PAIR(COLOR_BLACK));
        attron(A_DIM);
        printw("-");
        attrset(A_NORMAL);
    }

    printw("\n");
    printw("\n");
    attrset(COLOR_PAIR(1));
    printw("%-*s %*s", COLUMNWIDTH, "(i)", LINECHARS - COLUMNWIDTH, "Idle mode\n");
    attrset(A_NORMAL);
    printw("%-*s %*s", COLUMNWIDTH, "(1)", LINECHARS - COLUMNWIDTH, "Mode 1\n");
    printw("%-*s %*s", COLUMNWIDTH, "(2)", LINECHARS - COLUMNWIDTH, "Mode 2\n");
    printw("%-*s %*s", COLUMNWIDTH, "(0)", LINECHARS - COLUMNWIDTH, "Turn off\n");
    printw("\n");

    for (int i = 0; i < LINECHARS; i++)
    {
        attron(COLOR_PAIR(COLOR_BLACK));
        attron(A_DIM);
        printw("-");
        attrset(A_NORMAL);
    }

    printw("\n");

    refresh();

    // Set the timeout for the getch() function
    timeout(0);

    // Get user input
    int c = getch();

    if (c != ERR)
    {
        switch (c)
        {
        case '1':
            state = MODE1;
            break;
        case '2':
            state = MODE2;
            break;
        case '0':
            state = OFF;
            break;
        default:
            state = ON;
        }
    }
}

void mode1(void)
{
    printw("\n");
    printw("%*s", LINECHARS, "Machine is running in Mode 1\n");
    printw("\n");

    int position = iteration % LINECHARS;

    for (int i = 0; i < position; i++)
    {
        attrset(COLOR_PAIR(2));
        printw("-");
        attrset(A_NORMAL);
    }

    for (int j = position; j < LINECHARS; j++)
    {
        attron(COLOR_PAIR(COLOR_BLACK));
        attron(A_DIM);
        printw("-");
        attrset(A_NORMAL);
    }

    printw("\n");
    printw("\n");
    printw("%-*s %*s", COLUMNWIDTH, "(i)", LINECHARS - COLUMNWIDTH, "Idle mode\n");
    attrset(COLOR_PAIR(2));
    printw("%-*s %*s", COLUMNWIDTH, "(1)", LINECHARS - COLUMNWIDTH, "Mode 1\n");
    attrset(A_NORMAL);
    printw("%-*s %*s", COLUMNWIDTH, "(2)", LINECHARS - COLUMNWIDTH, "Mode 2\n");
    printw("%-*s %*s", COLUMNWIDTH, "(0)", LINECHARS - COLUMNWIDTH, "Turn off\n");
    printw("\n");

    for (int i = 0; i < LINECHARS; i++)
    {
        attron(COLOR_PAIR(COLOR_BLACK));
        attron(A_DIM);
        printw("-");
        attrset(A_NORMAL);
    }

    printw("\n");

    refresh();

    // Set the timeout for the getch() function
    timeout(0);

    // Get user input
    int c = getch();

    if (c != ERR)
    {
        switch (c)
        {
        case 'i':
            state = ON;
            break;
        case '2':
            state = MODE2;
            break;
        case '0':
            state = OFF;
            break;
        default:
            state = MODE1;
        }
    }
}

void mode2(void)
{
    printw("\n");
    printw("%*s", LINECHARS, "Machine is running in Mode 2\n");
    printw("\n");

    int position = iteration % LINECHARS;

    for (int i = 0; i < position; i++)
    {
        attrset(COLOR_PAIR(3));
        printw("-");
        attrset(A_NORMAL);
    }

    for (int j = position; j < LINECHARS; j++)
    {
        attron(COLOR_PAIR(COLOR_BLACK));
        attron(A_DIM);
        printw("-");
        attrset(A_NORMAL);
    }

    printw("\n");
    printw("\n");
    printw("%-*s %*s", COLUMNWIDTH, "(i)", LINECHARS - COLUMNWIDTH, "Idle mode\n");
    printw("%-*s %*s", COLUMNWIDTH, "(1)", LINECHARS - COLUMNWIDTH, "Mode 1\n");
    attrset(COLOR_PAIR(3));
    printw("%-*s %*s", COLUMNWIDTH, "(2)", LINECHARS - COLUMNWIDTH, "Mode 2\n");
    attrset(A_NORMAL);
    printw("%-*s %*s", COLUMNWIDTH, "(0)", LINECHARS - COLUMNWIDTH, "Turn off\n");
    printw("\n");

    for (int i = 0; i < LINECHARS; i++)
    {
        attron(COLOR_PAIR(COLOR_BLACK));
        attron(A_DIM);
        printw("-");
        attrset(A_NORMAL);
    }

    printw("\n");

    refresh();

    // Set the timeout for the getch() function
    timeout(0);

    // Get user input
    int c = getch();

    if (c != ERR)
    {
        switch (c)
        {
        case 'i':
            state = ON;
            break;
        case '1':
            state = MODE1;
            break;
        case '0':
            state = OFF;
            break;
        default:
            state = MODE2;
        }
    }
}

void shutdown(void)
{
    // End ncurses
    endwin();

    printf("Program terminated\n\n");

    exit(0);
}