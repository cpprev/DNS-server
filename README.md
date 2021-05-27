# DNS Server

## TODO

    - [DOING] Error handling (request, etc)

    - [DOING] Return right error codes (other than NO_ERR) in response (NXDOMAIN, NODATA, etc)

    - [DOING] Truncated req/response
    
    - [DOING] Add config tests

    - [] Add dig tests

    - [] MTU ? (in subject page 5, try to figure out what that is)

    - [] More than one question in request/answer

    - [] Handle some more record types

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
    https://powerdns.org/hello-dns/auth.md.html
    https://stackoverflow.com/questions/41512591/dns-query-over-tcp
    https://serverfault.com/questions/698251/how-does-the-dns-protocol-switch-from-udp-to-tcp
