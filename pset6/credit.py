from cs50 import get_string

number = get_string("Number: ")
length = len(number)
ok = 1
sum1 = 0
sum2 = 0
count = 0

for i in number[::-1]:
    c = int(i)
    if ok == 1:
        sum1 += c
    else:
        c *= 2
        sum2 += c % 10
        c = int(c / 10)
        if c > 0:
            sum2 += c
    ok = 1 - ok
sum1 += sum2

if sum1 % 10 == 0:
    if len(number) == 15 and (int(number[0:2]) == 34 or int(number[0:2]) == 37):
        print("AMEX")
    elif len(number) == 16 and (int(number[0:2]) >= 51 and int(number[0:2]) <= 55):
        print("MASTERCARD")

    elif (len(number) == 13 or len(number) == 16) and int(
        int(number[0:2]) / 10
    ) % 10 == 4:
        print("VISA")

    else:
        print("INVALID")
else:
    print("INVALID")
