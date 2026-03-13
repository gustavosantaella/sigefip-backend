from nest.core import PyNestFactory, Module
from .app_controller import AppController
from .app_service import AppService
from src.modules.auth.auth_module import AuthModule
from src.modules.admin.admin_module import AdminModule


@Module(
    imports=[AuthModule, AdminModule],
    controllers=[AppController],
    providers=[AppService],
)
class AppModule:
    pass


from fastapi.middleware.cors import CORSMiddleware

app = PyNestFactory.create(
    AppModule,
    description="Nexo Finance Backend API",
    title="Nexo Finance API",
    version="1.0.0",
    debug=True,
)
http_server = app.get_server()

http_server.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:4200", "http://localhost:8000"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)
