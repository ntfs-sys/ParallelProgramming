set(CMAKE_CUDA_STANDARD 23)
set(CMAKE_CUDA_ARCHITECTURES native)

find_package(CUDAToolkit REQUIRED)

add_executable(lab4 main.cu)
target_link_libraries(lab4 PRIVATE CUDA::cudart)
