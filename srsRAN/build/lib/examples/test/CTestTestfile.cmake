# CMake generated Testfile for 
# Source directory: /home/kuo/srsLTE_20_10_1/srsRAN/lib/examples/test
# Build directory: /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/examples/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(enb1 "/usr/bin/cmake" "-DCMD=/home/kuo/srsLTE_20_10_1/srsRAN/build/lib/examples/npdsch_enodeb" "-DARG=-o;/tmp/enbtest1.bin;-n;512;-t;2" "-V" "-P" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/examples/test/iqtests.cmake")
add_test(npdsch_npdcch_file1 "/usr/bin/cmake" "-DCMD=/home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/phch/test/npdsch_npdcch_file_test" "-DARG=-i;/tmp/enbtest1.bin;-m;512;-t;2;-w;0;-k" "-V" "-P" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/examples/test/iqtests.cmake")
set_tests_properties(npdsch_npdcch_file1 PROPERTIES  DEPENDS "enb1" PASS_REGULAR_EXPRESSION "pkt_ok=1")
add_test(npdsch_npdcch_file2 "/usr/bin/cmake" "-DCMD=/home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/phch/test/npdsch_npdcch_file_test" "-DARG=-i;/tmp/enbtest1.bin;-m;512;-t;2;-w;0;-r;0x1234" "-V" "-P" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/examples/test/iqtests.cmake")
set_tests_properties(npdsch_npdcch_file2 PROPERTIES  DEPENDS "enb1" PASS_REGULAR_EXPRESSION "pkt_ok=512")
add_test(enb2 "/usr/bin/cmake" "-DCMD=/home/kuo/srsLTE_20_10_1/srsRAN/build/lib/examples/npdsch_enodeb" "-DARG=-o;/tmp/enbtest2.bin;-n;10;-t;0;-i;1;-m;4;-r;2" "-V" "-P" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/examples/test/iqtests.cmake")
add_test(npdsch_npdcch_file3 "/usr/bin/cmake" "-DCMD=/home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/phch/test/npdsch_npdcch_file_test" "-DARG=-i;/tmp/enbtest2.bin;-m;512;-w;0;-r;0x1234" "-V" "-P" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/examples/test/iqtests.cmake")
set_tests_properties(npdsch_npdcch_file3 PROPERTIES  DEPENDS "enb2" PASS_REGULAR_EXPRESSION "pkt_ok=5")
