from nest.core import Controller, Get, Patch, Delete
from src.core.base_controller import BaseController
from .admin_service import AdminService
from .admin_model import UpdateUserModel

@Controller("admin", tag="admin")
class AdminController(BaseController):
    def __init__(self, admin_service: AdminService):
        self.admin_service = admin_service

    @Get("/stats")
    def get_stats(self):
        try:
            stats = self.admin_service.get_stats()
            return self.success("Stats retrieved successfully", stats)
        except Exception as e:
            return self.error("Failed to retrieve stats", str(e))

    @Get("/users")
    def get_users(self):
        try:
            users = self.admin_service.get_users()
            return self.success("Users retrieved successfully", users)
        except Exception as e:
            return self.error("Failed to retrieve users", str(e))

    @Patch("/users/{user_id}")
    def update_user(self, user_id: str, data: UpdateUserModel):
        try:
            result = self.admin_service.update_user(user_id, data.dict(exclude_unset=True))
            return self.success("User updated successfully", result)
        except Exception as e:
            return self.error("Failed to update user", str(e))

    @Delete("/users/{user_id}")
    def delete_user(self, user_id: str):
        try:
            result = self.admin_service.delete_user(user_id)
            return self.success("User deleted successfully", result)
        except Exception as e:
            return self.error("Failed to delete user", str(e))
