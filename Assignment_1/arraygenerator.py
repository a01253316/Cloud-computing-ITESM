import random

# Generate 1000 random integers between 0 and 1000
numbers = [random.randint(0, 1000) for _ in range(1000)]

# Save to file (one number per line)
with open("arrayA.txt", "w") as f:
    for num in numbers:
        f.write(f"{num}\n")
