# CMake generated Testfile for 
# Source directory: /home/kuo/srsLTE_20_10_1/srsRAN/srsue/src
# Build directory: /home/kuo/srsLTE_20_10_1/srsRAN/build/srsue/src
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(srsue_version "srsue" "--version")
add_test(srsue_help "srsue" "--help")
add_test(ue_rf_failure "srsue" "/home/kuo/srsLTE_20_10_1/srsRAN/srsue/ue.conf.example" "--rf.device_name=zmq")
add_test(ue_rf_failure_max_channels "srsue" "/home/kuo/srsLTE_20_10_1/srsRAN/srsue/ue.conf.example" "--rf.device_name=zmq" "--rf.nof_antennas=4" "--rf.nof_carriers=5")
add_test(ue_rf_failure_exceeds_channels "srsue" "/home/kuo/srsLTE_20_10_1/srsRAN/srsue/ue.conf.example" "--rf.device_name=zmq" "--rf.nof_antennas=5" "--rf.nof_carriers=5")
subdirs("phy")
subdirs("stack")
