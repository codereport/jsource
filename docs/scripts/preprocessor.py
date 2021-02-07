#!/usr/bin/python3

"""
preprocessor for sphinx files
- reads in all .rst files in input directory
- writes processed .rst files to output directory

by copying files to output directory, we aren't
modifying the  original .rst files in any way.

preprocessor steps:
- format tables
"""

import argparse
import shutil
import os


def find_markdown_tables(lines):
    """
    return (row_start, row_end) for all markdown tables.
    """
    tpos = []
    md_table_idx = None
    for idx, line in enumerate(lines):
        if line.startswith("|"):
            if not md_table_idx:
                md_table_idx = idx
        else: # end of md table
            if md_table_idx is not None:
                tpos.append((md_table_idx, idx - 1))
                md_table_idx = None
    if md_table_idx is not None:
        tpos.append((md_table_idx, idx))
    return tpos


def get_markdown_row_data(row):
    """
    get row (or header) data from markdown row (or header)

    row
        | apples | red | a |

    data
        ['apples', 'red', 'a']
    """
    data = row.split("|")
    data = data[0:len(data)-1]
    data = [e.strip() for e in data if e.strip()]
    return data


def convert_table(table, in_format="md", out_format="rst-2"):
    """
    md format
        | fruit | color | startswith |
        | ----- | ----- | ---------- |
        | apples | red | a |
        | oranges | orange | o |
    
    rst-1 format
        +---------+--------+------------+
        | fruit   | color  | startswith |
        +=========+========+============+
        | apples  | red    | a          |
        +---------+--------+------------+
        | oranges | orange | o          |
        +---------+--------+------------+

    rst-2 format
        =======  ====== ==========
        fruit    color  startswith
        =======  ====== ==========
        apples   red    a
        oranges  orange o
        =======  =====  ==========
    """
    if in_format == "md" and out_format=="rst-2":
        header = get_markdown_row_data(table[0])
        # NOTE: skip table[1]
        rows = [get_markdown_row_data(row) for row in table[2:]]
        cols = [[row[col_idx] for row in rows] for col_idx in range(len(header))]
        rst_table = []
        separator = ' '.join(
            ['=' * max(len(header[col_idx]), len(max(col, key=len))) for col_idx, col in enumerate(cols)],
        )
        rst_table.append(separator)
        data = []
        for col_idx, col in enumerate(cols):
            length = max(len(header[col_idx]), len(max(col, key=len)))
            data.append(header[col_idx] + ' ' * (length - len(header[col_idx])))
        rst_table.append(' '.join(data))
        rst_table.append(separator)
        for row in rows:
            data = []
            for col_idx, col in enumerate(cols):
                length = max(len(header[col_idx]), len(max(col, key=len)))
                data.append(row[col_idx] + ' ' * (length - len(row[col_idx])))
            rst_table.append(' '.join(data))
        rst_table.append(separator)
        return rst_table

    raise Exception("Conversion from {} to {} not implemented".format(
        in_format, out_format
    ))


def get_processed_lines(lines):
    """
    replace all md tables in lines with rst-2 tables
    
    - additional processing can be added later if needed
    """
    tpos = find_markdown_tables(lines)
    for pos in tpos:
        md_table = lines[pos[0]:pos[1]+1]
        rst_table = convert_table(md_table, in_format="md", out_format="rst-2")
        # replace md_table with rst_table
        lines = lines[0:pos[0]] + \
            rst_table + \
            lines[pos[1]+1:]
    return lines


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("--input", help="Input Sphinx source directory")
    parser.add_argument("--output", help="Output Sphinx source directory")
    args = parser.parse_args()

    try:
        shutil.rmtree(args.output)
    except:
        pass

    shutil.copytree(args.input, args.output)

    for root, dirs, files in os.walk(args.output):
        for file in files:
            if file.endswith('.rst'):
                with open('{}/{}'.format(root, file)) as f:
                    data = f.read()
                lines = data.splitlines()
                lines = get_processed_lines(lines)
                with open('{}/{}'.format(root, file), 'w') as f:
                    f.write('\n'.join(lines))
    
    print("preprocessor step finished")
