cmake_minimum_required(VERSION 3.10)
project(NotePermutations C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(note_permutations main.c)
set_target_properties(note_permutations PROPERTIES OUTPUT_NAME "note_permutations")
