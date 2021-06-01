# DNS Server

## TODO

    - [DOING] Error handling (request, etc)

    - [DOING] Add config tests

    - [DOING] Create DNS client (add multithreading feature to spam server)

    - [DOING] additional and auth sections

    - [] Add "threads" parameter in config to add optional multithreading (on top of epoll)

    - [] Add dig tests

    - [] Handle wildcard records

    - [] Message compression (RFC 1035 section 4.1.4.)

## Brief

📍 This is an authoritative DNS server which can support multiple clients concurrently (using epoll syscall).

📍 It listens on UDP / TCP and is multithreaded.

📍 This server supports : A, AAAA, CNAME, NS, TXT and SOA records.

📍 It can also handle wrongly formatted DNS requests.

## Requirements

#### CMake

    sudo apt-get install cmake

#### gcc

    sudo apt-get install gcc


## Build

    mkdir build; cd build; cmake ..; make

## Usage

    ./dns { configuration_file } [-t] [-v]

## Configuration files

#### Example of configuration file

    [server]
    ip = 127.0.0.1
    port = 53

    [zone]
    name = abc
    path = /home/prev/Desktop/DNS_serv/input/zones/abc.zone

## Zone files

#### Example of zone file

    example.com;SOA;3600;ns0.example.com. postmaster.example.com. 2020092501 86400 3600 604800 3600
    example.com;NS;86400;ns0.example.com.
    example.com;NS;86400;ns1.example.com.
    example.com;A;3600;192.0.2.1
    example.com;TXT;10;Hello there\; how are you?
    ent.sub.example.com;TXT;86400;bzzpaoz aim benz
    www.example.com;AAAA;7200;2001:DB8::1
    www-dev.example.com;CNAME;7200;www.example.com.
    ns0.example.com;A;86400;192.0.2.3
    ns1.example.com;A;7200;192.0.2.4
    sub.zone.example.com;NS;86400;ns1.sub.zone.example.com.
    sub.zone.example.com;NS;86400;ns2.sub.zone.example.com.
    ns1.sub.zone.example.com;A;86400;192.0.2.42
    ns1.sub.zone.example.com;AAAA;86400;2001:DB8::42
    ns2.sub.zone.example.com;A;86400;192.0.2.84
    ns2.sub.zone.example.com;AAAA;86400;2001:DB8:0:0:0:0:0:84

## Links

    https://datatracker.ietf.org/doc/html/rfc1035
    https://powerdns.org/hello-dns/auth.md.html
    https://stackoverflow.com/questions/41512591/dns-query-over-tcp
    https://serverfault.com/questions/698251/how-does-the-dns-protocol-switch-from-udp-to-tcp
    https://www.suchprogramming.com/epoll-in-3-easy-steps/
