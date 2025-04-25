# Save the code that generated the test case into a Python file
import random

# Generate the test-case file with 200 instructions
random.seed(123)

bucket_size = 7
total_instructions = 200

# Symbol pools
var_names = [f"v{i}" for i in range(1, 21)]
struct_names = [f"S{i}" for i in range(1, 11)]
union_names = [f"U{i}" for i in range(1, 11)]
func_names = [f"f{i}" for i in range(1, 11)]
number_names = [str(n) for n in range(1, 101)]
relops = ["<=", ">=", "==", "!=", "<", ">"]
simple_types = ["INT", "FLOAT", "BOOL", "STRING", "NUMBER"]
created_symbols = []

open_scopes = 0
close_scopes = 0

lines = []
for i in range(total_instructions):
    if i == total_instructions - 1:
        lines.append("Q")
        break

    # Decide allowed ops
    possible_ops = ["I", "L", "D", "P"]
    weights = [5, 2, 1, 1]
    if open_scopes - close_scopes < 5:
        possible_ops.append("S")
        weights.append(1)
    if open_scopes - close_scopes > 0:
        possible_ops.append("E")
        weights.append(1)
    op = random.choices(possible_ops, weights=weights, k=1)[0]

    if op == "S":
        lines.append("S")
        open_scopes += 1
    elif op == "E":
        lines.append("E")
        close_scopes += 1
    elif op == "P":
        lines.append(f"P {random.choice(['A','C'])}")
    elif op == "I":
        kind = random.choice(["VAR","FUNCTION","STRUCT","UNION","NUMBER","RELOP"])
        if kind == "VAR":
            sym = random.choice(var_names)
            lines.append(f"I {sym} VAR")
        elif kind == "FUNCTION":
            sym = random.choice(func_names)
            params = " ".join(random.choices(simple_types, k=random.randint(1, 4)))
            lines.append(f"I {sym} FUNCTION {params}")
        elif kind == "STRUCT":
            sym = random.choice(struct_names)
            fields = []
            for _ in range(random.randint(1, 4)):
                t = random.choice(simple_types)
                name = f"f{random.randint(1,99)}"
                fields.append(f"{t} {name}")
            lines.append(f"I {sym} STRUCT " + " ".join(fields))
        elif kind == "UNION":
            sym = random.choice(union_names)
            fields = []
            for _ in range(random.randint(1, 3)):
                t = random.choice(simple_types)
                name = f"u{random.randint(1,99)}"
                fields.append(f"{t} {name}")
            lines.append(f"I {sym} UNION " + " ".join(fields))
        elif kind == "NUMBER":
            sym = random.choice(number_names)
            lines.append(f"I {sym} NUMBER")
        elif kind == "RELOP":
            sym = random.choice(relops)
            lines.append(f"I {sym} RELOP")
        created_symbols.append(sym)
    elif op == "L":
        if random.random() < 0.2:
            sym = "X" + str(random.randint(100, 999))
        else:
            sym = random.choice(created_symbols)
        lines.append(f"L {sym}")
    elif op == "D":
        if random.random() < 0.2:
            sym = "Y" + str(random.randint(100, 999))
        else:
            sym = random.choice(created_symbols)
        lines.append(f"D {sym}")

# Write to file
file_path = "bin/mtestcase.txt"
with open(file_path, "w", newline="\n") as f:
    f.write(str(bucket_size) + "\n")
    f.write("\n".join(lines))

print(f"Test case written to {file_path}")

# # Save to a file
# code_file_path = "/mnt/data/generate_testcase.py"
# with open(code_file_path, "w") as f:
#     f.write(code)

# code_file_path