#!/bin/python3
import sys
import os
from typing import Tuple

cwd = os.getcwd()
if len(sys.argv) == 1:
    print("No Input file provided")
    sys.exit(1)

md_file = sys.argv[1]
out_file = "toc-" + md_file.strip()
output = "Table of Contents\n"
orig_content = ""

def get_line_and_check_space(line: str) -> Tuple[str,bool] | None:
    if not line:
        return None
    ret = ""
    ptr = 0
    hasSpace = False
    while ptr < len(line):
        if line[ptr] == '\n':
            break
        if line[ptr] == ' ':
            hasSpace = True
        ret+=line[ptr]
        ptr+=1
    return (ret,hasSpace)

with open(f"{cwd}/{md_file}") as file:
    while True:
        line = file.readline()
        if not line:
            break
        orig_content+=line
        if line[0] == '#':
            ptr = 1
            while  line[ptr] == '#':
                ptr+=1
            res = get_line_and_check_space(line[ptr:].strip())
            if not res:
                continue

            (name, linkSpace) = res
            name = name.strip()
            pad = '' if ptr == 1 else '\t' * (ptr-1)
            output+=f"{pad}* [{name}]({ '<' if linkSpace else '' }{'#'*ptr}{name}{ '>' if linkSpace else '' })\n"

with open(f"{cwd}/{out_file}",'w') as file:
    file.write(f"{output}{orig_content}")
