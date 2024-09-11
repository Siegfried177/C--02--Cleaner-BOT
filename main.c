#include <stdio.h>
#include "robot.h"

int count, count2, back_done, rows[N];
char log_moves[N * (N + 7)], second_log[N * (N + 7)];

// Función que muestra el tablero
void print_area() {
    for(int i = 0; i < N; i++, puts("\n"))
        for(int j = 0; j < N; j++)
            printf("\t%c", area[i][j]);
}

// Función que muestra el log
void print_log() {	
    printf("Log: ");
    for (int i = 0; i < count; i++)
        printf(" %c", log_moves[i]);
    printf("\nTotal de movimientos: %d\n", count);
}
void print_log2()
{
    printf("Log2: ");
    for (int i = 0; i < count2; i++)
        printf(" %c", second_log[i]);
    printf("\nTotal de movimientos: %d\n", count2);
}

// Función que cambia los dos primeros valores de un array
void update_array(int *array, int value1, int value2) {
    array[0] += value1;
    array[1] += value2;
}

// Función para verificar si el robot puede avanzar a una casilla
int verify(int x, int y, char next_move) {
    if (x == N || y == N || x < 0 || y < 0 || area[x][y] == '#' || area[x][y] == 'C')
        return 0;
    if (count >= 3)
        if (log_moves[count - 3] == log_moves[count - 2] && log_moves[count - 2] == log_moves[count - 1] && log_moves[count - 1] == next_move)
            return 0;
    return 1;
}

// Función para retroceder el robot
void move_back(int *next_move, char *last_move, int x, int y) {
    static int index;
    index = back_done ? 1 : index;
    char aux2 = second_log[count2 - index];
    back_done = 0;
    printf("Aux2: %c con index: %d\n", aux2, count2 - index);
    if (aux2 == 'D')
        x = -1;
    else if (aux2 == 'U')
        x = 1;
    else if (aux2 == 'L')
        y = 1;
    else
        y = -1;
    index++;
    update_array(next_move, x, y);
    *last_move = x == 1 ? 'D' : x == -1 ? 'U' : y == 1 ? 'R' : y == -1 ? 'L' : 0;
}

// Función para elegir el siguiente movimiento
char choose_move(int *bot_pos, int *next_move, char last_move, int *last_move_array) {
    int x = 0, y = 0;

    if (last_move != 'D' && verify(bot_pos[0] - 1, bot_pos[1], 'U')) // Ir Arriba
        x = -1;
    else if (last_move != 'L' && verify(bot_pos[0], bot_pos[1] + 1, 'R')) // Ir a la Derecha
        y = 1;
    else if (last_move != 'U' && verify(bot_pos[0] + 1, bot_pos[1], 'D')) // Ir Abajo
        x = 1;
    else if (last_move != 'R' && verify(bot_pos[0], bot_pos[1] - 1, 'L')) // Ir a la Izquierda
        y = -1;
    
    last_move = x == 1 ? 'D' : x == -1 ? 'U' : y == 1 ? 'R' : y == -1 ? 'L' : 'N';
    if (last_move == 'N') // No queda ninguna zona no limpiada o sin caja a donde moverse
        move_back(next_move, &last_move, x, y);
    else{
        update_array(next_move, x, y);
        second_log[count2++] = last_move; // Actualizo el log
    }
    update_array(last_move_array, - last_move_array[0] + x, - last_move_array[1] + y);

    return last_move;
}

// Función que mueve al robot y verifica si se puede limpiar la casilla actual
void move_bot(int *bot_pos, int *next_move, int *cleaned) {
    bot_pos[0] += next_move[0];
    bot_pos[1] += next_move[1];
    if (area[bot_pos[0]][bot_pos[1]] == '.'){
        area[bot_pos[0]][bot_pos[1]] = 'C';
        *cleaned += 1;
        rows[bot_pos[0]]--;
        back_done= 1;
    }
    count++;
}

int main(){
    char last_move = 0;
    int bot_pos[2] = {0, 0}, last_move_array[2] = {0, 0}, next_move[2] = {0, 0};
    int cleaned = 1, total_squares = 0, counter = 0;

    for(int i = 0, area[0][0] = 'C'; i < N; i++) // Se cuentan las casillas a limpiar y se marca como "limpia" la casilla de salida
        for(int j = 0; j < N; j++)
            if (area[i][j] == '.'){ 
                total_squares++;
                rows[i]++;
            }

    while(cleaned < total_squares && counter < 20){
        update_array(next_move, - next_move[0], - next_move[1]); // Reinicio el array del siguiente movimiento a (0, 0)
        last_move = choose_move(bot_pos, next_move, last_move, last_move_array);
        log_moves[count] = last_move; // Actualizo el log
        move_bot(bot_pos, next_move, &cleaned);
        counter++;
        for (int i = 0; i < N; i++)
            printf(" %d", rows[i]);
    }
    print_area();
    print_log();
    
    print_log2();
    return 0;
}