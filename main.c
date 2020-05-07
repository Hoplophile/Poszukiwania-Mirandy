#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int D = 1;
int current_depth;
int number_of_rooms;
int number_of_possible_rooms;

int* rooms_visited_init(){
    int* visited_array = (int*)malloc(sizeof(int) * number_of_rooms);
    
    for(int i=0; i<number_of_rooms; i++){
        visited_array[i] = false;
    }

    return visited_array;
}

int** rooms_graph_init(){
    int **graph_array = (int**)malloc(sizeof(int *) * number_of_rooms);
    for (int i=0; i<number_of_rooms; i++){
        graph_array[i] = (int*)malloc(sizeof(int) * number_of_rooms);
    }

    return graph_array;
}

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

void get_rooms_graph_from_file(FILE *file, int **graph_array){
    char buffer = ' ';
    int row=0, column=0;

    for(row=0; row<number_of_rooms; row++){
        do{
            buffer = (char)fgetc(file);
            if(buffer != ',') {
                printf("%c, ", buffer);
                graph_array[row][column] = (int)(buffer - '0');
                column++;
            }
        }while(column < number_of_rooms);
        fgetc(file); // pass CR
        column = 0;
        printf("\n");
    }

    rewind(file);
}

void DFS(int room_number, int** rooms_graph, int* rooms_visited){
    current_depth++;
    printf("going to room %d\n", room_number);
    printf("DEPTH %d\n", current_depth);
    
    if(current_depth > D) number_of_possible_rooms++;

    rooms_visited[room_number] = true;

    // search for neighbour of current room
    for(int neighbour_number=0; neighbour_number<number_of_rooms; neighbour_number++){
        if(rooms_graph[room_number][neighbour_number] == 1){
            printf("i found neighbour: %d\n", neighbour_number);
            // check if found neighbour is not visited
            if(rooms_visited[neighbour_number] != true){
                printf("neighbour %d not yet visited, entering room\n", neighbour_number);
                // call DFS for neighbour
                DFS(neighbour_number, rooms_graph, rooms_visited);
            } else{
                printf("already visited\n");
            }
        }
    }
    current_depth--;
}

int main(){
    current_depth = -1;
    number_of_possible_rooms = 0;
    FILE *input_data;
    int **rooms_graph;
    int *rooms_visited;

    input_data = fopen("input_data.txt", "r");
    count_number_of_rooms(input_data);
    printf("Alright, there are %d rooms in this house.\n", number_of_rooms);

    rooms_graph = rooms_graph_init();
    rooms_visited = rooms_visited_init();

    get_rooms_graph_from_file(input_data, rooms_graph);

    DFS(0, rooms_graph, rooms_visited);
    printf("possible rooms: %d", number_of_possible_rooms);

    free(rooms_graph);
    free(rooms_visited);
    fclose(input_data);

    return 0;
}