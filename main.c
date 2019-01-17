#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <time.h>


#define PI 3.14159265358979323846


// Function initializations
void displace_walker(double * walkers, double delta_tau, double rand_var, int i);
double gsl_ran_gaussian(const gsl_rng * r, double sigma);
double evaluate_weight_factor( double walkers, double delta_tau, double energy);
double getWaveFunction( double walkers );
double calculateNewEnergy(double energy, double alpha, double delta_tau, int N_0, int N);
int doom (double w, double u);

// Global variables
int nbrOfNewWalkers; 
int total_pos_save = 0;
int main() {
        int nbr_walkers;
        int nbrIt;
        int max_nbr_walkers;
        double uniform_var1, uniform_var2;
        double gaussian_random_variable;
        double delta_tau;
        int N_0;
        int nbr_iterations;
        double E_T;
        double alpha;
        double energy_sum;
        nbr_walkers = 800;
        max_nbr_walkers = nbr_walkers*100; 
        N_0 = nbr_walkers; 
        delta_tau = 0.05;
        nbr_iterations = 1E4;
        E_T = 0.7;
        alpha = 0.5;
        energy_sum = 0;

        int a = 1;
        // allocating memory
        double * walkers = malloc(sizeof(double)*max_nbr_walkers);
        double * walkers_copy = malloc(sizeof(double)*max_nbr_walkers);
        int * create_eliminate = malloc(sizeof(int)*max_nbr_walkers);
        double * saved_avg_energy = malloc(sizeof(double)*nbr_iterations);
        double * saved_energy = malloc(sizeof(double)*(nbr_iterations/a));
        int * nbr_of_walkers = malloc(sizeof(int)*nbr_iterations/a);
        double * walkers_positions = malloc(sizeof(double)*max_nbr_walkers);
        // Initialization GSL
        const gsl_rng_type *T;
        gsl_rng *q;
        gsl_rng_env_setup();
        T = gsl_rng_default;
        q = gsl_rng_alloc(T);
        gsl_rng_set(q,time(NULL));

        double grv;
        double u;
        size_t counter = 0;
        
        for (int j=0; j<nbr_walkers; j++) {
                grv  =  gsl_ran_gaussian(q,1);      
                walkers[j] = grv;
                displace_walker(walkers, delta_tau, grv, j);
        }
        for (int i=0; i<nbr_iterations; i++){
                double w;
                int d;
                nbrOfNewWalkers = 0;
                
                for (int j=0; j<nbr_walkers; j++) {
                        grv  =  gsl_ran_gaussian(q,1);                    
                        displace_walker(walkers, delta_tau, grv, j);
                        walkers_copy[j] = walkers[j];
                        w = evaluate_weight_factor(walkers[j], delta_tau, E_T);
                        u  =  gsl_rng_uniform(q);
                        d = doom(w,u);
                        create_eliminate[j] = d;
                        //printf("%d\n", d);
                        counter += d;
                        nbrOfNewWalkers += d;
                }

                energy_sum += E_T;
                saved_avg_energy[i] = energy_sum / (double) (i+1);
               
                if (i%a==0) {
                        saved_energy[i/a] = E_T;
                        nbr_of_walkers[i/a] = nbr_walkers;
                }

                // Calculate new energy
                E_T = calculateNewEnergy(saved_avg_energy[i], alpha, delta_tau, N_0, nbrOfNewWalkers);
                nbr_walkers = nbrOfNewWalkers;
               
                int check=0;
                int itx = 0;
                // Creates walkers according to the create_eliminate list
                for (itx=0; itx< nbrOfNewWalkers; itx++){
                        for (int jx = check; jx < check + create_eliminate[itx]; jx++) {
                                walkers[jx] = walkers_copy[itx];

                        }
                        check += create_eliminate[itx];
              
                }
                int iterCounter = 0;
                if (nbr_iterations-i < 20 ) { // takes the last 20 iterations to save.
                        for (int j=total_pos_save; j<nbrOfNewWalkers+total_pos_save; j++){
                                walkers_positions[j] = walkers[iterCounter];
                                iterCounter++;
                        }
                        total_pos_save += nbrOfNewWalkers;
                }
                
                // Set elements to zero just to be sure.
                for (int j=0; j<max_nbr_walkers; j++) {
                        walkers_copy[j] = 0;
                        create_eliminate[j] = 0;
                        if (j > check) {
                                walkers[j] = 0;
                        }
                }
                
        } // End i 
        printf("The average energy is: %f \n", saved_avg_energy[nbr_iterations-1]);

        FILE * nbr_walkers_F;
        nbr_walkers_F = fopen("number_walkers.dat", "w");

        for (int i = 0; i<nbr_iterations/a; i++){
                fprintf(nbr_walkers_F, "%d\n", nbr_of_walkers[i] );
        }
        fclose(nbr_walkers_F);

        FILE * energy_F;
        energy_F = fopen("energy.dat", "w");

        for (int i = 0; i<nbr_iterations/a; i++){
                fprintf(energy_F, "%f\n", saved_energy[i] );
        }
        fclose(energy_F);

        FILE * walkers_F;
                walkers_F = fopen("walkers.dat", "w");

        for (int i = 0; i<max_nbr_walkers; i++){
                if (walkers_positions[i] != 0) {
                        fprintf(walkers_F, "%f\n", walkers_positions[i] );
                }
        }
        fclose(walkers_F);
        
        free(walkers);
        free(walkers_copy);
        return 0;
}
// Function to displace the walkers
void displace_walker(double * walkers, double delta_tau, double rand_var, int i){
        // this one does one at the time, can make a for loop if it's necessary
        walkers [i] = walkers[i] + sqrt(delta_tau)*rand_var; 
}
// Function to evalute the weight factor
double evaluate_weight_factor( double walkers, double delta_tau, double energy) {
        double k, w;
        k = 0.5*walkers*walkers;
        w  = exp(-delta_tau*(k-energy));
        return w;              
}
// Function to calculate the energy of the system and adjusting w.r.t. number of walkers
double calculateNewEnergy(double energy, double alpha, double delta_tau, int N_0, int N) {
        double new_energy;

        new_energy = energy - (alpha / delta_tau)*(log(N) - log(N_0));
        return new_energy;
}

// Function that calculates if the walkers should be killed of or copied.
int doom (double w, double u) {
        int doom;
        doom = w + u;
        return doom;
}
