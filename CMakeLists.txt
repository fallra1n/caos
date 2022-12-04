cmake_minimum_required(VERSION 3.22)
project(intitled)

set(CMAKE_C_STANDARD 17)

add_executable(contest2_aa contest2/utf8-count-symbols.c contest6/dot_product/testing.c)
add_executable(print-list-posix contest8/print-list-posix/print-list-posix.c)
add_executable(sort-file-contents contest8/sort-file-contents/sort-file-contents.c)
add_executable(find-invalid-executables contest9/find-invalid-executables/find-invalid-executables.c)
add_executable(manage-symlinks contest9/manage-symlinks/manage-symlinks.c)
add_executable(normalize_path contest9/normalize-path/normalize-path.c)
add_executable(find-substrings-in-file contest10/find-substrings-in-file/find-substrings-in-file.c)
add_executable(print-list-using-mmap contest10/print-list-using-mmap/print-list-using-mmap.c)
add_executable(proc-count-words contest11/proc-count-words/proc-count-words.c)