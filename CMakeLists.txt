cmake_minimum_required (VERSION 3.10)

# 1. TOOLCHAIN
set(CMAKE_TOOLCHAIN_FILE "C:/laragon/www/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project ("sigefip-back")

# 2. Estándar C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 3. Buscar dependencias
find_package(Drogon REQUIRED)
find_package(PostgreSQL REQUIRED) 



# 4. Definir ejecutable
add_executable (sigefip-back 
    "sigefip-back.cpp"
    "app/controllers/users/UserController.cpp"
    "app/controllers/auth/AuthController.cpp"
 "app/repositories/UserRepository.h" "app/entities/User.h" "app/services/UserService.h" "app/shared/helpers/Sha256Helper.h")

# 5. Directorios de inclusión
target_include_directories(sigefip-back PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 6. Vincular Librerías
# Nota: Si PostgreSQL::PostgreSQL falla, intenta usar ${PostgreSQL_LIBRARIES}
target_link_libraries(sigefip-back PRIVATE 
    Drogon::Drogon 
    PostgreSQL::PostgreSQL    
    OpenSSL::SSL
    OpenSSL::Crypto
)

# 7. Desactivar "Edit and Continue" para evitar el error de Hot Reload
if(MSVC)
    # Cambia el formato de información de depuración para habilitar Edit and Continue
    string(REPLACE "/Zi" "/ZI" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    target_compile_options(sigefip-back PRIVATE /ZI)
endif()