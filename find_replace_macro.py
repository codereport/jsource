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
        with open(path, 'r') as f:
            matches = re.findall(r'#define\s+([\w\d]+)\(([^,]+),([^),]+)\)\s+(jt[\d\w]+)\(jt,\(\2\),\(\3\)\)', f.read())
            if len(matches) == 0:
                continue
            for name1, v1, v2, name2 in matches:
                yield r'(^|[ \t]+|[^\d\w_])' + name1 + r'\((?=([^,]+?),([^)]+?)\))', '\1' + name2 + r'(jt,'
    pass


def find_replaced_data():
    for regular_expression, replace_pattern in walk_matches():
        # print(regular_expression)
        # print(replace_pattern)
        for path in walk_path_cpp():
            # print(path)

            with open(path, 'r') as f:
                data = f.read()
                matches = re.search(regular_expression, data)
                if matches is None:
                    continue
                # print(matches)
                yield path, re.sub(regular_expression, replace_pattern, data)
    pass


def main():
    i = 0
    for path, new_data in find_replaced_data():
        with open(path, 'w') as f:
            f.write(new_data)
            i += 1
    print(str(i) + " replacements")
    pass


main()
