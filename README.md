# DNS Server

## TODO

    Error handling (zone file, input file, request body, etc)

    Return right error codes (other than NO_ERR) in response (NXDOMAIN, NODATA, etc)

    Use Union for the attribute "value" of the "record" object to be cleaner (specifically for SOA records)

    Add config tests

    Add dig tests

    Listen on TCP

    Make sure IPv6 works (server listen)

    Truncated req/response

    MTU ? (in subject page 5, try to figure out what that is)

    Multithreading

    More than one question in request/answer

    Handle some more record types

    Fasten everything (use clion time complexity tool)

    Handle wildcard records

    Message compression (RFC 1035 section 4.1.4.)

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
