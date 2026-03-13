from nest.core import Injectable
from src.config import supabase

@Injectable
class AdminService:
    def __init__(self):
        self.client = supabase

    def get_stats(self):
        """Get system statistics: total users and total admins."""
        try:
            print("DEBUG Admin: Fetching users from auth.admin...")
            # Use Admin API to get actual users from Supabase Auth
            auth_response = self.client.auth.admin.list_users()
            
            # auth_response is a list of User objects in latest supabase-py
            total_users = len(auth_response)
            print(f"DEBUG Admin: Found {total_users} users in Auth")
            
            # Count admins from settings
            admins = self.client.from_("settings") \
                .select("user_id", count="exact") \
                .eq("key", "is_admin") \
                .eq("value", "true") \
                .execute()
            total_admins = admins.count if admins.count is not None else 0
            
            stats = {
                "total_users": total_users,
                "total_admins": total_admins
            }
            print(f"DEBUG Admin: Stats calculated -> {stats}")
            return stats
        except Exception as e:
            print(f"DEBUG Admin Error: {str(e)}")
            if hasattr(e, 'response'):
                print(f"DEBUG Admin Error Response: {e.response}")
            raise Exception(f"Error getting stats: {str(e)}")

    def get_users(self):
        """List all users from Supabase Auth with their admin status."""
        try:
            # 1. Get all users from Supabase Auth
            auth_users = self.client.auth.admin.list_users()
            
            # 2. Get admin settings
            admin_settings = self.client.from_("settings") \
                .select("user_id, value") \
                .eq("key", "is_admin") \
                .execute()
            
            admins_map = {row['user_id']: row['value'].lower() == 'true' for row in admin_settings.data}
            
            # 3. Combine data
            users_list = []
            for au in auth_users:
                user_id = au.id
                users_list.append({
                    "id": user_id,
                    "email": au.email,
                    "full_name": au.user_metadata.get("full_name", "Usuario"),
                    "is_admin": admins_map.get(user_id, False),
                    "is_verified": au.email_confirmed_at is not None,
                    "last_sign_in": str(au.last_sign_in_at) if au.last_sign_in_at else None,
                    "created_at": str(au.created_at) if au.created_at else None
                })
            
            return users_list
        except Exception as e:
            raise Exception(f"Error getting users: {str(e)}")

    def update_user(self, user_id: str, data: dict):
        """Update user status or details."""
        try:
            # If is_verified is being updated (manual verification)
            if 'is_verified' in data and data['is_verified'] is True:
                self.client.auth.admin.update_user_by_id(
                    user_id,
                    attributes={"email_confirm": True}
                )
                
            return {"message": "User updated successfully"}
        except Exception as e:
            raise Exception(f"Error updating user: {str(e)}")

    def delete_user(self, user_id: str):
        """Delete a user."""
        try:
            # Delete from settings first
            self.client.from_("settings").delete().eq("user_id", user_id).execute()
            
            # Delete from auth (using admin API)
            self.client.auth.admin.delete_user(user_id)
            
            return {"message": "User deleted successfully"}
        except Exception as e:
            raise Exception(f"Error deleting user: {str(e)}")
