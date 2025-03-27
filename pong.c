#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>

#define WIDTH 60
#define HEIGHT 20
#define PADDLE_SIZE 4

// Colors
#define COLOR_RESET 7
#define COLOR_BORDER 11    // Light cyan
#define COLOR_PADDLE1 14   // Yellow
#define COLOR_PADDLE2 13   // Pink
#define COLOR_BALL 12      // Light red
#define COLOR_SCORE1 10    // Light green
#define COLOR_SCORE2 11    // Light cyan
#define COLOR_TEXT 15      // White

// Rainbow colors for ball
const int RAINBOW_COLORS[] = {12, 14, 10, 11, 13, 9};  // Red, Yellow, Green, Cyan, Pink, Blue
#define RAINBOW_SIZE (sizeof(RAINBOW_COLORS) / sizeof(RAINBOW_COLORS[0]))

// Game objects
struct {
    float x, y;
    float dx, dy;
    int colorIndex;
} ball;

struct {
    int y;
    int color;
} paddleLeft, paddleRight;

int scoreLeft = 0;
int scoreRight = 0;
int rainbowTimer = 0;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void setup() {
    // Initialize ball position and direction
    ball.x = WIDTH / 2;
    ball.y = HEIGHT / 2;
    ball.dx = -0.5f;
    ball.dy = 0.2f;
    ball.colorIndex = 0;

    // Initialize paddle positions and colors
    paddleLeft.y = HEIGHT / 2 - PADDLE_SIZE / 2;
    paddleRight.y = HEIGHT / 2 - PADDLE_SIZE / 2;
    paddleLeft.color = COLOR_PADDLE1;
    paddleRight.color = COLOR_PADDLE2;

    // Set up console
    hideCursor();
    system("mode con: cols=65 lines=25"); // Adjust console window size
}

void draw() {
    system("cls");  // Clear screen

    // Draw top border with gradient effect
    setColor(COLOR_BORDER);
    printf("╔");
    for (int i = 0; i < WIDTH; i++) {
        if (i % 2 == 0) setColor(COLOR_BORDER);
        else setColor(COLOR_BORDER + 1);
        printf("═");
    }
    setColor(COLOR_BORDER);
    printf("╗\n");

    // Draw game area
    for (int y = 0; y < HEIGHT; y++) {
        setColor(COLOR_BORDER);
        printf("║"); // Left border
        
        for (int x = 0; x < WIDTH; x++) {
            if (x == (int)ball.x && y == (int)ball.y) {
                setColor(RAINBOW_COLORS[ball.colorIndex]);
                printf("●"); // Ball
            }
            else if (x == 1 && y >= paddleLeft.y && y < paddleLeft.y + PADDLE_SIZE) {
                setColor(paddleLeft.color);
                printf("█"); // Left paddle
            }
            else if (x == WIDTH - 2 && y >= paddleRight.y && y < paddleRight.y + PADDLE_SIZE) {
                setColor(paddleRight.color);
                printf("█"); // Right paddle
            }
            else if (x == WIDTH / 2) {
                setColor(COLOR_BORDER);
                printf("┊"); // Center line
            }
            else {
                printf(" ");
            }
        }
        
        setColor(COLOR_BORDER);
        printf("║\n"); // Right border
    }

    // Draw bottom border with gradient effect
    setColor(COLOR_BORDER);
    printf("╚");
    for (int i = 0; i < WIDTH; i++) {
        if (i % 2 == 0) setColor(COLOR_BORDER);
        else setColor(COLOR_BORDER + 1);
        printf("═");
    }
    setColor(COLOR_BORDER);
    printf("╝\n");

    // Draw colorful score
    setColor(COLOR_SCORE1);
    printf("\n   ");
    printf("Player 1: ");
    setColor(COLOR_PADDLE1);
    printf("%d", scoreLeft);
    setColor(COLOR_SCORE2);
    printf("    Player 2: ");
    setColor(COLOR_PADDLE2);
    printf("%d\n", scoreRight);

    // Reset color
    setColor(COLOR_RESET);
}

void update() {
    // Move ball
    ball.x += ball.dx;
    ball.y += ball.dy;

    // Update rainbow effect
    rainbowTimer++;
    if (rainbowTimer >= 5) {  // Change color every 5 frames
        rainbowTimer = 0;
        ball.colorIndex = (ball.colorIndex + 1) % RAINBOW_SIZE;
    }

    // Ball collision with top and bottom walls
    if (ball.y <= 0 || ball.y >= HEIGHT - 1)
        ball.dy = -ball.dy;

    // Ball collision with paddles
    if (ball.x <= 2 && ball.y >= paddleLeft.y && ball.y < paddleLeft.y + PADDLE_SIZE) {
        ball.dx = -ball.dx;
        ball.x = 2;
        // Add slight randomization to ball direction
        ball.dy += ((float)rand() / RAND_MAX - 0.5f) * 0.2f;
        // Flash paddle color
        paddleLeft.color = COLOR_PADDLE1 + 1;
    }
    if (ball.x >= WIDTH - 3 && ball.y >= paddleRight.y && ball.y < paddleRight.y + PADDLE_SIZE) {
        ball.dx = -ball.dx;
        ball.x = WIDTH - 3;
        // Add slight randomization to ball direction
        ball.dy += ((float)rand() / RAND_MAX - 0.5f) * 0.2f;
        // Flash paddle color
        paddleRight.color = COLOR_PADDLE2 + 1;
    }

    // Reset paddle colors
    if (paddleLeft.color != COLOR_PADDLE1) paddleLeft.color = COLOR_PADDLE1;
    if (paddleRight.color != COLOR_PADDLE2) paddleRight.color = COLOR_PADDLE2;

    // Score points and reset ball
    if (ball.x <= 0) {
        scoreRight++;
        ball.x = WIDTH / 2;
        ball.y = HEIGHT / 2;
        ball.dx = 0.5f;
        ball.dy = ((float)rand() / RAND_MAX - 0.5f) * 0.5f;
    }
    if (ball.x >= WIDTH - 1) {
        scoreLeft++;
        ball.x = WIDTH / 2;
        ball.y = HEIGHT / 2;
        ball.dx = -0.5f;
        ball.dy = ((float)rand() / RAND_MAX - 0.5f) * 0.5f;
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

void showWelcomeScreen() {
    system("cls");
    setColor(COLOR_TEXT);
    printf("\n\n");
    printf("        ╔══════════════════════════════════╗\n");
    printf("        ║           PONG GAME              ║\n");
    printf("        ╚══════════════════════════════════╝\n\n");
    
    // Colorful controls display
    printf("                   ");
    setColor(COLOR_PADDLE1);
    printf("Controls:\n\n");
    setColor(COLOR_PADDLE1);
    printf("        Player 1:        ");
    setColor(COLOR_PADDLE2);
    printf("Player 2:\n");
    setColor(COLOR_PADDLE1);
    printf("        [W] Up          ");
    setColor(COLOR_PADDLE2);
    printf("[I] Up\n");
    setColor(COLOR_PADDLE1);
    printf("        [S] Down        ");
    setColor(COLOR_PADDLE2);
    printf("[K] Down\n\n");
    
    setColor(COLOR_TEXT);
    printf("        Press [Q] anytime to quit\n\n");
    printf("        Press any key to start...");
    _getch();
}

int main() {
    srand(time(NULL));  // Initialize random seed
    setup();
    showWelcomeScreen();

    while (1) {
        draw();
        update();
        Sleep(50); // Add a small delay
    }

    return 0;
} 