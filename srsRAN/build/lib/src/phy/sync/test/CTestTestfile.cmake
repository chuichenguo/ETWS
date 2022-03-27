# CMake generated Testfile for 
# Source directory: /home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/sync/test
# Build directory: /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/sync/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(sync_test_100 "sync_test" "-o" "100" "-p" "50" "-c" "501")
add_test(sync_test_400 "sync_test" "-o" "400" "-p" "50" "-c" "500")
add_test(sync_test_100_e "sync_test" "-o" "100" "-e" "-p" "50" "-c" "133")
add_test(sync_test_400_e "sync_test" "-o" "400" "-e" "-p" "50" "-c" "123")
add_test(npss_test_nonoise "npss_test")
add_test(nsss_test_nonoise_2 "nsss_test" "-c" "2")
add_test(nsss_test_nonoise_501 "nsss_test" "-c" "501")
add_test(sync_sl_test_tm2_p6_c_0 "sync_sl_test" "-p" "6" "-d" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm2_p6_c0_s1.92e6.dat")
add_test(sync_sl_test_tm2_p15_c_84 "sync_sl_test" "-p" "15" "-d" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm2_p15_c84_s3.84e6.dat")
add_test(sync_sl_test_tm2_p25_c_168 "sync_sl_test" "-p" "25" "-d" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm2_p25_c168_s7.68e6.dat")
add_test(sync_sl_test_tm2_p50_c_252 "sync_sl_test" "-p" "50" "-d" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm2_p50_c252_s15.36e6.dat")
add_test(sync_sl_test_tm2_p100_c_335 "sync_sl_test" "-p" "100" "-d" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm2_p100_c335_s30.72e6.dat")
add_test(sync_sl_test_tm2_p25_c_168_so "sync_sl_test" "-p" "25" "-d" "-o" "300" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm2_p25_c168_s7.68e6.dat")
add_test(sync_sl_test_tm2_self_test_p25_c_168 "sync_sl_test" "-p" "25" "-c" "168" "-d")
add_test(sync_sl_test_tm2_self_test_p25_c_168_fo "sync_sl_test" "-p" "25" "-c" "168" "-d" "-f" "100")
add_test(sync_sl_test_tm2_self_test_p25_c_168_fo_so "sync_sl_test" "-p" "25" "-c" "168" "-d" "-f" "100" "-o" "3600")
add_test(sync_sl_test_tm4_p6_c_0 "sync_sl_test" "-p" "6" "-t" "4" "-d" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm4_p6_c0_size6_num1_cshift0_s1.92e6.dat")
add_test(sync_sl_test_tm4_p15_c_84 "sync_sl_test" "-p" "15" "-t" "4" "-d" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm4_p15_c84_size5_num3_cshift0_s3.84e6.dat")
add_test(sync_sl_test_tm4_p25_c_168 "sync_sl_test" "-p" "25" "-t" "4" "-d" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm4_p25_c168_size5_num5_cshift0_s7.68e6.dat")
add_test(sync_sl_test_tm4_p50_c_252 "sync_sl_test" "-p" "50" "-t" "4" "-d" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm4_p50_c252_size10_num5_cshift0_s15.36e6.dat")
add_test(sync_sl_test_tm4_p25_c_168_so "sync_sl_test" "-p" "25" "-t" "4" "-d" "-o" "300" "-i" "/home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/phch/test/signal_sidelink_ideal_tm4_p25_c168_size5_num5_cshift0_s7.68e6.dat")
add_test(sync_sl_test_self_test_tm4_p25_c_168 "sync_sl_test" "-p" "25" "-t" "4" "-c" "168" "-d")
add_test(sync_sl_test_self_test_tm4_p25_c_168_fo "sync_sl_test" "-p" "25" "-t" "4" "-c" "168" "-d" "-f" "100")
add_test(sync_sl_test_self_test_tm4_p25_c_168_fo_so "sync_sl_test" "-p" "25" "-t" "4" "-c" "168" "-d" "-f" "100" "-o" "3600")
add_test(cfo_test_1 "cfo_test" "-f" "0.12345" "-n" "1000")
add_test(cfo_test_2 "cfo_test" "-f" "0.99849" "-n" "1000")
