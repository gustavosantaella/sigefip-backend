from nest.core import Injectable
from src.config import supabase


@Injectable
class AuthService:

    def __init__(self):
        self.client = supabase

    def delete_account(self, user_id: str):
        try:
            self.client.auth.admin.delete_user(user_id)
            return {"message": "Account deleted successfully"}
        except Exception as e:
            return {"error": str(e)}

    def verify_code(self, code: str):
        """Exchange the auth code from the Supabase redirect for a session."""
        try:
            response = self.client.auth.exchange_code_for_session({"auth_code": code})
            user = response.user
            session = response.session

            return {
                "message": "Email confirmed successfully",
                "access_token": session.access_token if session else None,
                "refresh_token": session.refresh_token if session else None,
                "user": {
                    "id": user.id,
                    "email": user.email,
                    "email_confirmed_at": str(user.email_confirmed_at) if user.email_confirmed_at else None,
                } if user else None,
            }
        except Exception as e:
            return {"error": str(e)}

    def reset_password(self, email: str):
        """Send a password reset email to the user."""
        try:
            self.client.auth.reset_password_email(email)
            return {"message": "Password reset email sent successfully"}
        except Exception as e:
            return {"error": str(e)}

    def update_password(self, code: str, new_password: str):
        """Exchange the reset code for a session, then update the password."""
        try:
            # First exchange the code from the reset link for a valid session
            session_response = self.client.auth.exchange_code_for_session({"auth_code": code})

            if not session_response.session:
                return {"error": "Invalid or expired code"}

            # Now update the password using the active session
            user_response = self.client.auth.update_user({"password": new_password})

            return {
                "message": "Password updated successfully",
                "user": {
                    "id": user_response.user.id,
                    "email": user_response.user.email,
                } if user_response.user else None,
            }
        except Exception as e:
            return {"error": str(e)}
