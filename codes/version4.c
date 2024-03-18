#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <stdbool.h>



// Function to display game rules
void rule(char m[],int n) {
    if(m[0] == '0' && m[1]=='\0') {
        printf("GAME RULES\n");
        printf("You will be placed at 1st position and you have to go to the last position in each level.\n");
        printf("If you lose before level 5 then you will have to restart the game from level 1.\n");
        printf("If you lose at level 5 or after level 5 then you will get to restart the game from level 5.\n");
        printf("Enter s to move Down\n");
        printf("Enter a to move Left\n");
        printf("Enter w to move Up\n");
        printf("Enter d to move Right\n");
    }
}

// Function to initialize the game board
void makeboard(char a[16][16], int l) {
    // Initialize all cells with '.'
    for(int i = 0; i < 16; i++)
        for(int j = 0; j < 16; j++)
            a[i][j] = '.';
    
    // Place Player 'P' at (0, 0) and End 'E' at (15, 15)
    a[0][0] = 'P';
    a[15][15] = 'E';
    
    srand(time(NULL)); // Initialize random seed
    int c = 0;

    // Place obstacles 'X' in <= 10*l random positions
    while(c < 10 * l) {
        int x = rand() % 16;
        int y = rand() % 16;
        if((x != 0 || y != 0) && (x != 15 || y != 15)) {
            a[x][y] = 'X';
            c++;
        }
    }
    c = 0;

    // Place traps '*' in <= 2*l random positions
    while(c < 2 * l) {
        int x = rand() % 16;
        int y = rand() % 16;
        if((x != 0 || y != 0) && (x != 15 || y != 15) && a[x][y] != 'X') {
            a[x][y] = '*';
            c++;
        }
    }
}

// Function to print the game board
void printboard(char a[16][16]) {
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            printf("%c ", a[i][j]);
        }
        printf("\n");
    }
}

// Structure to store player position
typedef struct position {
    int x;
    int y;
} pos;

// Function to check if there is a valid path to win the game
bool isValidPath(char a[16][16], pos s) {
    bool visited[16][16] = {false};
    int queue[256][2];
    int front = 0, rear = 0;

    // Add starting position to the queue
    queue[rear][0] = s.x;
    queue[rear][1] = s.y;
    rear++;

    while (front != rear) {
        int x = queue[front][0];
        int y = queue[front][1];
        front++;

        // Check if the current position is the end point
        if (x == 15 && y == 15) {
            return true;
        }

        // Check adjacent positions
        if (x > 0 && !visited[x - 1][y] && a[x - 1][y] != 'X' && a[x - 1][y] != '*') {
            queue[rear][0] = x - 1;
            queue[rear][1] = y;
            rear++;
            visited[x - 1][y] = true;
        }
        if (x < 15 && !visited[x + 1][y] && a[x + 1][y] != 'X' && a[x + 1][y] != '*') {
            queue[rear][0] = x + 1;
            queue[rear][1] = y;
            rear++;
            visited[x + 1][y] = true;
        }
        if (y > 0 && !visited[x][y - 1] && a[x][y - 1] != 'X' && a[x][y - 1] != '*') {
            queue[rear][0] = x;
            queue[rear][1] = y - 1;
            rear++;
            visited[x][y - 1] = true;
        }
        if (y < 15 && !visited[x][y + 1] && a[x][y + 1] != 'X' && a[x][y + 1] != '*') {
            queue[rear][0] = x;
            queue[rear][1] = y + 1;
            rear++;
            visited[x][y + 1] = true;
        }
    }

    return false;
}


// Function to regenerate the game board
void regenerateBoard(char a[16][16], int l, pos *s) {
    makeboard(a, l);
    s->x = 0;
    s->y = 0;
}

// Function to handle player movement
void movement(char a[16][16], char m[],int n, pos *s) {
    int u = s->x;
    int v = s->y;
    if((m[0] == 'a' || m[0]=='A') && m[1]=='\0')
        s->y = s->y - 1;
    else if((m[0] == 'd' || m[0] =='D') && m[1]=='\0')
        s->y = s->y + 1;
    else if((m[0] == 's' || m[0] =='S') && m[1]=='\0')
        s->x = s->x + 1;
    else if((m[0] == 'w' || m[0] =='W') && m[1]=='\0')
        s->x = s->x - 1;

    // Check boundaries and obstacles
    if(s->x < 0 || s->x >= 16 || s->y < 0 || s->y >= 16) {
        s->x = u;
        s->y = v;
    }
    // If position is valid, update player position
    else if(s->x >= 0 && s->x < 16 && s->y > -1 && s->y < 16 && a[s->x][s->y] != 'X' && a[s->x][s->y] != '*') {
        a[u][v] = '.';
        a[s->x][s->y] = 'P';
    }
    // If player reaches the end point
    else if(s->x == 15 && s->y == 15) {
        a[u][v] = '.';
        a[s->x][s->y] = 'P';
    }
    // If player encounters obstacle
    else if(a[s->x][s->y] == 'X') {
        s->x = u;
        s->y = v;
        printf("You are not allowed to use this position\n");
    }
}

// Main function
int main() {
    int l = 1;
    char m[100],bd[16][16];
    printf("For rules of the game press 0\n");
    makeboard(bd, l);
    pos step;
    step.x = 0;
    step.y = 0;
    pos *s;
    s = &step;
    
    // Main game loop
    do {
        if (!isValidPath(bd, *s)) {
            regenerateBoard(bd, l, s);
            continue;
        }
        printboard(bd);
    printf("Take your step : ");
        scanf(" %s", m);
        // Input validation loop
        do {
            if(m[0] == '0' && m[1]=='\0') {
                rule(m,100);
                scanf(" %s", m);
            }
            else if((m[0] != 's' && m[0] != 'a' && m[0] != 'd' && m[0] != 'w' && m[0] != 'S' && m[0] != 'A' && m[0] != 'D' && m[0] != 'W') || m[1]!='\0') {
                printf("Wrong Input! type 0 to know rules OR Retake your step\n");
                scanf(" %s", m);
            }
        } while(m[0] != 's' && m[0] != 'a' && m[0] != 'd' && m[0] != 'w' && m[0] != 'S' && m[0] != 'A' && m[0] != 'D' && m[0] != 'W' && m[1]!='\0');
        // Handle player movement
        movement(bd, m,100, s);

        // Check if player hits a trap and handle game over condition
        if(bd[s->x][s->y] == '*' && l < 5) {
            printf("You have lost the game \n Write R to start again from level 1 OR write Q if you are a QUITTER : ");
            char i[100];
            scanf("  %s", i);
            if((i[0] == 'R' || i[0] == 'r')&& i[1]=='\0') {
                printf("Go WARRIOR\n");
                makeboard(bd, 1);
                s->x = 0;
                s->y = 0;
                continue;
            }
            else {
                printf("Jazakallah Khair");
                exit(0);
            }
        }
        else if(bd[s->x][s->y] == '*' && l >= 5) {
            printf("You have lost the game \n Write R to start again from level 5 OR write Q if you are a QUITTER : ");
            char i[100];
            scanf("  %s", i);
            if((i[0] == 'R' || i[0] == 'r')&& i[1]=='\0') {
                printf("Go WARRIOR\n");
                makeboard(bd, 5);
                s->x = 0;
                s->y = 0;
                continue;
            }
            else {
                printf("Jazakallah Khair");
                exit(0);
            }
        }
        // Check if player reaches the end and handle level completion
        if(s->x == 15 && s->y == 15 && l < 7) {
            printf("Congratulations warrior you did it!\n");
            printf("Now let's go to next level and show who is the boss\n");
            printf("Write Y to go to next level or write anything I really don't like QUITTER : ");
            char i[100];
            scanf(" %s", i);
            if((i[0] == 'Y'|| i[0] == 'y') && i[1]=='\0') {
                l++;
                makeboard(bd, l);
                s->x = 0;
                s->y = 0;
                continue;
            }
            else {
                printf("Jazakallah Khair!");
                exit(0);
            }
        }
    } while(bd[15][15] != 'P' && bd[s->x][s->y] != '*' && l < 8);

    printf("HURRAY! You've conquered all the levels! Your determination and intelligence shine through brightly. Keep up the fantastic work, and stay tuned for the next challenge. You're a true superstar!");

    return 0;
}