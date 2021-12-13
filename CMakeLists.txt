cmake_minimum_required(VERSION 3.2)


project(dns_proxy)
add_subdirectory(libft-master)

include_directories(libft-master/includes DNS)
set(SOURCE_FILES
    ./project/dns.c
    ./project/blacklist.c
    ./project/main.c
    ./project/query.c
    ./project/server.c
)


add_executable(dns_proxy ${SOURCE_FILES})

set_target_properties(dns_proxy PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY build
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    COMPILE_OPTIONS  -w -q -fsyntax-only
        )

target_link_libraries (dns_proxy libft)