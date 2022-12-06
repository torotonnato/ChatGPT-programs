#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

// Constants
#define BOARD_WIDTH  40
#define BOARD_HEIGHT 20
#define PADDLE_WIDTH  3
#define PADDLE_HEIGHT 1
#define BALL_SIZE     1
#define BALL_SPEED    1

// ANSI escape codes
#define ANSI_CURSOR_HIDE "\033[?25l"
#define ANSI_CURSOR_SHOW "\033[?25h"
#define ANSI_CLEAR_SCREEN "\033[2J"
#define ANSI_CURSOR_POS(row, col) "\033[%d;%dH", row, col

// Structs
typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point pos;
    int speed;
    int dx;
    int dy;
} Ball;

typedef struct {
    Point pos;
    int score;
} Player;

// Function declarations
void init_term();
void init_game(Ball *ball, Player *player1, Player *player2);
void update_ball(Ball *ball, Player *player1, Player *player2);
void update_player(Player *player, int up, int down);
void draw_board(Ball *ball, Player *player1, Player *player2);
void clear_board();

int main() {
    Ball ball;
    Player player1;
    Player player2;

    //NOTE: Added by TT
    //=============================================//
    init_term();
    //=============================================//

    // Initialize the game
    init_game(&ball, &player1, &player2);

    // Hide the cursor
    printf(ANSI_CURSOR_HIDE);

    // Main game loop
    while (1) {
        // Update the ball position
        update_ball(&ball, &player1, &player2);

        // Update player positions
        update_player(&player1, 'w', 's');
        update_player(&player2, 'i', 'k');

        // Draw the game board
        draw_board(&ball, &player1, &player2);

        // Sleep for a short time
        usleep(16000);
    }

    // Show the cursor
    printf(ANSI_CURSOR_SHOW);

    return 0;
}

// NOTE: Added by TT to aid kbd input
//=============================================//
static struct termios prev_ts, new_ts;

void init_term() {
    tcgetattr(STDIN_FILENO, &prev_ts);
    new_ts = prev_ts;
    new_ts.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_ts);
}
//=============================================//

// init_game initializes the ball and player positions
void init_game(Ball *ball, Player *player1, Player *player2) {
    // Set the ball starting position and direction
    ball->pos.x = BOARD_WIDTH / 2;
    ball->pos.y = BOARD_HEIGHT / 2;
    ball->dx = 1;
    ball->dy = 1;
    ball->speed = BALL_SPEED;

    // Set the player starting positions
    player1->pos.x = 1;
    player1->pos.y = BOARD_HEIGHT / 2;
    player1->score = 0;

    player2->pos.x = BOARD_WIDTH - PADDLE_WIDTH - 1;
    player2->pos.y = BOARD_HEIGHT / 2;
    player2->score = 0;
}

// update_ball updates the ball position based on its current position and direction
void update_ball(Ball *ball, Player *player1, Player *player2) {
    // Update the ball position
    ball->pos.x += ball->dx * ball->speed;
    ball->pos.y += ball->dy * ball->speed;
    // Check if the ball hits the top or bottom wall
    if (ball->pos.y <= 0 || ball->pos.y >= BOARD_HEIGHT - BALL_SIZE) {
        ball->dy *= -1;
    }

    // Check if the ball hits player1's paddle
    if (ball->pos.x <= PADDLE_WIDTH &&
        ball->pos.y >= player1->pos.y &&
        ball->pos.y <= player1->pos.y + PADDLE_HEIGHT) {
        ball->dx *= -1;
    }

    // Check if the ball hits player2's paddle
    if (ball->pos.x >= BOARD_WIDTH - PADDLE_WIDTH - BALL_SIZE &&
        ball->pos.y >= player2->pos.y &&
        ball->pos.y <= player2->pos.y + PADDLE_HEIGHT) {
        ball->dx *= -1;
    }

    // Check if the ball hits player1's wall
    if (ball->pos.x <= 0) {
        player2->score++;
        ball->pos.x = BOARD_WIDTH / 2;
        ball->pos.y = BOARD_HEIGHT / 2;
        ball->dx = 1;
        ball->dy = 1;
    }

    // Check if the ball hits player2's wall
    if (ball->pos.x >= BOARD_WIDTH - BALL_SIZE) {
        player1->score++;
        ball->pos.x = BOARD_WIDTH / 2;
        ball->pos.y = BOARD_HEIGHT / 2;
        ball->dx = -1;
        ball->dy = -1;
    }

}

// update_player updates the player position based on the specified up and down keys
void update_player(Player *player, int up, int down) {
    // Check if the up key is pressed
    if (getchar() == up) {
        // Move the paddle up
        player->pos.y--;
        // Check if the paddle hits the top wall
        if (player->pos.y < 0) {
            player->pos.y = 0;
        }
    }

    // Check if the down key is pressed
    if (getchar() == down) {
        // Move the paddle down
        player->pos.y++;

        // Check if the paddle hits the bottom wall
        if (player->pos.y > BOARD_HEIGHT - PADDLE_HEIGHT) {
            player->pos.y = BOARD_HEIGHT - PADDLE_HEIGHT;
        }
    }
}

// draw_board draws the game board in the terminal
void draw_board(Ball *ball, Player *player1, Player *player2) {
    // Clear the screen
    printf(ANSI_CLEAR_SCREEN);
    // Draw the ball
    printf(ANSI_CURSOR_POS(ball->pos.y, ball->pos.x));
    printf("O");

    // Draw player1's paddle
    int i;
    for (i = 0; i < PADDLE_HEIGHT; i++) {
        printf(ANSI_CURSOR_POS(player1->pos.y + i, player1->pos.x));
        printf("|");
    }

    // Draw player2's paddle
    for (i = 0; i < PADDLE_HEIGHT; i++) {
        printf(ANSI_CURSOR_POS(player2->pos.y + i, player2->pos.x));
        printf("|");
    }

    // Draw the scores
    printf(ANSI_CURSOR_POS(0, BOARD_WIDTH / 2 - 3));
    printf("%d - %d", player1->score, player2->score);
}

// clear_board clears the game board in the terminal
void clear_board() {
    // Clear the screen
    printf(ANSI_CLEAR_SCREEN);
    // Move the cursor to the top left corner
    printf(ANSI_CURSOR_POS(0, 0));
}
