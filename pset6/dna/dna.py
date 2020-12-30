#! /usr/bin/env python3
# then run chmod a+x in linux command line
# then using ./[file].py to execute the python file

import re, csv
from sys import argv, exit

def main():
    # print an error message when the number of argvs is incorrect
    if len(argv) != 3:
        print("./dna.py [database].csv [sequence].txt")
        exit(1)

    # open the CSV file and read its contents into memory
    data_file = open(argv[1], "r")
    database = csv.DictReader(data_file)

    # the first row of the CSV file will be the column names
    headers = database.fieldnames
    STRs = headers[1:]

    # open the DNA sequence and read its contents into memory
    sequence_file = open(argv[2], "r")
    sequence = sequence_file.read().rstrip("\n")
    counted = [0] * len(STRs)
    for i in range(len(STRs)):
        repeated = re.findall(fr'(?:{STRs[i]})+', sequence)
        try:
            longest = max(repeated, key = len)
        except ValueError:
            print("No match")
            exit(1)
        counted[i] = len(longest) // len(STRs[i])

    for row in database:
        count = []
        for column in row[1:]:
            count.append(int(column))
        if count == counted:
            print(row[0])
            exit(1)
    print("No match")

    data_file.close()
    sequence_file.close()

if __name__ == "__main__":
    main()