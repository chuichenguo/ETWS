# CMake generated Testfile for 
# Source directory: /home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/resampling/test
# Build directory: /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/resampling/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(resample "resample_arb_test")
add_test(resampler_test_2 "resampler_test" "-s" "1920" "-r" "2" "-f" "2")
add_test(resampler_test_3 "resampler_test" "-s" "1920" "-r" "2" "-f" "3")
add_test(resampler_test_6 "resampler_test" "-s" "1920" "-r" "2" "-f" "6")
add_test(resampler_test_8 "resampler_test" "-s" "1920" "-r" "2" "-f" "8")
add_test(resampler_test_12 "resampler_test" "-s" "1920" "-r" "2" "-f" "12")
add_test(resampler_test_16 "resampler_test" "-s" "1920" "-r" "2" "-f" "16")
