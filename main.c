#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define n 5

// int n = 5;
int D = 1;
int current_depth;
int number_of_possible_rooms;

int rooms_graph[n][n] = {   {0,1,1,0,0},
                            {1,0,0,1,0},
                            {1,0,0,0,0},
                            {0,1,0,0,1},
                            {0,0,0,1,0}
                        };
                        
bool rooms_visited[n];

void rooms_visited_init(){
    for(int i=0; i<n; i++){
        rooms_visited[i] = false;
    }
}

int ** rooms_graph_init(void){
    int **graph_array = (int**)malloc(sizeof(int *) * n);
    for (int i=0; i<n; i++){
        graph_array[i] = (int*)malloc(n * sizeof(int));
    }
    printf("pointer to: %d\n", graph_array);
    return graph_array;
}

void DFS(int room_number){
    current_depth++;
    // printf("going to room %d\n", room_number);
    // printf("DEPTH %d\n", current_depth);
    
    if(current_depth > D) number_of_possible_rooms++;

    rooms_visited[room_number] = true;

    // search for neighbour of current room
    for(int neighbour_number=0; neighbour_number<n; neighbour_number++){
        if(rooms_graph[room_number][neighbour_number] == 1){
            // printf("i found neighbour: %d\n", neighbour_number);
            // check if found neighbour is not visited
            if(rooms_visited[neighbour_number] != true){
                // printf("neighbour %d not yet visited, entering room\n", neighbour_number);
                // call DFS for neighbour
                DFS(neighbour_number);
            } else{
                // printf("already visited\n");
            }
        }
    }
    current_depth--;
}

int main(){
    current_depth = -1;
    number_of_possible_rooms = 0;

    int **rooms_graph_2 = rooms_graph_init();

    rooms_graph_2[1][1] = 2;
    printf("\n%d\n", rooms_graph_2[1][1]);

    rooms_visited_init();
    printf("\n\n%d", rooms_graph_2[1]);

    DFS(0);
    // printf("possible rooms: %d", number_of_possible_rooms);

    return 0;
}