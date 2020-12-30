import csv
from sys import argv, exit
from cs50 import SQL

# give access to students.db
db = SQL("sqlite:///students.db")

# check command-line arguments
if (len(argv) != 2):
    print("python import.py [filename].csv")
    exit(1)

# open CSV file given by command-line argument
with open(argv[1], "r") as input_file:
    reader = csv.DictReader(input_file)
    # for each row, parse name
    for row in reader:
        # extract first, middle and last names from name
        first_name = row["name"].split()[0]
        last_name = row["name"].split()[-1]
        if len(row["name"].split()) >= 3:
            middle_name = row["name"].split()[1]
        else:
            middle_name = None

        # insert each student into the students table of student.db
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                                  first_name,
                                  middle_name,
                                  last_name,
                                  row["house"],
                                  row["birth"])






