# 1. REQUERIMIENTO MÍNIMO (Debe ser la primera línea sin espacios arriba)
cmake_minimum_required(VERSION 3.10)

# 2. TOOLCHAIN de vcpkg (Configurado antes del proyecto)
set(CMAKE_TOOLCHAIN_FILE "C:/laragon/www/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project("sigefip-back")

# 3. ESTÁNDAR C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 4. DEFINICIONES GLOBALES (Soluciona errores de picojson, openssl y getenv)
add_definitions(-DNOMINMAX)                # Fix para errores de picojson (C2589 y C3878)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)  # Fix para getenv
add_definitions(-DOPENSSL_API_COMPAT=30000) # Fix para SHA256_Init y otros

# 5. BUSCAR DEPENDENCIAS
find_package(Drogon REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)
# Nota: picojson no requiere find_package si es header-only desde vcpkg, 
# pero si quieres usarlo como target:
find_package(picojson CONFIG QUIET) 

# 6. DEFINIR EJECUTABLE
add_executable(sigefip-back 
    "sigefip-back.cpp"
    "app/controllers/users/UserController.cpp"
    "app/controllers/auth/AuthController.cpp"
    "app/repositories/UserRepository.h" 
    "app/entities/User.h" 
    "app/services/UserService.h" 
    "app/shared/helpers/Sha256Helper.h" 
    "app/shared/helpers/EnvHelper.h"
  "app/controllers/auth/AuthMiddleware.h")

# 7. DIRECTORIOS DE INCLUSIÓN
target_include_directories(sigefip-back PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 8. VINCULAR LIBRERÍAS
target_link_libraries(sigefip-back PRIVATE 
    Drogon::Drogon 
    PostgreSQL::PostgreSQL    
    OpenSSL::SSL
    OpenSSL::Crypto
)

# 9. CONFIGURACIÓN ESPECÍFICA PARA MSVC (Visual Studio)
if(MSVC)
    # Habilitar Edit and Continue y corregir warnings
    target_compile_options(sigefip-back PRIVATE /ZI /W3)
    
    # Asegurar que el formato de depuración sea compatible
    string(REPLACE "/Zi" "/ZI" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()