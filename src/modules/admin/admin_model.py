from pydantic import BaseModel
from typing import Optional

class UpdateUserModel(BaseModel):
    is_admin: Optional[bool] = None
    is_verified: Optional[bool] = None
    is_active: Optional[bool] = None
    full_name: Optional[str] = None
