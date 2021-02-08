import os
import re
import time

SKIPTESTS = True
rootdir = r"jsrc"
LOG_FILE_NAME = "removal_log.md"
old_name = ""  # only doing one name at a time. Maybe not work.


def write_log_header():
    with open(LOG_FILE_NAME, 'w') as logw:
        logw.write("|Path|Macro Name|Function|Regex|Replace|\n|---|---|---|---|---|")


def check_log_empty():
    if os.path.exists(LOG_FILE_NAME):
        with open(LOG_FILE_NAME, 'r') as log:
            if log.read().strip().rstrip('\n') == "":
                return True
    return False
    pass


def write_log_entry(paths, local_old_name, new_name, regular_expression, replace_pattern):
    if check_log_empty():
        write_log_header()
    unescaped = re.compile(r'\|')
    escaped = '\\|'
    with open(LOG_FILE_NAME, 'a') as log:
        log.write('\n|')
        log.write(' '.join([path.strip() for path in paths]))
        log.write('|' + local_old_name)
        log.write('|' + new_name)
        log.write('|`' + unescaped.sub(escaped, regular_expression.pattern) + '`')
        log.write('|`' + unescaped.sub(escaped, replace_pattern) + '`|')


def combine_log_entries():
    log_entry_regexp = re.compile(r'^\|([^|]+)\|(.+)$', re.M)

    result = {}
    with open(LOG_FILE_NAME, 'r') as log:
        findall = log_entry_regexp.findall(log.read())
        # print(findall)
        for path, key in findall:
            result.setdefault(key.strip().rstrip('\n'), []).append(path.strip())
        # print()
        # print("{" + "\n".join("{!r}: {!r},".format(k, v) for k, v in result.items()) + "}")
        # print()

    time.sleep(.25)  # race condition between close log and open log.
    with open(LOG_FILE_NAME, 'w') as log:
        log.write('\n'.join(['|' + ' '.join([path.strip() for path in paths]) + '|' +
                             key for key, paths in result.items()]))
    pass


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
            if line.strip() == old_name:
                return True
    return False
    pass


def walk_matches():
    global old_name
    for path in walk_path_cpp():
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
                    old_name = ""
                    continue
                print("remove " + full_str)
                data = data.replace(full_str, "")
                write_log_entry([path], full_str, "", regular_expression, "")
                time.sleep(.25)  # race condition?
                with open(path, 'w') as fw:
                    fw.write(data)
                yield re.compile(r'(^|[ \t]+|[^\d\w_])' + old_name + r'\((?=([^,]+?),([^)]+?)\))'), \
                      r'\1' + new_name + r'(jt,', old_name, new_name
                return
    old_name = ""
    pass


def find_replaced_data():
    global old_name
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
                write_log_entry([path], old_name, new_name, regular_expression, replace_pattern)
                yield path, regular_expression.sub(replace_pattern, data), old_name
    pass


def command_test(command_test_pair):
    (command, test, message) = command_test_pair
    print(message.strip() + " RUNNING...")
    current_output = os.popen(command).read()
    with open(test, 'r') as f:
        if f.read().strip() not in current_output.strip():
            print(current_output)
            return True
    print(message.strip() + " SUCCESSFUL!")
    return False
    pass


def command_tests(command_test_pairs):
    for command_test_pair in command_test_pairs:
        if command_test(command_test_pair):
            return True
    return False


def main():
    while True:
        global old_name
        old_name = ""
        matches = [i for i in find_replaced_data()]
        if old_name == "":
            return
        for path, new_data, old_name in matches:
            with open(path, 'w') as f:
                f.write(new_data)
        command_test_pairs = [('cmake -G "Ninja Multi-Config" -B build', "known_good_cmake.txt", "CMAKE "),
                              ('ninja -C build', "known_good_build.txt", "BUILD "),
                              ('ninja -C build test', "known_good_test.txt", "TEST ")]

        if (not SKIPTESTS) and command_tests(command_test_pairs):
            os.system('git reset --hard')
            with open("excluded_names.txt", 'a') as excluded_names:
                excluded_names.write("\n" + old_name)

            os.system('git commit -m "exclude ' + old_name + '" -a')
        else:
            combine_log_entries()
            os.system('git commit -m "remove #define ' + old_name + '" -a')
            # os.system('git push')
    pass


main()
