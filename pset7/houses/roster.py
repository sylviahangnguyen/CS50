#! /usr/bin/env python3

# then run chmod a+x in linux command line
# then using ./[file].py to execute the python file
from sys import argv, exit
from cs50 import SQL

# give access to students.db
db = SQL("sqlite:///students.db")

# check command-line arguments number
if (len(argv) != 2):
    print("python import.py [House name]")
    exit(1)

# check whether the house name is correct
elif argv[1] not in ['Slytherin', 'Gryffindor', 'Hufflepuff', 'Ravenclaw']:
    print("""House name must be one of the followings:
    \n- Slytherin\n- Gryffindor\n- Hufflepuff\n- Ravenclaw\n""")
    exit(1)

# select columns from the table "students", store the data in rows
rows = db.execute("""SELECT first, middle, last, birth
            FROM students
            WHERE house = ?
            ORDER BY last
            """, argv[1])

# iterate all the rows
for row in rows:
    # if the middle name is NULL in database, print only first+last
    if row['middle'] == None:
        print(row['first'] + ' ' + row['last']
              + ', born', row['birth'])
    # if the middle name is not NULL in database, print all
    else:
        print(row['first'] + ' ' + row['middle'] + ' ' + row['last']
              + ', born', row['birth'])
