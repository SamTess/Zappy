import os
import sys
import subprocess

EXCLUDED_RULES = [
    "-whitespace/indent",
    "-legal/copyright",
    "-whitespace/line_length",
    "-build/include_subdir",
    "-readability/namespace",
    "-whitespace/comments",
    "-build/header_guard",
    "-whitespace/parens",
    "-build/c++11",
    "-build/c++17"
]

def run_cpplint(directory):
    """Run cpplint recursively on the specified directory."""
    excluded_rules_arg = f"--filter={','.join(EXCLUDED_RULES)}"
    has_errors = False
    log_file = open('cpplint.log', 'w')

    for root, dirs, files in os.walk(directory):
        if "tests" in dirs:
            dirs.remove("tests")
        for file in files:
            if file.endswith(('.cpp', '.hpp')):
                filepath = os.path.join(root, file)
                try:
                    subprocess.run(
                        ['cpplint', excluded_rules_arg, filepath],
                        check=True,
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE
                    )
                except subprocess.CalledProcessError as e:
                    error_output = e.stderr.decode()
                    print(error_output, file=sys.stderr, end='')
                    log_file.write(error_output)
                    has_errors = True
    log_file.close()
    return has_errors

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 cpplint.py <directory>")
        sys.exit(1)

    target_directory = sys.argv[1]
    if not os.path.isdir(target_directory):
        print(f"Error: {target_directory} is not a valid directory.")
        sys.exit(1)

    has_errors = run_cpplint(target_directory)
    if has_errors:
        print(f"Lint errors found. See cpplint.log for details.")
        sys.exit(1)
    else:
        print("No lint errors found.")
