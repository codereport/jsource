import os
import re

rootdir = r"jsrc"


def walk_path_cpp():
    for root, subdirs, files in os.walk(rootdir):
        for file in files:
            ext = os.path.splitext(file)[1]
            if ext != ".c" and ext != ".h":
                continue
            yield os.path.join(root, file)
    pass


def walk_matches():
    for path in walk_path_cpp():
        data = None
        with open(path, 'r') as f:
            data = f.read()
        if data is not None:
            regular_expression = re.compile(
                r'(#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\3\),\(\4\)\))')
            matches = regular_expression.findall(data)
            if len(matches) == 0:
                continue

            for full_str, name1, v1, v2, name2 in matches:
                print("removed "+full_str)
                data = data.replace(full_str, "")
                with open(path, 'w') as fw:
                    fw.write(data)
                yield re.compile(r'(^|[ \t]+|[^\d\w_])' + name1 + r'\((?=([^,]+?),([^)]+?)\))'), r'\1' + name2 + r'(jt,'
                return

    pass


def find_replaced_data():
    for regular_expression, replace_pattern in walk_matches():
        # print(regular_expression)
        # print(replace_pattern)
        for path in walk_path_cpp():
            # print(path)

            with open(path, 'r') as f:
                data = f.read()
                matches = regular_expression.search(data)
                if matches is None:
                    continue
                # print(matches)
                yield path, regular_expression.sub(replace_pattern, data)
    pass


def main():
    i = 0
    matches = [i for i in find_replaced_data()]
    for path, new_data in matches:
        with open(path, 'w') as f:
            f.write(new_data)
            i += 1
            print('\r' + str(i) + " replacements", '')
    print()
    pass


main()
