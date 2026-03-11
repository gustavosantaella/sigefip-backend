from nest.core import PyNestFactory, Module
from .app_controller import AppController
from .app_service import AppService
from src.modules.auth.auth_module import AuthModule


@Module(
    imports=[AuthModule],
    controllers=[AppController],
    providers=[AppService],
)
class AppModule:
    pass


app = PyNestFactory.create(
    AppModule,
    description="Nexo Finance Backend API",
    title="Nexo Finance API",
    version="1.0.0",
    debug=True,
)
http_server = app.get_server()
