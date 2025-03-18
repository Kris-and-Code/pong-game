#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

#define WIDTH 60
#define HEIGHT 20
#define PADDLE_SIZE 4

// Game objects
struct {
    float x, y;
    float dx, dy;
} ball;

struct {
    int y;
} paddleLeft, paddleRight;

int scoreLeft = 0;
int scoreRight = 0;

void setup() {
    // Initialize ball position and direction
    ball.x = WIDTH / 2;
    ball.y = HEIGHT / 2;
    ball.dx = -0.5f;
    ball.dy = 0.2f;

    // Initialize paddle positions
    paddleLeft.y = HEIGHT / 2 - PADDLE_SIZE / 2;
    paddleRight.y = HEIGHT / 2 - PADDLE_SIZE / 2;
}

void draw() {
    system("cls");  // Clear screen

    // Draw top border
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    // Draw game area
    for (int y = 0; y < HEIGHT; y++) {
        printf("#"); // Left border
        for (int x = 0; x < WIDTH; x++) {
            if (x == (int)ball.x && y == (int)ball.y)
                printf("O"); // Ball
            else if (x == 1 && y >= paddleLeft.y && y < paddleLeft.y + PADDLE_SIZE)
                printf("|"); // Left paddle
            else if (x == WIDTH - 2 && y >= paddleRight.y && y < paddleRight.y + PADDLE_SIZE)
                printf("|"); // Right paddle
            else
                printf(" ");
        }
        printf("#\n"); // Right border
    }

    // Draw bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");

    // Draw score
    printf("Score: %d - %d\n", scoreLeft, scoreRight);
}

void update() {
    // Move ball
    ball.x += ball.dx;
    ball.y += ball.dy;

    // Ball collision with top and bottom walls
    if (ball.y <= 0 || ball.y >= HEIGHT - 1)
        ball.dy = -ball.dy;

    // Ball collision with paddles
    if (ball.x <= 2 && ball.y >= paddleLeft.y && ball.y < paddleLeft.y + PADDLE_SIZE) {
        ball.dx = -ball.dx;
        ball.x = 2;
    }
    if (ball.x >= WIDTH - 3 && ball.y >= paddleRight.y && ball.y < paddleRight.y + PADDLE_SIZE) {
        ball.dx = -ball.dx;
        ball.x = WIDTH - 3;
    }

    // Score points
    if (ball.x <= 0) {
        scoreRight++;
        ball.x = WIDTH / 2;
        ball.y = HEIGHT / 2;
    }
    if (ball.x >= WIDTH - 1) {
        scoreLeft++;
        ball.x = WIDTH / 2;
        ball.y = HEIGHT / 2;
    }

    // Get keyboard input
    if (_kbhit()) {
        char key = _getch();
        // Left paddle movement
        if (key == 'w' && paddleLeft.y > 0)
            paddleLeft.y--;
        if (key == 's' && paddleLeft.y < HEIGHT - PADDLE_SIZE)
            paddleLeft.y++;
        // Right paddle movement
        if (key == 'i' && paddleRight.y > 0)
            paddleRight.y--;
        if (key == 'k' && paddleRight.y < HEIGHT - PADDLE_SIZE)
            paddleRight.y++;
        // Quit game
        if (key == 'q')
            exit(0);
    }
}

int main() {
    setup();
    printf("Welcome to Pong!\n");
    printf("Controls:\n");
    printf("Left paddle: W/S\n");
    printf("Right paddle: I/K\n");
    printf("Press Q to quit\n");
    printf("Press any key to start...");
    _getch();

    while (1) {
        draw();
        update();
        Sleep(50); // Add a small delay
    }

    return 0;
} 