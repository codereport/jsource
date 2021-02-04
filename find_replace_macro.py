import os
import re

rootdir = r"jsrc"
old_name = ""  # only doing one name at a time. Maybe not work.


def walk_path_cpp():
    for root, subdirs, files in os.walk(rootdir):
        for file in files:
            ext = os.path.splitext(file)[1]
            if ext != ".c" and ext != ".h":
                continue
            yield os.path.join(root, file)
    pass


def check_excluded_names():
    with open("excluded_names.txt", 'r') as excluded_names:
        for line in excluded_names:
            if line == old_name:
                return True
    return False
    pass


def walk_matches():
    global old_name
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

            for full_str, old_name, v1, v2, new_name in matches:
                if check_excluded_names():
                    continue;
                print("remove " + full_str)
                data = data.replace(full_str, "")
                with open(path, 'w') as fw:
                    fw.write(data)
                yield re.compile(r'(^|[ \t]+|[^\d\w_])' + old_name + r'\((?=([^,]+?),([^)]+?)\))'), \
                    r'\1' + new_name + r'(jt,', old_name, new_name
                return

    pass


def find_replaced_data():
    global old_name
    unescaped = re.compile(r'\|')
    for regular_expression, replace_pattern, old_name, new_name in walk_matches():
        # print(regular_expression)
        # print(replace_pattern)
        for path in walk_path_cpp():
            # print(path)

            with open(path, 'r') as f:
                data = f.read()
                matches = regular_expression.search(data)
                if matches is None:
                    continue

                with open("removal_log.md", 'a') as log:
                    escaped = '\\|'
                    log.write('\n|`' + path + '`')
                    log.write('|' + old_name)
                    log.write('|' + new_name)
                    log.write('|`' + unescaped.sub(escaped, regular_expression.pattern) + '`')
                    log.write('|`' + unescaped.sub(escaped, replace_pattern) + '`|')
                # print(matches)
                yield path, regular_expression.sub(replace_pattern, data), old_name
    pass


def main():
    global old_name
    i = 0
    matches = [i for i in find_replaced_data()]

    for path, new_data, old_name in matches:
        with open(path, 'w') as f:
            f.write(new_data)
            i += 1
    print()
    os.system('git commit -m "remove #define ' + old_name + '" -a')
    os.system('git push')
    pass


main()
