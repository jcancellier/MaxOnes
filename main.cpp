/* Author: Joshua Cancellier
 *
 * Purpose: Solve Max Ones problem through genetic algorithms
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

//CONSTANTS
#define POPULATION_SIZE 25
#define CHROMOSOME_SIZE 32
#define PC 1 //probability of crossover
#define PM 0.01 //probability of mutation
#define K 3     //size of subset used for tournament selection

using namespace std;

//prototypes
void initPopulation(vector<vector<int> > &population);
int logPopulation(const vector<vector<int> > population);
int calcIndividualFitness(const vector<int> individual);
void mateAndMutate(vector<vector<int> > &population);
int selection(int individualPos, vector<vector<int> > population);
void crossover(vector<int> ind1, vector<int> ind2, vector<vector<int> > &toFill);
void mutate(vector<int> &individual);
static bool nameCmp(vector<int> a, vector<int> b) { return calcIndividualFitness(a) < calcIndividualFitness(b);}

int main()
{
    int best = 0;
    srand(time(NULL));
    vector< vector<int> > individuals; 
    initPopulation(individuals);
    best = logPopulation(individuals);
    cout << "GENERATION 1: Best individual's fitness is " << best << endl;
    cout << "**********************************************\n\n";

    int generationCount = 2;
    while(best != CHROMOSOME_SIZE){
        mateAndMutate(individuals);
        best = logPopulation(individuals);
        cout << "GENERATION " << generationCount << ": Best individual's fitness is " << best << endl;
        cout << "**********************************************\n\n";
        generationCount++;
    }
    return 0;
}


//randomly sets bits to members in population (0 or 1)
void initPopulation(vector< vector<int> > &population)
{
    vector<int> individual;    //temporary vector to fill individual
    //srand(time(NULL));
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < CHROMOSOME_SIZE; j++) {
            int random = rand() % 2;    //random number (0 or 1)
            individual.push_back(random);
        }
        population.push_back(individual);
        individual.clear();
    }
}

//select random member for crossover
int selection(int individualPos, vector<vector<int> > population)
{
    //srand(time(NULL));
    int tournamentSubset[K];

    //determine subset first (size K)
    for (int i = 0; i < K; i++) {
        do{
            tournamentSubset[i] = rand() % POPULATION_SIZE;
        }while(tournamentSubset[i] == individualPos);
    }

    //pick member from subset with highest fitness and return
    int greatestFitness = tournamentSubset[0];
    for (int i = 1; i < K; i++) {
        if(calcIndividualFitness(population[tournamentSubset[i]]) 
        > calcIndividualFitness(population[greatestFitness])) {
            greatestFitness = tournamentSubset[i];
        }
    }
    return greatestFitness;
}

void crossover(vector<int> ind1, vector<int> ind2, vector<vector<int> > &toFill)
{
    vector<int> ind1end;    //latter half of ind1
    vector<int> ind2beg;    //first half  of ind2

    //fill ind1end
    for(int i = CHROMOSOME_SIZE/2; i < CHROMOSOME_SIZE; i++)
        ind1end.push_back(ind1[i]);

    //fill ind2beg
    for(int i = 0; i < CHROMOSOME_SIZE/2; i++)
        ind2beg.push_back(ind2[i]);

    //perform crossover for 2nd individual
    for (int i = 0; i < CHROMOSOME_SIZE/2; i++) {
        ind2[i] = ind1end[i];
    }

    //perform crossover for 1st individual 
    int j = 0;
    for (int i = CHROMOSOME_SIZE/2; i < CHROMOSOME_SIZE; i++) {
        ind1[i] = ind2beg[j];
        j++;
    }

    toFill.push_back(ind1);
    toFill.push_back(ind2);
}

void mutate(vector<int> &individual)
{
    //random bit position to flip
    int random = rand() % CHROMOSOME_SIZE;

    //flip bit (ternary operator)
    individual[random] = individual[random] ? 0 : 1;

    //cout << "bit to flip: " << random << endl;
}


void mateAndMutate(vector< vector<int> > &population)
{
    vector<vector<int> > newPopulation; 
    float random;
    bool shouldCrossover = false; //determine if individual should mate (0 or 1)
    bool shouldMutate = false;  //determine if individual will mutate (0 or 1)

    //iterate over population for selection, crossover, and mutation
    for (int i = 0; i < POPULATION_SIZE; i++) {

        //determine if chromosome should mate
        random = (rand() % 100) + 1; //random  number from 1 - 100
        random /= 100;
        shouldCrossover = (random <= PC) ? true : false;

        //determine if chromosome should mutate
        random = (rand() % 100) + 1; //random  number from 1 - 100
        random /= 100;
        shouldMutate = (random <= PM) ? true : false;
        //shouldMutate = true;
        //selection & crossover
        if (shouldCrossover) {
            //determine member to crossover with
            int toCrossover = selection(i, population); //individual to crossover with

            //perform crossover
            crossover(population[i], population[toCrossover], newPopulation);
        }

        //perform mutation
        if (shouldMutate) {
            mutate(newPopulation[i]);
        }
    }

    //sort new population
    sort(newPopulation.begin(), newPopulation.end(), nameCmp);

    //grab last half (greater half)
    newPopulation.erase(newPopulation.begin(), newPopulation.begin()+(newPopulation.size()/2));
    
    //replace population with greatest members of offspring
    population = newPopulation;
    return;
}


//calculates a single individual's fitness
int calcIndividualFitness(const vector<int> individual)
{
    int sum = 0;
    for (int i = 0; i < CHROMOSOME_SIZE; i++)
        sum += (individual[i]);
    return sum;
}


//displays all members of population
int logPopulation(const vector<vector<int> > population)
{
    cout << "**********************************************\n";
    int greatest = 0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        cout << "Individual " << i+1 << ": [";
        for (int j = 0; j < CHROMOSOME_SIZE; j++) {
            cout << (int) population[i][j];
        }
        cout << "] and fitness is " << calcIndividualFitness(population[i]);
        cout << endl;
        if(calcIndividualFitness(population[i]) > greatest)
            greatest = calcIndividualFitness(population[i]);
    }
    cout << "**********************************************\n";
    return greatest;
}
