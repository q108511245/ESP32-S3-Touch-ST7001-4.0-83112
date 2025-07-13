file(GLOB_RECURSE SOURCES ./*.c ./driver/*.c ./service/*.c ./utils/*.c)
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS 
                    ./
                    ./ui
                    ./driver/include
                    ./service/include
                    ./utils/include
                    REQUIRES 
                    EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem)
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-unused-variable)

