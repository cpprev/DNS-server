# DNS Server

### Requirements

CMake

    sudo apt-get install cmake

gcc

    sudo apt-get install gcc


### Build

    mkdir build; cd build; cmake ..; make

### TODEL Notes

Cf https://www.ques10.com/p/10908/explain-dns-message-format-with-neat-diagram-1/

    DNS query : header + question section

    DNS response : header + question section + answer section + authoritative section + additional section
