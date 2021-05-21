echo -e "\n$red""[Config Files Tests]"

# Valid

test_func_conf "valid" "valid zone" "input/valid/valid1.cnf"

# Invalid

test_func_conf "invalid" "invalid zone" "input/invalid/invalid1.cnf"
