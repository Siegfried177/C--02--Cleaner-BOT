#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "robot.h"

int count = 0;

// Función que muestra el tablero
void print_area() {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++)
            printf("\t%c", area[i][j]);
        printf("\n");
    }
}

// Función que muestra el log de las jugadas
void print_log(char *log) {
    for (int i = 0; i < count; i++)
        printf("  %c", log[i]);
}

// Función que actualiza el log
void update_log(char *log, char move) {
    log[count] = move;
    printf("%d", count++);
}

int main(){
    char log[N * (N + 7)];
    int bot_pos[2] = {0, 0};
    return 0;
}