# How to stress test the server :

### With dnsperf (to test with concurrent requests)

Github repository:

    https://github.com/DNS-OARC/dnsperf

Usage example:

    echo "example.com A" > in.txt

    dnsperf -c 1 -l 5 -T 5 -d in.txt        

# With custom stress tester (to test with wrongly formatted requests essentially)

    mkdir build; cd build; cmake ..; make -j; ./client

