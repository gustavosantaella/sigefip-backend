#include <drogon/drogon.h>
#include "app/controllers/auth/AuthMiddleware.h"

int main() {


    drogon::app().addListener("0.0.0.0", 8080);
    drogon::app().createDbClient("postgresql", "100.86.153.124", 5432, "finance", "postgres", "", 5);
    drogon::app().run();
    return 0;
}