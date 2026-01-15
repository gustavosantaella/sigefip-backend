#define NOMINMAX
#include <drogon/drogon.h>

int main() {

    drogon::app().addListener("0.0.0.0", 8080);
    drogon::app().createDbClient("postgresql", "localhost", 5432, "finance", "postgres", "", 5);
    drogon::app().run();
    return 0;
}