#include "app/controllers/auth/AuthMiddleware.h"
#include <drogon/drogon.h>


using namespace std;

int main() {

  cout << "Starint api..." << endl;

  drogon::app().addListener("0.0.0.0", 8080);
  cout << "Api is running on http://localhost:8080" << endl;

  drogon::app().createDbClient("postgresql", "localhost", 5432, "finance",
                               "postgres", "", 5);
  cout << "Postgres Database is running" << endl;

  cout << "Congratulation :D App is running." << endl;
  drogon::app().run();
  return 0;
}