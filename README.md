# Pedigree sampling
A suite of Python3 scripts for the selection of individuals for sampling, based on a pedigree data. The individuals are selected based on the criterion of the largest sum of mutual distances in a pedigree. The distances are calculated for the single gender lineages. The selected individuals should offer the best coverage of mitochondrial DNA or Y chromosome variability in a population.

## setrefpop.py 
	
Used to annotate idividuals that belong to the reference population. A standard case is to restrict the referenece population to individuals born on and after a given reference year. When dealing with Y chromosome, the reference population must be restricted exclusively to male individuals. This can be combined with a reference year.
This script is optional, if the reference population is not defined, the preprocess.py and main.py treat all individuals in a pedigree as the reference population.

#### Input:

Input is a pedigree file in comma-separated values format. The file must contain at least five columns named: "x", "ID", "father", "mother", "YOB", and "gender". "x" denotes an index column; inputs in "ID", "father" and "mother" columns are treated as strings, therefore any naming format is acceptable; gender must be denoted numerically with "1" for male and "2" for female individuals. 

#### Output:

Output is the pedigree file with added new column named "live" where "1" denotes that the individual belongs to reference population and "0" that it does not.

#### Usage:

		setrefpop.py [OPTIONS]

- -p PEDIGREE; input pedigree file name
- -y YEAR; reference year - all individuals born in that year and later will be included in the reference population
- -g GENDER; should be set to "male" if dealing with Y chromosome variability, the default is "female"
- -o OUTPUTFILENAME; output pedigree file name, the default is inputfilename.out
- -h HELP

#### Examples:

		python3 setrefpop.py -p filename.csv -y 2000

Sets reference population to all individuals born in and after year 2000. Output is stored in filename.csv.out.

		python3 setrefpop.py -p filename.csv -g male -y 2000 -o filename.refpop.csv

Sets reference population to all male individuals born in and after year 2000. Output is stored in filename.refpop.csv.


## preprocess.py

Transforms the input pedigree file into a graph file represented with the list of edges.

#### Input:

Input is a pedigree file in CSV format. It can be the output file of setrefpop.py program, or not. The file must contain at least "ID", "father", and "mother" columns, and any naming format for the individuals is supported. If the optional "live" column is present, the reference population is restricted to the annotated individuals.

#### Output:

Output is a graph represented as the list of edges between vertices (individual/parent). The reference population is listed at the end of the output graph. If the reference population is not defined, all individuals in the pedigree are regarded as the reference population and listed at the end. The output is streamed to stdout.

#### Usage: 

		preprocess.py [-h] [--parent PARENT] filename

`PARENT` should be set to `father` if dealing with Y chromosome variability; the default value is `mother`.
Output is streamed to stdout.

#### Examples:

		python3 preprocess.py filename > filename.graph

Produces a graph file `filename.graph` with edges between individuals and their mothers. Used for the analysis of mitochondrial DNA variablity. The reference population is listed at the end of the file. If the reference population is not defined, all individuals are included.

		python3 preprocess.py --parent PARENT filename.csv > filename.graph

Produces a graph file `filename.graph` with edges between individuals and their fathers. Used for the analysis of Y chromosome variablity. The reference population is listed at the end of the file. If the reference population is not defined, all individuals are included.


## main.py

Based on the graph representation of a pedigree finds the set of K individuals with the maximization of the sum of mutual distances as a criterion. Allows for a choice between a greedy and an optimal method. We recomend the greedy method as it is faster while the result is very close to optimal.

#### Input:

Input is a graph file produced with `preprocess.py`. The user must specify input filename, the number of samples K, and the method (greedy/optimal).

#### Output:

Output is a list of K* selected individuals. K* is usually less than K as a result of the rounding-off error that occurs when K is proportionally distributed over pedigreee lineages. The list of selected individuals is preceeded with the  designation of the used method, the cumulative score, and time needed for the processing. The output is streamed to stdout.

#### Usage:

		main.py filename K method [greedy/optimal]

#### Examples:
	
		python3 main.py filename.graph 1000 greedy > samples1

		python3 main.py filename.graph 1000 optimal > samples2

The files `samples1` and `samples2` will contain partially different set of individuals, the scores will be slightly better with the optimal method, and the processing will be faster with the greedy method.


## A reminder: 

The default procedures are used for the mitochondrial DNA variability analysis.

For the analysis of the Y chromosome variability two steps are required: 

1. The option `-g male` must be used with `setrefpop.py`. 
2. The option `--parent father` must be used with `preprocess.py`.

