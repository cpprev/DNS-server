# DNS Server

## TODO

    - [DOING] Error handling (zone file, input file, request body, etc)

    - [DOING] Make verbose print for requests/input file/zone and remove trash prints.

    - [DOING] Return right error codes (other than NO_ERR) in response (NXDOMAIN, NODATA, etc)

    - [DOING] Multithreading

    - [DOING] Add config tests

    - [DOING] Listen on TCP

    - [] Use Union for the attribute "value" of the "record" object to be cleaner (specifically for SOA records)

    - [] Add dig tests

    - [] Make sure IPv6 works (server listen)

    - [] Truncated req/response

    - [] MTU ? (in subject page 5, try to figure out what that is)

    - [] More than one question in request/answer

    - [] Handle some more record types

    - [] Fasten everything (use clion time complexity tool)

    - [] Handle wildcard records

    - [] Message compression (RFC 1035 section 4.1.4.)

### Requirements

CMake

    sudo apt-get install cmake

gcc

    sudo apt-get install gcc


### Build

    mkdir build; cd build; cmake ..; make

### Links

    https://datatracker.ietf.org/doc/html/rfc1035
    https://www.ques10.com/p/10908/explain-dns-message-format-with-neat-diagram-1/
