#include <stdio.h>
#include "robot.h"

int count = 0, count2 = 0, back = 0; // count = total de movimientos, count2 = total de movimientos sin contar retrocesos y back es 1 si el bot ha retrocedido y 0 si no
char log_moves[TAM_AREA * (TAM_AREA + 7)], log_2[TAM_AREA * (TAM_AREA + 7)]; // log_moves = registro de movimientos, log_2 = registro de movimientos sin contar retrocesos

// Función que muestra el registro de los movimientos
// Parametros: void
// Retorno: void
void print_log() {
    for (int i = 0; i < count; i++)
        printf(" %c", log_moves[i]);
    printf("\nTotal de movimientos: %d\n", count);
}

// Función que cambia los dos primeros valores de un array
// Parametros: Un array de enteros y dos enteros que serán los nuevos valores del array
// Retorno: void
void update_array(int *array, int value1, int value2) {
    array[0] = value1;
    array[1] = value2;
}

// Función para verificar si el robot puede avanzar a una casilla
// Parametros: Las coordenadas de la casilla a la que se intenta mover el robot y la dirección (U, D, L, R)
// Retorno: 1 si se puede avanzar a esa casilla o 0 si no se puede
int verify(int x, int y, char next_move) {
    if (x >= TAM_AREA || y >= TAM_AREA || x < 0 || y < 0 || area[x][y] == '#' || area[x][y] == 'C') // Se evita que el bot se mueva a una casilla que no existe, haya sido limpiada o que sea una caja
        return 0;
    if (count >= 3) // Se verifica la secuencia de movimientos para que no se repita más de 3 veces seguidas una dirección
        if (log_moves[count - 3] == log_moves[count - 2] && log_moves[count - 2] == log_moves[count - 1] && log_moves[count - 1] == next_move) 
        return 0;
    return 1;
}

// Función para retroceder el robot
// Parametros: El char que representa el último movimiento y dos punteros de enteros que serán los valores de la dirección del próximo movimiento
// Retorno: void
void move_back(char *last_move, int *x, int *y) {
    static int index;
    back = 1; // Se indica que el bot va a retroceder
    index = back = 1 ? 1 : index;
    char aux2 = log_2[count2 - index]; // Movimiento a la que el bot va a retroceder
    
    // El bot se mueve en la dirección opuesta a la que indique el log_2
    if (aux2 == 'D') // Retroceder hacia arriba
        *x = -1;
    else if (aux2 == 'U') // Retroceder hacia abajo
        *x = 1;
    else if (aux2 == 'L') // Retroceder hacia la derecha
        *y = 1;
    else // Retroceder hacia la izquierda
        *y = -1;

    index++;
    *last_move = *x == 1 ? 'D' : *x == -1 ? 'U' : *y == 1 ? 'R' : *y == -1 ? 'L' : 0; // Se actualiza el char de ultimo movimiento
}

// Función para elegir el siguiente movimiento
// Parametros: La posición actual del robot, el movimiento elegido y el char que representa el último movimiento
// Retorno: void
void choose_move(int *bot_pos, int *next_move, char *last_move) {
    int x = 0, y = 0; // x e y representan la dirección del siguiente movimiento

    if (*last_move != 'D' && verify(bot_pos[0] - 1, bot_pos[1], 'U')) // Ir Arriba
        x = -1;
    else if (*last_move != 'L' && verify(bot_pos[0], bot_pos[1] + 1, 'R')) // Ir a la Derecha
        y = 1;
    else if (*last_move != 'U' && verify(bot_pos[0] + 1, bot_pos[1], 'D')) // Ir Abajo
        x = 1;
    else if (*last_move != 'R' && verify(bot_pos[0], bot_pos[1] - 1, 'L')) // Ir a la Izquierda
        y = -1;
    
    *last_move = x == 1 ? 'D' : x == -1 ? 'U' : y == 1 ? 'R' : y == -1 ? 'L' : 'N'; // Se actualiza el char de ultimo movimiento
    if (*last_move == 'N') // Si no queda ninguna zona no limpiada o sin caja a donde moverse
        move_back(last_move, &x, &y); // El robot debe retroceder
    else{
        back = 0; // Se indica que el bot no retrocedió
        log_2[count2++] = *last_move; // Se registra el movimiento elegido
    }
    update_array(next_move, x, y); // Se guarda la jugada elegida como la proxima jugada
}

// Función que mueve al robot y verifica si se puede limpiar la casilla actual
// Parametros: La posición actual del robot, el movimiento elegido y el total de casillas a limpiar restantes
// Retorno: void
void move_bot(int *bot_pos, int *next_move, int *total_squares) {
    bot_pos[0] += next_move[0];
    bot_pos[1] += next_move[1];
    if (area[bot_pos[0]][bot_pos[1]] == '.'){ // Se verifica si se puede limpiar la casilla actual
        area[bot_pos[0]][bot_pos[1]] = 'C'; // Se marca como "limpia" la casilla
        --*total_squares; // Se decrementa el total de casillas a limpiar
    }
}

int main(){
    char last_move = 0; // El char que representa el último movimiento (L, R, U, D)
    int bot_pos[2] = {0, 0}, next_move[2] = {0, 0}, total_squares = -1; // La ubicación del robot, el movimiento elegido y el total de casillas a limpiar restantes

    for(int i = 0; i < TAM_AREA; i++) // Se cuentan las casillas a limpiar
        for(int j = 0; j < TAM_AREA; j++)
            if (area[i][j] == '.') total_squares++;

    area[0][0] = 'C'; // Se marca como "limpiada" la casilla inicial
    while(total_squares){
        choose_move(bot_pos, next_move, &last_move); // Se elige el siguiente movimiento
        move_bot(bot_pos, next_move, &total_squares); // Se mueve al robot
        log_moves[count++] = last_move; // Actualizo el log
        if (back) count2--; // Si se retrocedió se actualiza el contador 2
    }
    print_log();
    return 0;
}