# DNS Server

## TODO

    - [DOING] Error handling (request, etc)

    - [DOING] Return right error codes (other than NO_ERR) in response (NXDOMAIN, NODATA, etc)

    - [DOING] Add config tests

    - [DOING] Create DNS client (add multithreading feature to spam server)

    - [] Add "threads" parameter in config to add optional multithreading (on top of epoll)

    - [] Add dig tests

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
    https://www.suchprogramming.com/epoll-in-3-easy-steps/
