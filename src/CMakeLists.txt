
set(SOURCES
    http-client.cpp
    http-request.cpp
    ${PROJECT_ROOT}/vendor/http-parser/http_parser.c
)


add_library(uvw-http ${SOURCES})
target_link_libraries(uvw-http)