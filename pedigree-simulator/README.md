# pedigree-simulator
## A program to create a synthetic pedigree

### Compiling the program:

		g++ pedigreesim.cpp population.cpp -o pedigreesim

### Execution:

		./pedigreesim --male-pop=<int> --female-pop=<int> --final-pop=<int> --male-ratio=<float> --average-partners=<int> --average-offspring=<int> > output.csv

For help, execute: `./pedigreesim --help` or `./pedigreesim -h`

### Input parameters:

- --male-pop (*int*): Size of the male founder population. Default: 5.

- --female-pop (*int*): Size of the female founder population. Default: 5.

- --final-pop (*int*): Size of the final population. Default: 100. The program stops upon reaching the desired population size without finishing the initiated generation.

- --male-ratio (*float*): Fraction of the male population generated during the simulation. Default: 0.5. Probably keep this number around 0.5, but you can experiment.

- --average-partners (*int*): Average number of partners males will have during the simulation. Default: 3.

- --average-offspring (*int*): Average number of offspring females will have during the simulation. Default: 4.

### Comments:

The program starts with an initial population of the desired size and composition and at each generation it creates new members. Each member has an assigned ID, maternal and paternal IDs, which its generation, sex, number of partners (for males) and number of offspring (for females). At the beginning of a new generation, the program randomly assigns a new number of partners to each existing male and a number of offspring to each existing female according to the normal distribution with `mean = --average-partners` or `--average-offspring` and `sigma = 2.0`. Number of partners/offspring for each member can be between and including 0 and 6, so please choose `--average-partners` and `--average-offspring` whithin that range. 
Thereafter the program adds new members to the population accordingly. Each female can generate offspring in 2 consecutive subsequent generations, afterwards it stops. For males there is no such constraint. The execution ends when `--final-pop` is reached.
 
The program throws the final pedigree in CSV format to the STDOUT which can be redirected to a file. YOB for each individual is given as 2000 (chosen arbitrarily) + the generation the individual belongs to. Comments during the execution are directed to STDERR.
