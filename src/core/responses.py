from pydantic import BaseModel
from typing import Any, Optional, Union

class ApiResponse(BaseModel):
    success: bool
    message: str
    data: Optional[Any] = None
    error: Optional[str] = None

def success_response(message: str, data: Any = None) -> dict:
    return ApiResponse(success=True, message=message, data=data).dict()

def error_response(message: str, error: str = None) -> dict:
    return ApiResponse(success=False, message=message, error=error).dict()
