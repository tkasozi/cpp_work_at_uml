#include "Assign5.hpp"
//jdf dfid nvnjdud v dat ham nsnid viasf dfd
using namespace std;

void printd(request req[], string policy, int mem) {

    int fail = 0, pass = 0;

    for (int i = 0; i <= NUMBER_ENTRIES; i++) {

        if (req[i].status == FAILED) {
            //failed
            req[i].base_adr = -1;
            
            //++fail;

        }
        if (req[i].is_allocated){
            ++pass;
        }
    }

    cout << "POLICY: " << policy << " MEMORY SIZE: " << mem << "\n" << endl;

    cout << "TOTAL ALLOCATIONS: " << pass  << endl << endl;


    for (int i = 1; i < NUMBER_ENTRIES; i++) {
        if (req[i].status == DONE)
            cout << "Seq " << i << " \tReq " << ((req[i].is_req) ? "alloc" : "free") << " \tAlloc size " << req[i].size << " \tBase addr "
                << req[i].base_adr << " \t\tTotal mem " << req[i].memory_left << " \tLargest chunk " << req[i].largest_chunk << endl; //<<"\tlinks: "<<req[i].elements_on_free_list 

    }

    //<< " \tDone: " << ((req[i].status==DONE) ? "Yes" : "No")

    cout << "\n\nTOTAL FAILED ALLOCATIONS: " << (1000-pass) << endl;
}

int update_list(int i, request req_array[], free_list list_head, int* total_free) {

    /* free_lists to hold objects*/
    struct free_list* list;
    struct free_list* block;
    struct free_list* compare;

    if (req_array[i].is_allocated == false) {

        //        printf("Failed:%d %d \n", req_array[i].memory_left, ++x);
        /* unusued */
        return 0;
    }

    /* block is done */
    // req_array[i].is_allocated = true;
    req_array[i].status = DONE;

    *total_free += req_array[i].size;

    // look at all the blocks in the free list 
    for (list = list_head.next; list; list = list -> next) {

        //bad index
        if (req_array[i].base_adr > list -> block_adr) {
            //do nothing
        }

        /* create a block to store */
        block = (free_list*) malloc(sizeof (struct free_list));
        block -> block_size = req_array[i].size;
        block -> block_adr = req_array[i].base_adr;
        block -> adjacent_adr = block -> block_adr + block -> block_size;

        block->next = list;
        list -> previous -> next = block;
        block -> previous = list -> previous;
        list -> previous = block;

        // Check to see if we can combine with next 
        if (block -> adjacent_adr == block -> next -> block_adr) {
            compare = block -> next;
            block -> block_size = block -> block_size + block -> next -> block_size;
            block -> adjacent_adr = block -> next -> adjacent_adr;
            block -> next = block -> next -> next;

            if (block -> next) {
                block -> next -> previous = block;
            }

            free(compare);

        }

        // with previous 
        block = block -> previous;
        if ((block != NULL) && (block -> adjacent_adr != 0)) {
            if (block -> adjacent_adr == block -> next -> block_adr) {
                compare = block->next;
                block -> block_size = block -> block_size + block -> next -> block_size;
                block -> adjacent_adr = block -> next -> adjacent_adr;
                block -> next = block -> next -> next;

                if (block -> next) {
                    block ->next -> previous = block;
                }

                free(compare);

            }
        }

        return 0;

    }
    return 0;
}
