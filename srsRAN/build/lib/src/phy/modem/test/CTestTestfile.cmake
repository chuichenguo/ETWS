# CMake generated Testfile for 
# Source directory: /home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/modem/test
# Build directory: /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/modem/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(modem_bpsk "modem_test" "-n" "1024" "-m" "1")
add_test(modem_qpsk "modem_test" "-n" "1024" "-m" "2")
add_test(modem_qam16 "modem_test" "-n" "1024" "-m" "4")
add_test(modem_qam64 "modem_test" "-n" "1008" "-m" "6")
add_test(modem_qam256 "modem_test" "-n" "1024" "-m" "8")
add_test(modem_bpsk_soft "modem_test" "-n" "1024" "-m" "1")
add_test(modem_qpsk_soft "modem_test" "-n" "1024" "-m" "2")
add_test(modem_qam16_soft "modem_test" "-n" "1024" "-m" "4")
add_test(modem_qam64_soft "modem_test" "-n" "1008" "-m" "6")
add_test(modem_qam256_soft "modem_test" "-n" "1024" "-m" "8")
