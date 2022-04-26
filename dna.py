from sys import argv
from csv import DictReader

def main():
    #Ensure correct usage
    if len(argv) < 3 or len(argv) > 3:
        print("Usage: python dna.py data.csv sequence.txt")
    
    #Open csv and dna sequence files, read contents into memory
    #Database file
    csvdata = []
    with open(argv[1]) as database:
        reader = DictReader(database)
        for row in reader:
            person = row
            csvdata.append(person)
    #Sequence file
    dna = ""
    with open(argv[2]) as sequence:
        dna = sequence.read()
    
    #Compute STR counts
    #Add str's to list
    STR = []
    STRd = {}
    for key in csvdata[0]:
        STR.append(key)
    #Check the amount of each STR sequence found in the dna & add to a dictionary
    for i in range(1, len(STR), 1):
        result = strcount(STR[i], dna, 0, 0)
        STRd[STR[i]] = result
        
    #Compare the STR counts to each person in the csv file
    #Same counter
    samec = 0
    found = False
    #Loop
    for person in range(len(csvdata)):
        for s in range(len(STR)):
            #If both STR values are the same add to same counter
            if csvdata[person][STR[s]] == STRd[STR[s]]:
                samec += 1
        if samec == len(STR):
            print(csvdata[person][name])
            found = True
            break
        else:
            samec = 0
    if found == False:
        print("No match")

#strcount function to calculate the highest STR sequence count in the strand of dna
#STR is the STR sequence we are trying to find, dna is the entire dna strand, count and i are to keep track of numbers for recursion purposes keep set to 0 when calling the function
def strcount(STR, dna, count, i):
    #If no STR sequences are found
    if count < 1:
        #Max sequence length counter
        shigh = 0
        #Loop through each position looking for str sequence
        for n in range(len(dna)):
            #If letter equals first letter in STR
            if dna[n] == STR[0]:
                #If STR sequence is found
                if dna[n : n + 4] == STR:
                    #Recursively call itself with one sequence found & add the result if it beats the previous high record
                    result = strcount(STR, dna, count + 1, n + 4)
                    if result > shigh:
                        shigh = result
        #Return the biggest STR sequence
        return shigh
                    
    #If 1+ STR sequences are found because of recursion
    if count > 0:
        #Check if there is another sequence coming after
        if dna[i : i + 4] == STR:
            #Call recursion to continiously keep checking
            strcount(STR, dna, count + 1, i + 4)
        else:
            #Return the amount or STR sequences found in a row
            return count
main()    
