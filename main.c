#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int current_depth;
int number_of_rooms;
int num_of_searched_rooms;

/* rooms_visited_init
 * Allocates array of visited rooms based on 
 * global variable 'number_of_rooms'
 * 
 * return: 
 *     pointer to array of visited rooms 
 */
int* rooms_visited_init(){
    int* visited_array = (int*)malloc(sizeof(int) * number_of_rooms);
    
    for(int i=0; i<number_of_rooms; i++){
        visited_array[i] = false;
    }

    return visited_array;
}

/* rooms_graph_init
 * Allocates 2D array of rooms (main graph) based on 
 * global variable 'number_of_rooms'
 * 
 * return: 
 *     pointer to 2D array of rooms (main graph)
 */
int** rooms_graph_init(){
    int **graph_array = (int**)malloc(sizeof(int *) * number_of_rooms);
    for (int i=0; i<number_of_rooms; i++){
        graph_array[i] = (int*)malloc(sizeof(int) * number_of_rooms);
    }

    return graph_array;
}

/* count_number_of_rooms
 * Reads the first line of file, calculates number of rooms
 * based on number of ',' in file and stores number of rooms
 * in global variable 'number_of_rooms'
 * 
 * params:
 *      file: input file with rooms graph
 */
void count_number_of_rooms(FILE* file){
    number_of_rooms = 1;
    char buffer = ' ';

    do{
        buffer = (char)fgetc(file);
        if(buffer == ',') 
            number_of_rooms++;
    } while(buffer != '\n');
    
    rewind(file);
}

/* get_rooms_graph_from_file
 * Reads file char by char and copies 1's and 0's
 * to array given as second parameter. Requires correct
 * number of rooms stored in global variable 'number_of_rooms'
 * 
 * params:
 *      file:        input file with rooms graph and D number
 *      graph_array: array describing the rooms graph
 */
void get_rooms_graph_from_file(FILE *file, int **graph_array){
    char buffer = ' ';
    int row=0, column=0;

    for(row=0; row<number_of_rooms; row++){
        do{
            buffer = (char)fgetc(file);
            if(buffer != ',') {
                graph_array[row][column] = (int)(buffer - '0');
                column++;
            }
        }while(column < number_of_rooms);
        fgetc(file); // pass CR
        column = 0;
    }
}

/* get_num_of_searched_rooms_from_file
 * Reads file char by char and writes first found number
 * to global variable 'num_of_searched_rooms' (number D)
 * 
 * params:
 *      file: input file with rooms graph and D number
 */
void get_num_of_searched_rooms_from_file(FILE *file){
    char buff;

    do{
        buff = (char)fgetc(file);
    } while((buff < 48) || (buff > 57));

    num_of_searched_rooms = (int)(buff  - '0');
}

/* DFS
 * Main graph searching recursive function. Function is called on
 * each not-visited room (graph node).
 * 
 * params:
 *      room_number:            number of room that algorithm is currently in
 *      rooms_graph:            2D array with 1's and 0's describing the rooms graph
 *      rooms_visited:          1D array indicating visited rooms
 *      num_of_possible_rooms:  number of rooms where the hamster can be
 */
void DFS(int room_number, int** rooms_graph, int* rooms_visited, int* num_of_possible_rooms){
    current_depth++;
    printf("Entering room number %d (%d stage(s) from room with cage).\n", room_number, current_depth);
    
    if(current_depth > num_of_searched_rooms) (*num_of_possible_rooms)++;

    rooms_visited[room_number] = true;

    // search for neighbour of current room
    for(int neighbour_number=0; neighbour_number<number_of_rooms; neighbour_number++){
        if(rooms_graph[room_number][neighbour_number] == 1){
            printf("It is connected with room number %d", neighbour_number);
            // check if found neighbour is not visited
            if(rooms_visited[neighbour_number] != true){
                printf(" and it's not been visited yet.\n-->\n", neighbour_number);
                // call DFS for neighbour
                DFS(neighbour_number, rooms_graph, rooms_visited, num_of_possible_rooms);
                printf(", going back to room number %d.\n<--\n", room_number);
            } else{
                printf(", but it's already been visited.\n");
            }
        }
    }
    printf("All rooms connected to room number %d are visited", room_number);
    current_depth--;
}

int main(){
    current_depth = -1;
    number_of_rooms = 0;
    num_of_searched_rooms = 0;
    FILE *input_data;
    int **rooms_graph;
    int *rooms_visited;
    int num_of_possible_rooms = 0;

    input_data = fopen("input_data.txt", "r");
    count_number_of_rooms(input_data);

    rooms_graph = rooms_graph_init();
    rooms_visited = rooms_visited_init();

    get_rooms_graph_from_file(input_data, rooms_graph);
    get_num_of_searched_rooms_from_file(input_data);
    rewind(input_data);

    printf("Alright, there are %d rooms in this house and 1 hamster to find.\n", number_of_rooms);
    printf("ESLCh has already searched max depth of %d lavels counting from the room with cage.\n\n", num_of_searched_rooms);

    DFS(0, rooms_graph, rooms_visited, &num_of_possible_rooms);

    printf("\n\nMiranda may be in %d rooms", num_of_possible_rooms);

    free(rooms_graph);
    free(rooms_visited);
    fclose(input_data);

    return 0;
}