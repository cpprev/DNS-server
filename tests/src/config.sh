echo -e "\n$red""[Config Files Tests]"

# Valid

test_func_conf "valid" "valid zone" "input/valid/valid1.cnf"

# Invalid

test_func_conf "invalid" "invalid nothing" "input/invalid/invalid1.cnf"
test_func_conf "invalid" "invalid no port" "input/invalid/invalid2.cnf"
test_func_conf "invalid" "invalid no ip" "input/invalid/invalid3.cnf"
