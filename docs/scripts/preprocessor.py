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
import itertools


def find_markdown_tables(lines):
    """
    return (row_start, row_end) for all markdown tables.
    """
    indices = [idx for idx, line in enumerate(lines) if line.startswith("|")]
    nindices = len(indices)
    indices_type = ["end" if i + 1 == nindices or indices[i+1] - indices[i] > 1
                    else "start" if i == 0 or indices[i] - indices[i-1] > 1
                    else "mid" for i in range(nindices)]
    return list(zip([indices[i] for i in range(nindices) if indices_type[i] == "start"],
                    [indices[i] for i in range(nindices) if indices_type[i] == "end"]))


def get_markdown_row_data(row):
    """
    get row (or header) data from markdown row (or header)

    row
        | apples | red | a |

    data
        ['apples', 'red', 'a']
    """
    return [e.strip() for e in row.split("|")[1:-1] if e.strip()]


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
    if in_format == "md" and out_format == "rst-2":
        header = get_markdown_row_data(table[0])
        rows = [get_markdown_row_data(row) for row in table[2:]]
        cols = [[row[col_idx] for row in rows]
                for col_idx in range(len(header))]
        tbl_separator = ' '.join(
            ['=' * max(len(header[col_idx]), len(max(col, key=len)))
             for col_idx, col in enumerate(cols)],
        )
        tbl_header = ' '.join([
            header[idx] + ' ' * (max(len(header[idx]), len(max(col, key=len))) - len(header[idx]))
            for idx, col in enumerate(cols)
        ])
        tbl_rows = [
            ' '.join([
                row[idx] + ' ' * (max(len(header[idx]), len(max(col, key=len))) - len(row[idx]))
                for idx, col in enumerate(cols)
            ]) for row in rows
        ]
        return [tbl_separator] + [tbl_header] + [tbl_separator] + tbl_rows + [tbl_separator]

    raise Exception("Conversion from {} to {} not implemented".format(
        in_format, out_format
    ))


def get_processed_lines(lines):
    """
    replace all md tables in lines with rst-2 tables

    - additional processing can be added later if needed
    """
    tpos = find_markdown_tables(lines)
    
    if not tpos:
        return lines
    
    chunks = [[(0, tpos[0][0] - 1)]] + \
        [[pos] if idx == 0
         else [(tpos[idx-1][1] + 1, pos[0] - 1), pos] for idx, pos in enumerate(tpos)] + \
        [[(tpos[-1][1] + 1, len(lines) - 1)]]
    
    return list(itertools.chain.from_iterable([
        convert_table(lines[chunk[0]:chunk[1]+1], "md", "rst-2") if chunk in tpos \
        else lines[chunk[0]:chunk[1]+1] \
        for chunk in list(itertools.chain(*chunks))
    ]))


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
                lines = get_processed_lines(data.splitlines())
                with open('{}/{}'.format(root, file), 'w') as f:
                    f.write('\n'.join(lines))

    print("preprocessor step finished")