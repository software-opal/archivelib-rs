import subprocess
import sys
import zlib

COUNTS = [2**8, 2**9]
FILES = [
    "aabb",
    "b",
    "c",
    # "d", "e", "f",
    # "j", "k", "l",
    # "m", "n", "o",
    # "u", "v", "w",
    # "x", "y", "z"
]


def fname(count, content):
    return f"{count:03}-{content.encode('ascii').hex()}"


fileContents = [
    (f"{fname(count, fileContent)}.txt", fileContent.encode("ascii") * count)
    for count in COUNTS
    for fileContent in FILES
]

# fileContents += [
#     (
#         f"{fname(leftCount, leftFileContent)}--{fname(rightCount, rightFileContent)}.txt",
#         (leftFileContent.encode("ascii") * leftCount)
#         + (rightFileContent.encode("ascii") * rightCount),
#     )
#     for leftFileContent in FILES
#     for rightFileContent in FILES
#     if leftFileContent != rightFileContent
#     for leftCount in COUNTS[0:3]
#     for rightCount in COUNTS[0:3]
# ]

# fileContents += [
#     (f"run-{content}-{len(content)}", content.encode("ascii"))
#     for content in (
#         ("a" * count) + suffix for suffix in ["", "b"] for count in range(0, 16)
#     )
# ]

# fileContents = [
#     (f"{content}", content.encode("ascii"))
#     for content in [
#         # 'abba' * 4,
#         # 'abab' * 4,
#         "aabb" * 4,
#         "aaaa" * 4,
#         "bbbb" * 4,
#     ]
# ]

fileContents = [(f, c) for c, f in ({c: f for f, c in fileContents}).items()]

subprocess.run(["rm", "-rf", "files/"], check=True)
subprocess.run(
    ["mkdir", "-p", "files/inputs", "files/binary", "files/hex", "files/output"],
    check=True,
)

with open("files/.gitignore", "w") as g:
    g.write("*\n**/*")


def convert_to_hex(origin, dest):
    with open(origin, "rb") as zip, open(dest, "w") as hex:
        for i, b in enumerate(zip.read()):
            if i == 0:
                None
            elif i % 16 == 0:
                hex.write("\n")
            elif i % 8 == 0:
                hex.write(" ")
            hex.write(f"{b:02X} ")


fileCompressContent = {}

for fileName, fileContent in fileContents:
    with open(f"files/inputs/{fileName}.bin", "wb") as file:
        file.write(fileContent)

    last_output = ""
    for level in range(0, 10):
        output = zlib.compress(fileContent, level=level)
        if output == last_output:
            continue
        with open(f"files/binary/{fileName}.z{level}", "wb") as out:
            out.write(output)
        last_output = output
        convert_to_hex(
            f"files/binary/{fileName}.z{level}", f"files/hex/{fileName}.z{level}.hex"
        )
        fileCompressContent[f"{fileName}.z{level}"] = output

    last_output = ""
    for level in range(0, 6):
        proc = subprocess.run(
            ["../target/debug/cli", "-c", f"-{level}", f"files/inputs/{fileName}.bin"],
            capture_output=True,
            universal_newlines=False,
            check=True,
        )
        output = proc.stdout
        if output == last_output:
            continue
        with open(f"files/binary/{fileName}.al{level}", "wb") as out:
            out.write(output)
        convert_to_hex(
            f"files/binary/{fileName}.al{level}", f"files/hex/{fileName}.al{level}.hex"
        )

        fileCompressContent[f"{fileName}.al{level}"] = output


def inversion(input):
    input = [*input]
    out = {}
    for fname, content in input:
        if content not in out:
            out[content] = []
        out[content] += [fname]

    lines = [f"  {start} :: {', '.join(files)}" for start, files in sorted(out.items())]
    return f"({len(out)} uniques / {len(input)} files)\n" + "\n".join(lines)


with open("files/cutoff.txt", "w") as f:
    for i in range(max(len(c) for c in fileCompressContent.values())):
        f.write(f"Cutoff {i}")
        f.write(inversion((f, c[:i].hex()) for f, c in fileCompressContent.items()))
        f.write("\n\n")

with open("files/index1.txt", "w") as f:
    for i in range(min(len(c) for c in fileCompressContent.values())):
        f.write(f"Index {i}")
        f.write(
            inversion(
                (f, c[:i].hex() + "__" + c[i + 1 :].hex())
                for f, c in fileCompressContent.items()
            )
        )
        f.write("\n\n")

with open("files/index2.txt", "w") as f:
    for i in range(min(len(c) for c in fileCompressContent.values()) - 1):
        f.write(f"Index {i}")
        f.write(
            inversion(
                (f, c[:i].hex() + "__" + c[i + 2 :].hex())
                for f, c in fileCompressContent.items()
            )
        )
        f.write("\n\n")
