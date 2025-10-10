import random


typeOfCreatingNewRecords = input("To append new records input 'a', to override input 'w'")

numberOfRecords = int(input("Give number of numbers: "))
minOfDigits = int(input("Minimal number of digits: "))
maxOfDigits = int(input("Maximal number of digits: "))

file = open("exampleData", typeOfCreatingNewRecords)

def generateNumber():
    newNumber = ""
    nmbOfDigits = random.randint(minOfDigits, maxOfDigits)
    isFloat = random.randint(0,1)
    pointPlace = -1
    if (isFloat==1):
        pointPlace = random.randint(1,nmbOfDigits)
    isNegative = random.randint(0,1)
    if(isNegative==1):
        newNumber+="-"
    newNumber+=str(random.randint(1,9))
    for i in range(1,nmbOfDigits):
        if i == pointPlace:
            newNumber+="."
        newNumber += str(random.randint(0,9))
    file.write(newNumber+"\n")

for i in range(0, numberOfRecords):
    generateNumber()

file.close()