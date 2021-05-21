echo -e "\n$red""[Config Files Tests]"

# Valid

test_func_conf "valid" "valid zone" "input/conf/valid/valid1.cnf"

# Invalid

test_func_conf "invalid" "invalid nothing" "input/conf/invalid/invalid1.cnf"
test_func_conf "invalid" "invalid no port" "input/conf/invalid/invalid2.cnf"
test_func_conf "invalid" "invalid no ip" "input/conf/invalid/invalid3.cnf"
test_func_conf "invalid" "invalid zone no exist" "input/conf/invalid/invalid4.cnf"
test_func_conf "invalid" "invalid zone record no ttl" "input/conf/invalid/invalid5.cnf"
test_func_conf "invalid" "invalid zone record no domain" "input/conf/invalid/invalid6.cnf"
test_func_conf "invalid" "invalid zone record no value" "input/conf/invalid/invalid7.cnf"
test_func_conf "invalid" "invalid zone record no record type" "input/conf/invalid/invalid8.cnf"
test_func_conf "invalid" "invalid zone record no record type" "input/conf/invalid/invalid9.cnf"
