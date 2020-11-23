#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "ai.h"
#include "utils.h"
#include "hashtable.h"
#include "stack.h"
#include "list.h"


void copy_state(state_t* dst, state_t* src){
	
	//Copy field
	memcpy( dst->field, src->field, SIZE*SIZE*sizeof(int8_t) );

	dst->cursor = src->cursor;
	dst->selected = src->selected;
}

/**
 * Saves the path up to the node as the best solution found so far
*/
void save_solution( node_t* solution_node ){
	node_t* n = solution_node;
	while( n->parent != NULL ){
		copy_state( &(solution[n->depth]), &(n->state) );
		solution_moves[n->depth-1] = n->move;

		n = n->parent;
	}
	solution_size = solution_node->depth;
}


node_t* create_init_node( state_t* init_state ){
	node_t * new_n = (node_t *) malloc(sizeof(node_t));
	new_n->parent = NULL;	
	new_n->depth = 0;
	copy_state(&(new_n->state), init_state);
	return new_n;
}

/**
 * Apply an action to node n and return a new node resulting from executing the action
*/
 void applyAction(node_t* n, node_t* new_node, position_s* selected_peg, \
 	move_t action ){
	//FILL IN MISSING CODE

	//update state
	copy_state(&(new_node->state), &(n->state)); 

	//update parent
	new_node->parent = n;

	//update depth
	new_node->depth = new_node->parent->depth + 1;

	//update move
	new_node->move = action;

	//update cursor
	new_node->state.cursor.x = selected_peg->x;
	new_node->state.cursor.y = selected_peg->y;

	//update action
    execute_move_t( &(new_node->state), &(new_node->state.cursor), action ); 
	
	return;

}

/**
 * Find a solution path as per algorithm description in the handout
 */

void find_solution( state_t* init_state  ){

	HashTable table;
	int remain_pegs;
	int x; // traversing horizontally
	int y; //traversing vertically
	move_t jump; //direction in move_t
	position_s *selected_peg = (position_s *)malloc(sizeof(position_s));
	node_t *new_node;
	list_t *popped_list = make_empty_list();
	
	// Choose initial capacity of PRIME NUMBER 
	// Specify the size of the keys and values you want to store once 
	ht_setup( &table, sizeof(int8_t) * SIZE * SIZE,\
	 sizeof(int8_t) * SIZE * SIZE, 16769023);

	// Initialize Stack
	initialize_stack();

	//Add the initial node
	node_t* n = create_init_node( init_state );
	stack_push(n);
	remain_pegs = num_pegs(init_state);

	//FILL IN THE GRAPH ALGORITHM
	while (!is_stack_empty()){
		n = stack_top();
		stack_pop();
		popped_list = insert_at_foot(popped_list, n);
		expanded_nodes += 1;

		//Found a better solution
		if ( num_pegs(&(n->state)) < remain_pegs ){ 
			save_solution(n);
			remain_pegs = num_pegs(&(n->state));
		}

		for (x = 0; x < SIZE; x++){
			selected_peg->x = x;

			for (y = 0; y < SIZE; y++){
				selected_peg->y = y;

				for(jump = left; jump <= down; jump ++){
					if (can_apply(&(n->state), selected_peg, jump)){
						new_node =(node_t *) malloc(sizeof(node_t));

						//Create child node
						applyAction(n,new_node, selected_peg ,jump); 
						generated_nodes += 1;
						
						//Puzzle soved!
						if (won(&(new_node->state))){ 
							save_solution(new_node);
							remain_pegs = num_pegs(&(new_node->state));

							free(selected_peg);
							free_stack();
							ht_destroy(&table);
							free_list(popped_list);
							free(new_node);
							return;
						}

						//Check for duplicates
						if (ht_contains(&table, &(new_node->state.field))\
						 == HT_FOUND){ 
							free(new_node);
							continue;
						}

						//Add to hashtable if the vertex is not visited yet
						else if (ht_contains(&table, &(new_node->state.field))\
						 == HT_NOT_FOUND){ 
							stack_push(new_node);
							ht_insert(&table, &(new_node->state.field),\
							 &(new_node->state.field));
						}							
					}
				}
			}
		}

		//Budget dried out
		if (expanded_nodes >= budget){ 
			free(selected_peg);
			free_stack();
			ht_destroy(&table);
			free_list(popped_list);
			return;
		}	
	}
}



