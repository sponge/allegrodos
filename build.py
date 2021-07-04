import os
import sys
import shutil
import re
import textwrap
import glob

COMPILER = "i586-pc-msdosdjgpp-gcc"
SRC_GLOB = "src/**"
BIN_DIR = "bin"
BIN_NAME = "launch.exe"
EMBED_GLOB = "src/embed/*"
TEMPSRC_DIR = ".tempsrc"

CFLAGS = ["-O2", "-Wall", "-s"]
DLIBS = ["alleg"]
DLIBDIRS = ["lib"]
DEFINES = []
INCLUDES = ["src", TEMPSRC_DIR]


def make_c_include(name, data):
    name = re.sub("[^a-z0-9]", "_", name.lower())
    res = "static const char " + name + "[] = {"
    for c in data:
        res += str(ord(c)) + ", "
    res = res.rstrip(", ") + "};"
    return name, textwrap.fill(res, width=99)


def main():
    os.chdir(sys.path[0])

    if not os.path.exists(BIN_DIR):
        os.makedirs(BIN_DIR)

    if not os.path.exists(TEMPSRC_DIR):
        os.makedirs(TEMPSRC_DIR)

    embedded_files = glob.glob(EMBED_GLOB, recursive=True)

    for filename in embedded_files:
        name = os.path.basename(filename)
        name, text = make_c_include(name, open(filename).read())
        open(f'{TEMPSRC_DIR}/{name}.h', "wb").write(text)

    cfiles = [x for x in glob.glob(SRC_GLOB, recursive=True) if x.lower().endswith(('.c'))]

    opt = {
        "compiler": COMPILER,
        "flags": " ".join(CFLAGS),
        "defines": " ".join(map(lambda x: "-D " + x, DEFINES)),
        "includes": " ".join(map(lambda x: "-I " + x, INCLUDES)),
        "outfile": BIN_DIR + "/" + BIN_NAME,
        "srcfiles": " ".join(cfiles),
        "libs": " ".join(map(lambda x: "-l" + x, DLIBS)),
        "libdirs": " ".join(map(lambda x: "-L" + x, DLIBDIRS)),
        "argv": " ".join(sys.argv[1:])
    }
    cmd = "{compiler} {flags} {defines} {includes} -o {outfile} {srcfiles} {libdirs} {libs} {argv}".format(**opt)

    print("compiling...")
    print(cmd)
    res = os.system(cmd)

    print("deleting temporary files...")
    if os.path.exists(TEMPSRC_DIR):
        shutil.rmtree(TEMPSRC_DIR)

    print("done" + (" with errors" if res else ""))


if __name__ == "__main__":
    main()
