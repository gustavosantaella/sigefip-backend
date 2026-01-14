#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/sha.h>

class Sha256Helper {
public:
    // Función para "Encriptar" (Generar el Hash)
    static std::string encrypt(const std::string& password) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, password.c_str(), password.size());
        SHA256_Final(hash, &sha256);

        std::stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            // Convierte cada byte a formato hexadecimal (ej: ff, 0a, 2b)
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        return ss.str();
    }

    // Función para "Desencriptar" (En realidad es verificar/comparar)
    static bool verify(const std::string& inputPassword, const std::string& storedHash) {
        // Hasheamos la contraseña que el usuario acaba de escribir
        std::string inputHash = encrypt(inputPassword);

        // Si los hashes son iguales, la contraseña es correcta
        return inputHash == storedHash;
    }
};