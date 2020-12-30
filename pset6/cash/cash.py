#! /usr/bin/env python3
# then run chmod a+x in linux command line
# then using ./[file].py to execute the python file

from cs50 import get_float

while True:
    dollars = get_float("Change owed: ")
    if dollars >= 0:
        break

# Convert dollars into cents
cents = round(dollars * 100)
coins = 0
quarter = 0
dime = 0
nickel = 0
penny = 0

# Can I use quarters?
while cents >= 25:
    coins += 1
    quarter += 1
    cents -= 25

# Can I use dimes?
while cents >= 10:
    coins += 1
    dime += 1
    cents -= 10

# Can I use nickels?
while cents >= 5:
    coins += 1
    nickel += 1
    cents -= 5

# Can I use pennies?
while cents >= 1:
    coins += 1
    penny += 1
    cents -= 1

print(f"I have {coins} coin(s)")
print(f"{quarter} quarter(s)")
print(f"{dime} dime(s)")
print(f"{nickel} nickel(s)")
print(f"{penny} penny(pennies)")
