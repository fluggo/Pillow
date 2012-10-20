# generate manifest file

import string

out = open("MANIFEST", "w")

for line in open("CONTENTS").readlines():
    line = line.strip()
    if line:
        line = line.split("Imaging/", 1)
        out.write(line[1] + "\n")

out.close()
