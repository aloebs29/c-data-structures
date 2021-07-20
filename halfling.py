from dataclasses import dataclass
import subprocess
import re

import halfling
import git

EXECUTABLE_NAME = "test_data_structures"
BUILD_DIR = "build"
SRC_DIR = "src"
TEST_DIR = "test"

NO_SETUP_FLAG = "HALFLING_NO_SETUP"

build_options = halfling.builders.CxxBuildOptions(
    executable_name=EXECUTABLE_NAME,
    compiler="clang",
    build_dir=BUILD_DIR,
    sources=[],
    flags=["-Wall"],
    libs=["cmocka"],
    include_paths=["src"],
    create_compile_flags_txt=True,
)

@dataclass
class TestInfo:
    name: str
    has_setup_teardown: bool

def find_tests(test_file_path):
    pattern = re.compile("void (test_.+?)\(.*?\)\s*(?:////\s*(\w+))?\n")
    matches = pattern.findall(open(test_file_path).read())
    tests = []
    for match in matches:
        tests.append(TestInfo(
            match[0],
            match[1] != NO_SETUP_FLAG,
        ))
    return tests


def make_header_contents(module_name, author, brief=None):
    if brief is None:
        brief = f"Header of the {module_name} module."

    return \
f"""/**
 * @file		{module_name}.h
 * @author		{author}
 * @brief		{brief}
 *
 */

#ifndef {module_name.upper()}_H_
#define {module_name.upper()}_H_



#endif // {module_name.upper()}_H_"""


def make_impl_contents(module_name, author, brief=None):
    if brief is None:
        brief = f"Implementation of the {module_name} module."

    return \
f"""/**
 * @file		{module_name}.c
 * @author		{author}
 * @brief		{brief}
 *
 */

#include "{module_name}.h"


"""


def make_test_contents(module_name, author):
    brief = f"Tests for the {module_name} module."
    return \
f"""/**
 * @file		test_{module_name}.c
 * @author		{author}
 * @brief		{brief}
 *
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "{module_name}.h"


"""


def make_test_runner(test_dir_path, test_files, tests):
    output = f"""#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
"""
    for file in test_files:
        output += f"#include \"{file}\"\n"

    output += """int main(void)
{
    const struct CMUnitTest tests[] = {
"""

    for test in tests:
        if test.has_setup_teardown:
            output += f"cmocka_unit_test_setup_teardown({test.name}, setup, teardown),\n"
        else:
            output += f"cmocka_unit_test({test.name}),\n"
    output += """
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}"""
    return output


# Tasks ---
def make_new_module_add_args(parser):
    parser.add_argument("name", help="name of the module")
    parser.add_argument("-b", "--brief", 
                        help="brief description to be placed at the top of the files")


def make_new_module(args):
    author = git.Repo(search_parent_directories=True).config_reader().get_value("user", "name")

    # make header/impl
    src_path = halfling.utils.get_project_base_dir() / SRC_DIR / args.name
    src_path.parent.mkdir(parents=True, exist_ok=True)
    with src_path.with_suffix(".h").open(mode="w") as f:
        f.write(make_header_contents(args.name, author, args.brief))
    with src_path.with_suffix(".c").open(mode="w") as f:
        f.write(make_impl_contents(args.name, author, args.brief))

    # make test
    test_path = halfling.utils.get_project_base_dir() / TEST_DIR / f"test_{args.name}"
    test_path.parent.mkdir(parents=True, exist_ok=True)
    with test_path.with_suffix(".c").open(mode="w") as f:
        f.write(make_test_contents(args.name, author))


def test_add_args(parser):
    parser.add_argument("module", help="module to test; use 'all' to test all")


def test(args):
    test_dir = halfling.utils.get_project_base_dir() / TEST_DIR
    test_files = []
    if args.module == "all":
        test_files.extend([path.name for path in test_dir.glob("test_*.c")])
    else:
        test_files.append(f"test_{args.module}.c")

    tests = []
    for test_file in test_files:
        tests.extend(find_tests(test_dir / test_file))

    test_runner = test_dir / "main.c"
    with (test_runner).open(mode="w") as f:
        f.write(make_test_runner(halfling.utils.get_project_base_dir() / TEST_DIR, test_files, 
                                 tests))

    # NOTE (aloebs): Derive sources from tests -- this is a little hacky, but we want to glob on 
    # the test directory when matching 'all', in case a module does not yet have a test.
    src_dir = halfling.utils.get_project_base_dir() / SRC_DIR
    sources = [(src_dir / (test_file.replace("test_", ""))) for test_file in test_files]
    sources = [source for source in sources if source.exists()]
    build_options.sources.extend(sources)
    build_options.sources.append(test_runner)

    try:
        halfling.builders.CxxBuilder(build_options).build()
        subprocess.run([halfling.utils.get_project_base_dir() / BUILD_DIR / EXECUTABLE_NAME])
    finally:
        pass
        # test_runner.unlink()


halfling.tasks.add_task("new", make_new_module, make_new_module_add_args)
halfling.tasks.add_task("test", test, test_add_args)
