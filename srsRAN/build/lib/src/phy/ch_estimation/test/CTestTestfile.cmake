# CMake generated Testfile for 
# Source directory: /home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/ch_estimation/test
# Build directory: /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/ch_estimation/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(chest_test_dl_cellid0 "chest_test_dl" "-c" "0")
add_test(chest_test_dl_cellid1 "chest_test_dl" "-c" "1")
add_test(chest_test_dl_cellid2 "chest_test_dl" "-c" "2")
add_test(chest_test_dl_cellid0_50prb "chest_test_dl" "-c" "0" "-r" "50")
add_test(chest_test_dl_cellid1_50prb "chest_test_dl" "-c" "1" "-r" "50")
add_test(chest_test_dl_cellid2_50prb "chest_test_dl" "-c" "2" "-r" "50")
add_test(chest_test_ul_cellid0 "chest_test_ul" "-c" "0" "-r" "50")
add_test(chest_test_ul_cellid1 "chest_test_ul" "-c" "2" "-r" "50")
add_test(chest_test_srs_6 "chest_test_srs" "-c" "2" "-r" "6")
set_tests_properties(chest_test_srs_6 PROPERTIES  LABELS "long;phy")
add_test(chest_test_srs_15 "chest_test_srs" "-c" "2" "-r" "15")
set_tests_properties(chest_test_srs_15 PROPERTIES  LABELS "long;phy")
add_test(chest_test_srs_25 "chest_test_srs" "-c" "2" "-r" "25")
set_tests_properties(chest_test_srs_25 PROPERTIES  LABELS "long;phy")
add_test(chest_test_srs_50 "chest_test_srs" "-c" "2" "-r" "50")
set_tests_properties(chest_test_srs_50 PROPERTIES  LABELS "long;phy")
add_test(chest_test_srs_75 "chest_test_srs" "-c" "2" "-r" "75")
set_tests_properties(chest_test_srs_75 PROPERTIES  LABELS "long;phy")
add_test(chest_test_srs_100 "chest_test_srs" "-c" "2" "-r" "100")
set_tests_properties(chest_test_srs_100 PROPERTIES  LABELS "long;phy")
add_test(chest_nbiot_test_dl "chest_nbiot_test_dl")
add_test(chest_test_sl_psbch "chest_test_sl")
add_test(dmrs_pdsch_test "dmrs_pdsch_test")
