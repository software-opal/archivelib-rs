import pathlib
import subprocess


def inversion(input):
    input = [*input]
    out = {}
    for fname, content in input:
        if content not in out:
            out[content] = []
        out[content] += [fname]

    lines = [f"  {start} :: {', '.join(files)}" for start, files in sorted(out.items())]
    return f"({len(out)} uniques / {len(input)} files)\n" + "\n".join(lines)


baseFile = bytes.fromhex("000328044bfe26f30f8013")

for modIndex in range(len(baseFile)):
    before = baseFile[0:modIndex]
    replaced = baseFile[modIndex]
    after = baseFile[modIndex + 1 :]

    baseDir = f"decomp/{modIndex:02}"
    subprocess.run(["rm", "-rf", baseDir], check=True)
    subprocess.run(["mkdir", "-p", f"{baseDir}/binary", f"{baseDir}/hex"], check=True)

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

    fileDecompressContent = {}

    for btwn in range(0, 256):
        fileName = f"{btwn:02X}"
        if btwn == replaced:
            fileName += "-original"
        between = bytes.fromhex(f"{btwn:02X}")
        input = before + between + after
        try:
            with open(f"{baseDir}/binary/{fileName}.bin", "wb") as out, open(
                f"{baseDir}/binary/{fileName}.err", "wb"
            ) as err:
                subprocess.run(
                    ["../target/debug/cli", "-d"],
                    input=input,
                    stdout=out,
                    stderr=err,
                    universal_newlines=False,
                    check=True,
                )
            pathlib.Path(f"{baseDir}/binary/{fileName}.err").unlink()
            with open(f"{baseDir}/binary/{fileName}.bin", "rb") as f:
                fileDecompressContent[fileName] = f.read().hex()
            convert_to_hex(
                f"{baseDir}/binary/{fileName}.bin", f"{baseDir}/hex/{fileName}.hex"
            )
        except subprocess.CalledProcessError as e:
            pathlib.Path(f"{baseDir}/binary/{fileName}.bin").unlink()
            error = f"{e}".split("\n")
            print(f"Unable to decompres {btwn:02X}: {error}")

    with open(f"{baseDir}/summary.txt", "w") as f:
        f.write(inversion(fileDecompressContent.items()))


# content = bytes.fromhex('62 62 62')
# r = subprocess.run(
#   ["../target/debug/cli", "-d"],
#   input=baseFile,
#   universal_newlines=False,
#   check=True,
#   capture_output=True
# )

# print(r.stderr.decode("utf-8"))
# print(r.stdout.hex())

# print("---------")
# content = bytes.fromhex('62 62 62')
# content = b'ab'
# r = subprocess.run(
#   ["../target/debug/cli", "-V"],
#   input=content,
#   universal_newlines=False,
#   check=True,
#   capture_output=True
# )

# print(r.stderr.decode("utf-8"))
# print(r.stdout.hex())
