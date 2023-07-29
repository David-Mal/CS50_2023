from cs50 import get_int

height = get_int("Height: ")
while height < 1 or height > 8:
    height = get_int("Height: ")

for i in range(1, height + 1):
    j = height - i
    for a in range(j):
        print(" ", end="")
    for a in range(i):
        print("#", end="")
    print("  ", end="")
    for a in range(i):
        print("#", end="")
    print()
