#include "computing.h"
#include "random.h"

double init_value()
{
    double value;
    value = (double) random_rand32();
    return value;
}

// Syncronous mode
double compute_value_from_neighbours(double my_value, uint32_t n_neighbours,
        struct received_values *neighbors_values, uint8_t value_num)
{

    double new_value = my_value;
    int i;

    for (i = 0; i < MAX_NUM_NEIGHBOURS; i++) {
        if (!neighbours_values[i].valid)
            continue;

        uint32_t n_num_neighbours = neighbours_values[i].num_neighbours;
        double n_value = neighbours_values[i].values.v[value_num];

        // Add contribution for this neighbour with
        // n_num_neighbours and n_value
        // new_value += ...;
        (void) n_value;
        (void) n_num_neighbours;
        new_value = fmax(new_value, n_value);
    }

    return new_value;
}

// when running in poisson mode
double compute_value_from_gossip(double my_value, double neigh_value)
{
    return fmax(my_value, neigh_value);
}

// When needed to calculate one int result in gossip mode
uint32_t compute_final_value()
{

    // calculate expected number of nodes here using
    // 'my_values' and 'num_values'
    return 0;
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */

uint8_t compute_number = 0;
struct values my_values = {.v={NAN}};
struct received_values neighbours_values[MAX_NUM_NEIGHBOURS] = {{0}};

/*
 * General functions managing multiples values
 */
void compute_all_values_from_gossip(struct received_values *neigh_values)
{
    int i;
    compute_number++;
    for (i = 0; i < NUM_VALUES; i++) {
        my_values.v[i] = compute_value_from_gossip(
                my_values.v[i], neigh_values->values.v[i]);
    }
}
