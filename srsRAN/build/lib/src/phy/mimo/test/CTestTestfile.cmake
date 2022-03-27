# CMake generated Testfile for 
# Source directory: /home/kuo/srsLTE_20_10_1/srsRAN/lib/src/phy/mimo/test
# Build directory: /home/kuo/srsLTE_20_10_1/srsRAN/build/lib/src/phy/mimo/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(layermap_single "layermap_test" "-n" "1000" "-m" "p0" "-c" "1" "-l" "1")
add_test(layermap_diversity_2 "layermap_test" "-n" "1000" "-m" "div" "-c" "1" "-l" "2")
add_test(layermap_diversity_4 "layermap_test" "-n" "1000" "-m" "div" "-c" "1" "-l" "4")
add_test(layermap_multiplex_11 "layermap_test" "-n" "1000" "-m" "mux" "-c" "1" "-l" "1")
add_test(layermap_multiplex_12 "layermap_test" "-n" "1000" "-m" "mux" "-c" "1" "-l" "2")
add_test(layermap_multiplex_13 "layermap_test" "-n" "1002" "-m" "mux" "-c" "1" "-l" "3")
add_test(layermap_multiplex_14 "layermap_test" "-n" "1000" "-m" "mux" "-c" "1" "-l" "4")
add_test(layermap_multiplex_22 "layermap_test" "-n" "1000" "-m" "mux" "-c" "2" "-l" "2")
add_test(layermap_multiplex_23 "layermap_test" "-n" "1002" "-m" "mux" "-c" "2" "-l" "3")
add_test(layermap_multiplex_24 "layermap_test" "-n" "1000" "-m" "mux" "-c" "2" "-l" "4")
add_test(precoding_single "precoding_test" "-n" "1000" "-m" "p0")
add_test(precoding_diversity2 "precoding_test" "-n" "1000" "-m" "div" "-l" "2" "-p" "2")
add_test(precoding_diversity4 "precoding_test" "-n" "1024" "-m" "div" "-l" "4" "-p" "4")
add_test(precoding_cdd_2x2_zf "precoding_test" "-m" "cdd" "-l" "2" "-p" "2" "-r" "2" "-n" "14000" "-d" "zf")
add_test(precoding_cdd_2x2_mmse "precoding_test" "-m" "cdd" "-l" "2" "-p" "2" "-r" "2" "-n" "14000" "-d" "mmse")
add_test(precoding_multiplex_1l_cb0 "precoding_test" "-m" "mux" "-l" "1" "-p" "2" "-r" "2" "-n" "14000" "-c" "0")
add_test(precoding_multiplex_1l_cb1 "precoding_test" "-m" "mux" "-l" "1" "-p" "2" "-r" "2" "-n" "14000" "-c" "1")
add_test(precoding_multiplex_1l_cb2 "precoding_test" "-m" "mux" "-l" "1" "-p" "2" "-r" "2" "-n" "14000" "-c" "2")
add_test(precoding_multiplex_1l_cb3 "precoding_test" "-m" "mux" "-l" "1" "-p" "2" "-r" "2" "-n" "14000" "-c" "3")
add_test(precoding_multiplex_2l_cb0_zf "precoding_test" "-m" "mux" "-l" "2" "-p" "2" "-r" "2" "-n" "14000" "-c" "0" "-d" "zf")
add_test(precoding_multiplex_2l_cb1_zf "precoding_test" "-m" "mux" "-l" "2" "-p" "2" "-r" "2" "-n" "14000" "-c" "1" "-d" "zf")
add_test(precoding_multiplex_2l_cb2_zf "precoding_test" "-m" "mux" "-l" "2" "-p" "2" "-r" "2" "-n" "14000" "-c" "2" "-d" "zf")
add_test(precoding_multiplex_2l_cb0_mmse "precoding_test" "-m" "mux" "-l" "2" "-p" "2" "-r" "2" "-n" "14000" "-c" "0" "-d" "mmse")
add_test(precoding_multiplex_2l_cb1_mmse "precoding_test" "-m" "mux" "-l" "2" "-p" "2" "-r" "2" "-n" "14000" "-c" "1" "-d" "mmse")
add_test(precoding_multiplex_2l_cb2_mmse "precoding_test" "-m" "mux" "-l" "2" "-p" "2" "-r" "2" "-n" "14000" "-c" "2" "-d" "mmse")
add_test(pmi_select_test "pmi_select_test")
