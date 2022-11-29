cmake_minimum_required(VERSION 3.22)
project(intitled)

set(CMAKE_C_STANDARD 17)

add_executable(contest2_aa contest2/utf8-count-symbols.c)
add_executable(print-list-posix contest8/print-list-posix/print-list-posix.c)
add_executable(sort-file-contents contest8/sort-file-contents/sort-file-contents.c)
add_executable(find-invalid-executables contest9/find-invalid-executables/find-invalid-executables.c)
add_executable(manage-symlinks contest9/manage-symlinks/manage-symlinks.c)
