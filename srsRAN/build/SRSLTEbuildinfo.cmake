cmake_minimum_required(VERSION 2.6)

execute_process(
COMMAND git rev-parse --abbrev-ref HEAD
WORKING_DIRECTORY "/home/kuo/srsLTE_20_10_1/srsRAN"
OUTPUT_VARIABLE GIT_BRANCH
OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
COMMAND git log -1 --format=%h
WORKING_DIRECTORY "/home/kuo/srsLTE_20_10_1/srsRAN"
OUTPUT_VARIABLE GIT_COMMIT_HASH
OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Generating build_info.h")
configure_file(
  /home/kuo/srsLTE_20_10_1/srsRAN/lib/include/srslte/build_info.h.in
  /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/include/srslte/build_info.h
)
