import csv
import sys


def main():
    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read database file into a variable
    table = []
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for row in reader:
            table.append(row)

    #  Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as seq:
        sample = seq.read()

    #  Find longest match of each STR in DNA sequence
    person = []
    strs = []
    for key in list(table[0])[1:]:
        strs.append(key)
        person.append(longest_match(sample, key))
    # print (person)
    # Check database for matching profiles
    ok = True
    for row in table:
        cnt = 0
        for i in range(len(strs)):
            if int(row[strs[i]]) == int(person[i]):
                cnt += 1
        if cnt == len(strs):
            ok = False
            print(row["name"])
            # return
    if ok == True:
        print("No match")


def longest_match(sequence, subsequence):
    # Returns length of longest run of subsequence in sequence.

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
