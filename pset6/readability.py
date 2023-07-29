from cs50 import get_string

text = get_string("Text: ")
letters = 0
words = 1
sentences = 0
for i in text:
    if i.isalpha():
        letters += 1

    elif i == "." or i == "?" or i == "!":
        sentences += 1

    elif i == " ":
        words += 1

L = letters / words * 100
S = sentences / words * 100
index = 0.0588 * L - 0.296 * S - 15.8
ans = round(index)

if index >= 16:
    print("Grade 16+")

elif index < 1:
    print("Before Grade 1")

else:
    print(f"Grade {ans}")
