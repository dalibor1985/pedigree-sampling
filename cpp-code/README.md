# C++ code for pedigree sampling


For the interested users, we provide the essential C++ code in preprocess.cpp and main.cpp. Although the Python scripts are easier to read and more user friendly, the C++ programs can be faster, esspecially for larger inputs. This may be of interest for users with experience in C++ coding.

Unlike Python scripts, C++ code considers the entire pedigree as reference population, thus it will not consider the "live" column if it is present in pedigree.

## Usage

		cat inputfilename | ./exefilename > outputfilename

## Notes

### preprocess.cpp


The gender is implied from the column position. This is controlled in line 58. In the current version the preprocessing is done for maternal lines if the first columns of the input file are: "index, ID, father, mother". 

### main.cpp


The value of K is hardcoded in line 212. Currently it is set to 500. The program by default calls both methods, greedy and optimal. 



