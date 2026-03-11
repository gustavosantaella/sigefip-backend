from nest.core import Controller, Get


@Controller("/")
class AppController:

    @Get("/")
    def get_app_info(self):
        return {
            "app_name": "Nexo Finance API",
            "version": "1.0.0",
            "status": "running",
        }
