# This is a simple program in which a user can enter 
# in a list of numbers and a summary of the numbers is printed out
# @author Jacob McLain

print("Hello!!")
print("This is a program in which I will read numbers and automatically sort them")

numbers = []

while True:
    user_input = input("Enter a number (or anything else to stop): ")

    if not user_input.isnumeric():
        break

    numbers.append(int(user_input))

numbers.sort()
print()
print()
print("--- Summary of Data ---")
print()
print("Sorted numbers:", numbers)

sum = 0
for nums in numbers:
    sum += nums
mean = sum / len(numbers)
print("Mean: ", mean)
if len(numbers) % 2 == 0:
    median = (numbers[len(numbers)//2] + numbers[len(numbers)//2 + 1]) / 2
else:
    median = numbers[len(numbers)//2]
print("Median: ", median)

from collections import Counter

counter = Counter(numbers)
mode, count = counter.most_common(1)[0]

print("Mode: ", mode, "   (It occurs", count, "times)")