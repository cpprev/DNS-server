echo -e "\n$red""[Config Files Tests]"

################################################################################

# Valid

test_func_conf "valid" "valid zone" "input/valid/zone1.zone"

# Invalid

test_func_conf "invalid" "invalid zone" "input/invalid/zone1.zone"
