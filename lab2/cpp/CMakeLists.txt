add_executable(lab2)
target_compile_features(lab2 PUBLIC cxx_std_23)

find_package(OpenMP REQUIRED)

target_sources(lab2
        PRIVATE
                main.cpp
        PUBLIC
        FILE_SET "modules" TYPE CXX_MODULES FILES
                Multiplyer.cppm
                IOMatrix.cppm
                RMatrix.cppm
)

target_link_libraries(lab2 PRIVATE OpenMP::OpenMP_CXX)
