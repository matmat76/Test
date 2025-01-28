#include "pilot.h"
#include "copilot.h"
#include "stdbool.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static int current_step;
int path_step;
path_status_t path_status;
static move_t *path;

void copilot_set_Path(move_t* a_path, int num_moves){
    if (path != NULL){
        free(path);
    }
    path = (move_t*)calloc(num_moves,sizeof(move_t));
    path = memcpy(path,a_path,num_moves*sizeof(move_t));
    path_step = num_moves;
}

move_t* copilot_get_path(){
    return path;
}
move_t copilot_get_step(int index){
    return path[index];
}

void copilot_set_number_step(int num_steps){
    path_step = num_steps;
}

int copilot_get_number_step(){
    return path_step;
}

int copilot_destroy_path(){
    if(path != NULL){
        free(path); 
        path = NULL;
        return 0; 
    }else{
        path_status = PATH_ERROR; 
        return -1;
    }
}
void copilot_add_step(move_t a_move){
    if(path == NULL){
        return; 
    }
    path[current_step] = a_move;
    current_step++;
}

void copilot_start_path(){
    if(path == NULL){
        path_status = PATH_ERROR;
        return;
    }
    fprintf(stdout,"Starting path\n");
    current_step = 0;

    path_status = PATH_MOVING; 
    fprintf(stdout,"Entering pilot start move\n");
    pilot_start_move(path[current_step]);
    fprintf(stdout,"Value of move_type : %d\n",path[current_step].move_type);
    fprintf(stdout,"Path started\n");
}

path_status_t copilot_stop_at_step_completion(){
    if(pilot_stop_at_target() == MOVE_DONE){
        current_step++;
        if(current_step == path_step){
            path_status = PATH_COMPLETE;
        }else{
           pilot_start_move(path[current_step]);
        }
    }
    return path_status;
}

/*Return the true or false if the variable path_status is PATH_COMPLETE
In other words : if the whole movements are */
bool copilot_is_path_complete(){
    return path_status == PATH_COMPLETE; 
}