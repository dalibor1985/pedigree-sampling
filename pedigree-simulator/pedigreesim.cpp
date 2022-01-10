#include<iostream>
#include<assert.h>
#include <string.h>
#include <vector>
#include "population.h"

void printHelp() {

    std::cerr << "\n+====================+\n";
    std::cerr << "| PEDIGREE SIMULATOR |\n";
    std::cerr << "+====================+\n\n";
    std::cerr << "Author: Dalibor Hršak\n\n";
    std::cerr << "Usage: pedigreesim [OPTIONS] > output.csv\n\n";
    std::cerr << "FLAGS:\n\n\t-h, --help:\t Print this help and exit.\n\n";
    std::cerr << "OPTIONS:\n\n\t--male-pop=<int>\t\tSize of the founder male population. [default: 5]\n\n";
    std::cerr << "\t--female-pop=<int>\t\tSize of the founder female population. [default: 5]\n\n";
    std::cerr << "\t--final-pop=<int>\t\tSize of the final population. [default: 100]\n\n";
    std::cerr << "\t--male-ratio=<float>\t\tFraction of the male population generated during the simulation. [default: 0.5]\n\n";
    std::cerr << "\t--average-partners=<int>\tAverage number of partners males will have during the simulation. [default: 3]\n\n";
    std::cerr << "\t--average-offspring=<int>\tAverage number of offspring females will have during the simulation. [default: 4]\n\n";
    exit(0);
}

int argumentParser(int argc, char *argv[], int* male_pop, int* female_pop, int* final_population, float* male_ratio, int* anp, int* ano) {

    if (argc == 1) return 0;

    for (int i=1; i <= argc; ++i) {
        std::vector<char *> tokens;
        char* pch;
        pch = strtok(argv[i], "=");
        while (pch != NULL) {
            tokens.push_back(pch);
            pch = strtok(NULL, "=");
        }
        if (tokens.size() == 0) break;

        if (strcmp(tokens[0], "--male-pop") == 0) *male_pop = atoi(tokens[1]);
        else if (strcmp(tokens[0], "--female-pop") == 0) *female_pop = atoi(tokens[1]);
        else if (strcmp(tokens[0], "--final-pop") == 0) *final_population = atoi(tokens[1]);
        else if (strcmp(tokens[0], "--male-ratio") == 0) *male_ratio = atof(tokens[1]);
        else if (strcmp(tokens[0], "--average-partners") == 0) *anp = atoi(tokens[1]);
        else if (strcmp(tokens[0], "--average-offspring") == 0) *ano = atoi(tokens[1]);
        else if ((strcmp(tokens[0], "--help") == 0) || (strcmp(tokens[0], "-h") == 0)) printHelp();
        else {
            std::cerr << "ERROR: Unknown command line option " << tokens[0] << "\n";
            std::cerr << "Exiting with error...\n";
            exit(1);
        }

    }
    return 0;
}

int main(int argc, char *argv[]) {

    int rc = 0;
    int male_pop = 5;
    int female_pop = 5;
    int final_population = 100;
    float male_ratio = 0.5;
    int ano = 4;
    int anp = 3;
    rc = argumentParser(argc,argv,&male_pop,&female_pop,&final_population,&male_ratio,&anp,&ano);
    
    assert(male_pop + female_pop < final_population);
    assert(male_ratio >= 0.0 && male_ratio <= 1.0);
    assert(ano >= 0 && ano < 7);
    assert(anp >= 0 && anp < 6);
    Population newpop(male_pop,female_pop,final_population,male_ratio,anp,ano);
    rc = newpop.initiatePopulation();
    rc = newpop.breedPopulation();
    newpop.printPopulation();

    return rc;
}
