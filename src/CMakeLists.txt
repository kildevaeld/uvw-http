
set(SOURCES
    http-client.cpp
    http-request.cpp
    #http-request2.cpp
    http-url.cpp
    ${PROJECT_ROOT}/vendor/http-parser/http_parser.c
)


add_library(uvw-http ${SOURCES})
target_link_libraries(uvw-http uv)