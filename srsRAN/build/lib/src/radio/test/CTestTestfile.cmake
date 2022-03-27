# CMake generated Testfile for 
# Source directory: /home/kuo/srsLTE_20_10_1/srsRAN/lib/src/radio/test
# Build directory: /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/radio/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(benchmark_radio_multi_rf "benchmark_radio" "-d" "zmq" "-a" "tx_port=tcp://*:2000,rx_port=tcp://localhost:2000;tx_port=tcp://*:2001,rx_port=tcp://localhost:2001;tx_port=tcp://*:2002,rx_port=tcp://localhost:2002;tx_port=tcp://*:2003,rx_port=tcp://localhost:2003;" "-p" "4")
