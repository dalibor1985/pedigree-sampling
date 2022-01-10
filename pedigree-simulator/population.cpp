#include<iostream>
#include<sstream>
#include<vector>
#include<ios>
#include<iomanip>
#include <math.h>
#include <algorithm> // std::shuffle
#include <random>    // std::default_random_engine
#include <chrono>    // std::chrono::system_clock
#include "population.h"

std::string pad_with_zeros(int number) {
    std::ostringstream out;
    out << std::internal << std::setfill('0') << std::setw(10) << number;
    return out.str();
}

std::vector<int> generate_normal_random(int mean, int size) {
    const float fmean = static_cast<float>(mean);
    const float sigma = 2.0;
    const int min_value = 0;
    const int max_value = 6;
    std::vector<int> values;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::normal_distribution<double> distribution(fmean, sigma);

    while (values.size() < size) {
        double number = distribution(generator);
        if ((int(number) >= min_value) && (int(number) <= max_value)) {
            values.push_back(int(number));
        }
    }
    return values;
}

template <typename T>
void shuffle_vector(std::vector<T> *members)
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(members->begin(), members->end(), std::default_random_engine(seed));
}

Population::Population(int x, int y, int z, float w, int q, int p)
{
    std::cerr << "Calling class constructor...\n";
    male_pop = x;
    female_pop = y;
    final_population = z;
    male_ratio = w;
    avrg_number_of_partners = q;
    avrg_number_of_offspring = p;
    std::cerr << "male_pop = " << male_pop << "\n";
    std::cerr << "female_pop = " << female_pop << "\n";
    std::cerr << "final population = " << final_population << "\n";
    std::cerr << "male_ratio = " << male_ratio << "\n";
    std::cerr << "avrg_number_of_partners = " << avrg_number_of_partners << "\n";
    std::cerr << "avrg_number_of_offspring = " << avrg_number_of_offspring << "\n";
}

Population::Member Population::makeMember(int id_number, int generation, int sex, std::string fatherID, std::string motherID) {
    //std::cerr << "Making a new member...\n";
    Member new_member;
    std::string ID = "ID";
    ID += pad_with_zeros(id_number);
    new_member.ID = ID;
    new_member.generation = generation;
    new_member.sex = sex;
    new_member.number_of_offspring = 0;
    new_member.number_of_partners = 0;
    if (fatherID.size() > 0){
        new_member.father = fatherID;
    }
    if (motherID.size() > 0) {
	    new_member.mother = motherID;
    }
    //std::cerr << "Created a new member, ID: " << new_member.ID;
    //std::cerr << ", descends from " << fatherID << " and " << motherID << "\n";

    return new_member;
}

int Population::initiatePopulation() {
    std::cerr << "Creating initial population, size: \n";
    std::cerr << "\tmales: " << male_pop << "\n";
    std::cerr << "\tfemales: " << female_pop << "\n";
    for (int i = 0; i < male_pop; i++)
        males.push_back(makeMember(i + 1, 0, 1, "", ""));
    for (int i = male_pop; i < male_pop+female_pop; i++)
        females.push_back(makeMember(i + 1, 0, 2, "", ""));
    std::cerr << "Size of created male population: " << males.size() << "\n";
    std::cerr << "Size of created female population: " << females.size() << "\n";
    return 0;
}

void Population::distributeOffspring() {
    std::cerr << "New normal distribution of offspring...\n";
    std::vector<int> offsprings = generate_normal_random(avrg_number_of_offspring,females.size());
    for (int i = 0; i < females.size(); i++) {
        females[i].number_of_offspring = offsprings[i];
    }
}

void Population::distributePartners() {
    std::cerr << "New normal distribution of partners...\n";
    std::vector<int> partners = generate_normal_random(avrg_number_of_partners, males.size());
    for (int i = 0; i < males.size(); i++) {
        males[i].number_of_partners = partners[i];
    }
}

/* 
   Function that simulates breeding
*/
int Population::breedPopulation() {
    //initiate first generation
    int generation = 1;
    while (males.size() + females.size() <= final_population) {
        std::cerr << "Adding new generation nr. " << generation << " to the population...\n";
        // shuffle the vectors to obtain randomness in pairing of males and females
        shuffle_vector(&males);
        shuffle_vector(&females);
        distributeOffspring();
        distributePartners();
        // initiate temporary vector for new members
        std::vector<Member> tmp_members;
        std::cerr << "Creating new members...\n";
        
        // begin_female: it keeps updated the first female to pair with the next male
        int begin_female = 0;

        // loop over all males in the population
        for (int i = 0; i < males.size(); i++) {
            // if there are no available females left, leave the loop
            if (begin_female >= females.size()) break;
            if (males[i].number_of_partners == 0) continue;
            //if (males[i].generation < generation - 3) continue;
            // iterator j keeps track of how many females have been iterated after begin_female
            int j = 0;
            // keeps track how many females has male[i] fertilized
            int fertilized_females = 0;
            while (fertilized_females < males[i].number_of_partners) {
                // if there are no available females left, leave the loop
                if (begin_female+j >= females.size()) break;
                // females can give offspring in 2 generations only or it can have 0 offspring for this generation
                // in this case increase j and move on to the next female
                if ((females[begin_female + j].generation < generation - 2) || (females[begin_female + j].number_of_offspring == 0)) {
                    ++j;
                    continue;
                }
                // loop over possible offspring for this female
                for (int k = 1; k < females[begin_female+j].number_of_offspring+1; k++) {
                    int sex;
                    // first we create new males according to male_ratio
                    if (k <= int(floor(females[begin_female + j].number_of_offspring * male_ratio))) sex = 1;
                    // then we create females
                    else sex = 2;
                    Member new_member = makeMember(males.size()+females.size()+tmp_members.size()+1,generation,sex,males[i].ID,females[begin_female+j].ID);
                    // add new members to the temporary vector
                    tmp_members.push_back(new_member);
                }
                // fertilization occured, increase both j and fertilized_females
                ++j;
                ++fertilized_females;
            }
            // update begin_female for the next male
            begin_female += j;
        }
        // if there are no new members created leave the function
        if (tmp_members.size() == 0) {
            std::cerr << "No new members created in this generation.\nStopping the simulation...\n";
            return 1;
        }
        std::cerr << "Adding new " << tmp_members.size() << " created members to the initial population...\n";
        // add newly created members to the main population according to their sex
        // if the desired population size is reached, leave the loop
        for (int i = 0; i < tmp_members.size(); i++) {
            if (tmp_members[i].sex == 1) males.push_back(tmp_members[i]);
            else if (tmp_members[i].sex == 2) females.push_back(tmp_members[i]);
            if (males.size() + females.size() == final_population) break;

        }
        std::cerr << "Now the male population has " << males.size() << " members.\n";
        std::cerr << "Now the female population has " << females.size() << " members.\n";
        // if the desired population size is reached, leave the loop, otherwise, go to the next generation
        if (males.size() + females.size() == final_population) break;
	    generation += 1;
    }
    shuffle_vector(&males);
    shuffle_vector(&females);
    return 0;
}

/*
  Print population to STDOUT
  Since we are dealing with 2 vectors and we don't want to print 
  males and females separately, we create an auxiliary vector of indices,
  shuffle it and then randomly print either males or females
*/
void Population::printPopulation() {
    //ConcatVector<int> members(males, females);
    std::vector<int> indices;
    for (int i = 0; i < males.size()+females.size(); i++) indices.push_back(i);
    shuffle_vector(&indices);
    std::cerr << "Printing the final population to STDOUT...\n";
    std::cout << "x,ID,father,mother,YOB,gender,\n";
    for (int i = 0; i < indices.size(); i++) {
        if (indices[i] < males.size()) {
            std::cout << i + 1 << "," << males[indices[i]].ID;
            std::cout << "," << males[indices[i]].father;
            std::cout << "," << males[indices[i]].mother;
            std::cout << "," << 2000 + males[indices[i]].generation;
            std::cout << "," << males[indices[i]].sex << "\n";
        }
        else {
            std::cout << i + 1 << "," << females[indices[i]-males.size()].ID;
            std::cout << "," << females[indices[i]-males.size()].father;
            std::cout << "," << females[indices[i]-males.size()].mother;
            std::cout << "," << 2000 + females[indices[i]-males.size()].generation;
            std::cout << "," << females[indices[i]-males.size()].sex << "\n";
        }
        
    }
}

Population::~Population() {
    male_pop = 0;
    female_pop = 0;
    final_population = 0;
    male_ratio = 0.0;
    males.clear();
    females.clear();
    std::cerr << "Class destructor called, population deleted" << std::endl;
}
