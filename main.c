#include <stdio.h>
#include "robot.h"

// Función que muestra el tablero
void print_area() {
    for(int i = 0; i < N; i++, puts("\n"))
        for(int j = 0; j < N; j++)
            printf("\t%c", area[i][j]);
}

// Función que muestra el log
void print_log(char *log, int *count) {	
    printf("Log: ");
    for (int i = 0; i < *count; i++)
        printf(" %c", log[i]);
    printf("\nTotal de movimientos: %d\n", *count);
}

// Función que cambia los dos primeros valores de un array
void update_array(int *array, int value1, int value2) {
    array[0] += value1;
    array[1] += value2;
}

// Función para verificar si el robot puede avanzar a una casilla
int verify(int x, int y) {
    if (area[x][y] == '#' || x == N || y == N || x < 0 || y < 0 || area[x][y] == 'C')
        return 0;
    return 1;
}

// Función para elegir el siguiente movimiento
void choose_move(int *bot_pos, int *next_move, char *last_move, int *last_move_array) {
    int x = 0, y = 0;
    char aux = *last_move;

    if (*last_move != 'D' && verify(bot_pos[0] - 1, bot_pos[1])) // Ir Arriba
        x = -1;
    else if (*last_move != 'L' && verify(bot_pos[0], bot_pos[1] + 1)) // Ir a la Derecha
        y = 1;
    else if (*last_move != 'U' && verify(bot_pos[0] + 1, bot_pos[1])) // Ir Abajo
        x = 1;
    else if (*last_move != 'R' && verify(bot_pos[0], bot_pos[1] - 1)) // Ir a la Izquierda
        y = -1;
    
    *last_move = x == 1 ? 'D' : x == -1 ? 'U' : y == 1 ? 'R' : y == -1 ? 'L' : 'N';
    if (*last_move == 'N'){ // No queda ninguna zona no limpiada o sin caja para moverse
        *last_move = aux == 'D' ? 'U' : aux == 'U' ? 'D' : aux == 'L' ? 'R' : 'L';
        update_array(next_move, - last_move_array[0], - last_move_array[1]);
    }
    else
        update_array(next_move, x, y);
    update_array(last_move_array, - last_move_array[0] + x, - last_move_array[1] + y);
}

// Función que mueve al robot y verifica si se puede limpiar la casilla actual
void move_bot(int *bot_pos, int *next_move, int *cleaned, int *count) {
    bot_pos[0] += next_move[0];
    bot_pos[1] += next_move[1];
    if (area[bot_pos[0]][bot_pos[1]] == '.'){
        area[bot_pos[0]][bot_pos[1]] = 'C';
        *cleaned += 1;
    }
    ++*count;
}

int main(){
    char log[N * (N + 7)], last_move = 0;
    int bot_pos[2] = {0, 0}, last_move_array[2] = {0, 0}, next_move[2] = {0, 0}, cleaned = 1, total_squares = 0, count = 0;

    for(int i = 0; i < N; i++) // Se cuentan las casillas a limpiar
        for(int j = 0; j < N; j++)
            area[i][j] == '.' ? total_squares++ : 0;
    area[0][0] = 'C'; // Se marca como "limpia" la casilla de salida

    while(cleaned < total_squares){
        update_array(next_move, - next_move[0], - next_move[1]); // Reinicio el array del siguiente movimiento
        choose_move(bot_pos, next_move, &last_move, last_move_array);
        log[count] = last_move; // Actualizo el log
        move_bot(bot_pos, next_move, &cleaned, &count);
    }
    print_area();
    print_log(log, &count);
    return 0;
}