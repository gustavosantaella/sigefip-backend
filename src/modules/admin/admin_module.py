from nest.core import Module
from .admin_controller import AdminController
from .admin_service import AdminService

@Module(
    controllers=[AdminController],
    providers=[AdminService],
)
class AdminModule:
    pass
