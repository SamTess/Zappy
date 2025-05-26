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

    for root, _, files in os.walk(directory):
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
                    print(e.stderr.decode(), file=sys.stderr, end='')
                    has_errors = True
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
        sys.exit(1)
