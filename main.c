#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGTH 25

void render(int rpX, int rpY, int lpX, int lpY, int ballY, int ballX) {
    for (int y = 0; y < HEIGTH; y++) {
        for (int x = 0; x <= WIDTH - 1; x++) {
            if (y == 0 || y == HEIGTH - 1) {
                printw("-");
            } else if (x == 0 || x == WIDTH - 1) {
                printw("|");
            } else {
                if ((x == rpX && (y == rpY || y == rpY + 1 || y == rpY + 2)) 
                    || (x == lpX && (y == lpY || y == lpY + 1 || y == lpY + 2))) {
                    printw("#");
                } else if (x == ballX && y == ballY) {
                    printw("*");
                } else {
                    printw(" ");
                }
            }
        }
        printw("\n");
    }
}

int ball_speedY(int ball_speed_y, int ballY) {
    if ((ballY == 1) || (ballY == HEIGTH - 2)) {
        ball_speed_y = -ball_speed_y;
    }
    return ball_speed_y;
}

int ball_speedX(int ball_speed_x, int ballX, int ballY, int lprX, int rprX, int lprY, int rprY) {
    if (((ballX == lprX + 1) && ((ballY == lprY) || (ballY == lprY + 1) || (ballY == lprY + 2))) 
        || ((ballX == rprX - 2) && ((ballY == rprY) || (ballY == rprY + 1) || (ballY == rprY + 2)))) {
        ball_speed_x = -ball_speed_x;
        return ball_speed_x;
    }
    if (ballX == 1) {
        return -10;
    }
    if (ballX == WIDTH - 1) {
        return 10;
    }

    return ball_speed_x;
}

int ball_X(int ballX, int ball_speed_x) {
    ballX += ball_speed_x;
    return ballX;
}

int ball_Y(int ballY, int ball_speed_y) {
    ballY += ball_speed_y;
    return ballY;
}

int r_move_up(int r_y) {
    if (r_y < HEIGTH - 4) {
        return r_y + 1;
    } else
        return r_y;
}

int r_move_down(int r_y) {
    if (r_y > 1) {
        return r_y - 1;
    } else
        return r_y;
}

int l_racketMove(char c, int lprY) {
    switch (c) {
        case 'A':
            lprY = r_move_down(lprY);
            return lprY;
        case 'Z':
            lprY = r_move_up(lprY);
            return lprY;
    }
    return lprY;
}

int r_racketMove(char c, int rprY) {
    switch (c) {
        case 'K':
            rprY = r_move_down(rprY);
            return rprY;
        case 'M':
            rprY = r_move_up(rprY);
            return rprY;
    }
    return rprY;
}

void winfunction(char c) { printf("%s%c%s", "Player ", c, " WIN!"); }

int main() {
    WINDOW *win = initscr();
    // Ball cords.
    int ballX = WIDTH / 2.0;
    int ballY = HEIGTH / 2.0;
    int ball_speed_x = 1;
    int ball_speed_y = 1;
    int score_l = 0;
    int score_r = 0;
    char p1 = '1';
    char p2 = '2';

    // Rockets cords.
    int lprX = 2, lprY = 11;
    int rprX = 77, rprY = 11;

    render(rprX, rprY, lprX, lprY, ballY, ballX);
    nodelay(win, TRUE);
    refresh();
    while (1) {
        usleep(100000);
        char c = getch();

        if (c == 'A' || c == 'Z') {
            lprY = l_racketMove(c, lprY);
        }
        if (c == 'K' || c == 'M') {
            rprY = r_racketMove(c, rprY);
        }
        ball_speed_x = ball_speedX(ball_speed_x, ballX, ballY, lprX, rprX, lprY, rprY);
        clear();
        render(rprX, rprY, lprX, lprY, ballY, ballX);

        refresh();

        if (ball_speed_x == 10) {
            score_l++;
            ball_speed_x = 1;
            ballX = WIDTH / 2.0;
            ballY = HEIGTH / 2.0;
        }
        if (ball_speed_x == -10) {
            score_r++;
            ball_speed_x = 1;
            ballX = WIDTH / 2.0;
            ballY = HEIGTH / 2.0;
        } else {
            ball_speed_y = ball_speedY(ball_speed_y, ballY);
            ballX = ball_X(ballX, ball_speed_x);
            ballY = ball_Y(ballY, ball_speed_y);
        }
        printw("Left player points %d\n", score_l);
        printw("Right player points %d\n", score_r);
        
        if (score_l == 21) {
            winfunction(p1);
            endwin();
            return 0;
        }
        if (score_r == 21) {
            winfunction(p2);
            endwin();
            return 0;
        }
    }

    return 0;
}