#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

long long int **tag_array;
long long int **lru_position, **fifo_count;
bool **dirty, **valid;

int num_sets, assoc, cache_size, lru, wb;
long long int hit = 0, miss = 0, reads = 0, writes = 0;

void update_lru(int index, int block_index) {
    unsigned long long max_lru_counter = 0;

    for (int i = 0; i < assoc; i++) {
        if (i != block_index && lru_position[index][i] > max_lru_counter) {
            max_lru_counter = lru_position[index][i];
        }
    }

    lru_position[index][block_index] = max_lru_counter + 1;
}

void update_fifo(int index, int block_index) {
    fifo_count[index][block_index] = miss;
}

void simulate_access(char op, unsigned long long int add) {
    unsigned long long block_size = 64;
    unsigned long long index_mask = (cache_size / (assoc * block_size)) - 1;
    unsigned long long index = (add / block_size) & index_mask;
    unsigned long long tag = add / (block_size * (index_mask + 1));
    
    for (int i = 0; i < assoc; i++) {
        if (valid[index][i] && tag == tag_array[index][i]) {
            hit++;
            if (op == 'W') {
                if (wb) {
                    dirty[index][i] = true;
                } else {
                    writes++;
                }
            }
			
            if (lru == 0) {
                update_lru(index, i);
            }
			return;
        }
    }

   	miss++;
    reads++;
    
    int victim_block_index = -1;
    unsigned long long min_counter = -1;

    for (int i = 0; i < assoc; i++) {
        unsigned long long counter = lru_position[index][i];

        if (lru == 1) {
            counter = fifo_count[index][i];
        }

        if (counter < min_counter) {
            min_counter = counter;
            victim_block_index = i;
        }
    }
        
    if (op == 'W') {
        if (wb) {
	        if (dirty[index][victim_block_index]) {
	            writes++;
	        }
        } else {
            writes++;
        }
    }
    
    if (op == 'R' && wb && dirty[index][victim_block_index] && valid[index][victim_block_index]) {
        writes++;
    }

    valid[index][victim_block_index] = true;
    tag_array[index][victim_block_index] = tag;
	dirty[index][victim_block_index] = (op == 'W' && wb);
  
    if (lru == 0) {
        update_lru(index, victim_block_index);
    } else {
    	update_fifo(index, victim_block_index);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        puts("Invalid Input Args");
        return 1;
    }

    cache_size = atoi(argv[1]);
    assoc = atoi(argv[2]);
    lru = atoi(argv[3]);
    wb = atoi(argv[4]);
    char *trace_file = argv[5];

    num_sets = cache_size / (64 * assoc);

    tag_array = calloc(num_sets, sizeof(long long int *));
    lru_position = calloc(num_sets, sizeof(long long int *));
    fifo_count = calloc(num_sets, sizeof(long long int *));
    dirty = calloc(num_sets, sizeof(bool *));
    valid = calloc(num_sets, sizeof(bool *));
    for (int i = 0; i < num_sets; i++) {
        tag_array[i] = calloc(assoc, sizeof(long long int));
        lru_position[i] = calloc(assoc, sizeof(long long int));
        fifo_count[i] = calloc(assoc, sizeof(long long int));
        dirty[i] = calloc(assoc, sizeof(bool));
        valid[i] = calloc(assoc, sizeof(bool));
    }

    FILE *inp = fopen(trace_file, "r");
    if (inp == NULL) {
        printf("Error: Could not open trace file.\n");
        return 1;
    }

    char op;
    long long int add;
    int cnt = 0;

	while (!feof(inp)) {
	    fscanf(inp, " %c %llx", &op, &add);
	    simulate_access(op, add);
	}    
	fclose(inp);

    for (int i = 0; i < num_sets; i++) {
        free(tag_array[i]);
        free(lru_position[i]);
        free(fifo_count[i]);
        free(dirty[i]);
        free(valid[i]);
    }
    free(tag_array);
    free(lru_position);
    free(fifo_count);
    free(dirty);
    free(valid);

    double miss_ratio = (double)miss / (hit + miss);
    printf("%.6f\n", miss_ratio);
    printf("%lld\n", writes);
    printf("%lld\n", reads);

    return 0;
}


