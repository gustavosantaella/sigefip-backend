cmake_minimum_required(VERSION 3.10)

# El proyecto debe ir ANTES de buscar paquetes
project("sigefip-back")

# 1. ESTÁNDAR C++20 (Crucial para co_await en Linux)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 2. DEFINICIONES GLOBALES
add_definitions(-DNOMINMAX)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if(WIN32)
    add_definitions(-DOPENSSL_API_COMPAT=30000)
endif()


# 3. BUSCAR DEPENDENCIAS
find_package(Drogon REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(jwt-cpp REQUIRED) # Añadido para AuthController
find_path(PICOJSON_INCLUDE_DIRS "picojson.h") # Buscamos el header
# 4. DEFINIR EJECUTABLE
# Asegúrate de que los nombres de carpetas coincidan exactamente (ej. 'app' vs 'App')
add_executable(sigefip-back 
    "sigefip-back.cpp"
    "app/controllers/users/UserController.cpp"
    "app/controllers/auth/AuthController.cpp"
    "app/repositories/UserRepository.h" 
    "app/entities/User.h" 
    "app/services/UserService.h" 
    "app/shared/helpers/Sha256Helper.h" 
    "app/shared/helpers/EnvHelper.h"
    "app/controllers/auth/AuthMiddleware.h"
)

# 5. DIRECTORIOS DE INCLUSIÓN
target_include_directories(sigefip-back PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# 6. VINCULAR LIBRERÍAS
target_link_libraries(sigefip-back PRIVATE 
    Drogon::Drogon 
    PostgreSQL::PostgreSQL    
    OpenSSL::SSL
    OpenSSL::Crypto
    jwt-cpp::jwt-cpp
)

# 7. CONFIGURACIÓN ESPECÍFICA PARA MSVC
if(MSVC)
    target_compile_options(sigefip-back PRIVATE /ZI /W3)
    string(REPLACE "/Zi" "/ZI" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()