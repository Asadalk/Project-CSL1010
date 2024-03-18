#include<stdio.h>
#include<stdlib.h>
#include<time.h>

// Function to display game rules
void rule(int m) {
    if(m == 0) {
        printf("GAME RULES\n");
        printf("You will be placed at 1st position and you have to go to the last position in each level.\n");
        printf("If you lose before level 5 then you will have to restart the game from level 1.\n");
        printf("If you lose at level 5 or after level 5 then you will get to restart the game from level 5.\n");
        printf("Enter 2 to move Down\n");
        printf("Enter 4 to move Left\n");
        printf("Enter 8 to move Up\n");
        printf("Enter 6 to move Right\n");
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
            printf("%c", a[i][j]);
        }
        printf("\n");
    }
}

// Structure to store player position
typedef struct position {
    int x;
    int y;
} pos;

// Function to handle player movement
void movement(char a[16][16], int m, pos *s) {
    int u = s->x;
    int v = s->y;
    if(m == 4)
        s->y = s->y - 1;
    else if(m == 6)
        s->y = s->y + 1;
    else if(m == 2)
        s->x = s->x + 1;
    else if(m == 8)
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
    int l = 1, m;
    char bd[16][16];
    printf("For rules of the game press 0\n");
    makeboard(bd, l);
    pos step;
    step.x = 0;
    step.y = 0;
    pos *s;
    s = &step;
    // Main game loop
    do {
        printboard(bd);
        printf("Take your step : ");
        scanf("%d", &m);
        // Input validation loop
        do {
            if(m == 0) {
                rule(0);
                scanf("%d", &m);
            }
            else if(m != 2 && m != 4 && m != 6 && m != 8) {
                printf("Wrong Input! type 0 to know rules OR Retake your step\n");
                scanf("%d", &m);
            }
        } while(m != 2 && m != 4 && m != 6 && m != 8);
        // Handle player movement
        movement(bd, m, s);

        // Check if player hits a trap and handle game over condition
        if(bd[s->x][s->y] == '*' && l < 5) {
            printf("You have lost the game \n Write R to start again from level 1 OR write Q if you are a QUITTER : ");
            char i;
            scanf("  %c", &i);
            if(i == 'R' || i == 'r') {
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
            char i;
            scanf("  %c", &i);
            if(i == 'R' || i == 'r') {
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
        if(s->x == 15 && s->y == 15 && l < 8) {
            printf("Congratulations warrior you did it!\n");
            printf("Now let's go to next level and show who is the boss\n");
            printf("Write Y to go to next level or write anything I really don't like QUITTER : ");
            char i;
            scanf(" %c", &i);
            if(i == 'Y'|| i == 'y') {
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
