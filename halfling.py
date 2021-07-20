import dataclasses
import subprocess

import halfling
import git

BUILD_DIR = "build"
SRC_DIR = "src"
TEST_DIR = "test"

build_options = halfling.builders.CxxBuildOptions(
    executable_name="",
    compiler="clang",
    build_dir=BUILD_DIR,
    sources=[],
    flags=["-Wall"],
    libs=["cmocka"],
    include_paths=["src"],
    create_compile_flags_txt=True,
)


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



int main(void)
{{
    const struct CMUnitTest tests[] = {{
    }};

    return cmocka_run_group_tests(tests, NULL, NULL);
}}
"""


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
    modules = []
    if args.module == "all":
        modules.extend([str(path.stem).replace("test_", "") for path in test_dir.glob("test_*.c")])
    else:
        modules.append(args.module)

    src_dir = halfling.utils.get_project_base_dir() / SRC_DIR
    executables = []
    for module in modules:
        executable_name = f"test_{module}"
        sources = [src_dir / f"{module}.c", test_dir / f"test_{module}.c"]
        sources = [source for source in sources if source.exists()] # support header-only modules
        build_test_options = dataclasses.replace(
            build_options,
            executable_name=executable_name,
            sources=sources
        )
        halfling.builders.CxxBuilder(build_test_options).build()

        executables.append(executable_name)

    summary = "\nSummary:\n"
    for exe in executables:
        proc = subprocess.run(halfling.utils.get_project_base_dir() / BUILD_DIR / exe)
        status = "FAILED" if proc.returncode else "PASSED"
        summary += f"{exe:20}| {status}\n"
    print(summary)


halfling.tasks.add_task("new", make_new_module, make_new_module_add_args)
halfling.tasks.add_task("test", test, test_add_args)
