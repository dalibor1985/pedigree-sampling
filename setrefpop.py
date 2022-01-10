import argparse
import pandas as pd 
import sys

def argparser():
    
    # check Python version
    pyver = sys.version_info
    if pyver[0] < 2 or pyver[0] == 2 and pyver[1] < 7:
        exit('ERROR: Python >= 2.7 required.')

    parser = argparse.ArgumentParser(description='Script for adding live or not column to a pedigree',
                               epilog='Thank you for using it!',
                               usage='%(prog)s [OPTIONS] ',
                               fromfile_prefix_chars='@')

    parser.add_argument('-p', '--pedigree', dest='pedigree', metavar='PEDIGREE', type=str,
                        help='''Specify the input pedigree file.''')

    parser.add_argument('-o', '--output', dest='output', metavar='OUTPUT',type=str,
                        help='''Specify the output pedigree file.''')

    parser.add_argument('-y', '--year-live', dest='year', metavar='YEAR', type=int,
                        default=0,
                        help='''Specify starting year for live individuals. [default: %(default)s]''')

    parser.add_argument('-g', '--gender-live', dest='gender', metavar='GENDER', type=str,
                        default='both',
                        help='''Specify which gender is denoted live. Possible values: male, female, both [default: %(default)s]''')

    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(0)
    args = parser.parse_args()

    if (not args.pedigree):
        print("ERROR: Please specify the input pedigree file!")
        parser.print_help()
        sys.exit(0)

    pedigree = args.pedigree

    if (not args.output):
        output = pedigree + '.out'
    else:
        output = args.output
    
    year = args.year
    gender= args.gender

    if gender != 'both' and gender != 'male' and gender != 'female':
        print("ERROR: Unrecognized value for gender. Permitted values are: both, male and female!")
        parser.print_help()
        sys.exit(0)
    
    return pedigree, output, year, gender

def main():

    pedigree, output, year, gender = argparser()

    data = pd.read_csv(pedigree,index_col=0, usecols=['x','ID','father','mother','YOB','gender'])

    data['live'] = 1
    if gender == 'male':
        data.loc[(data.gender == 2),'live'] = 0
    elif gender == 'female':
        data.loc[(data.gender == 1), 'live'] = 0

    data.loc[(data.YOB < year), 'live'] = 0

    data.to_csv(output)

    
if __name__ == "__main__":
    main()
