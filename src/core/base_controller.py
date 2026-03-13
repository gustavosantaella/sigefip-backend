from typing import Any
from .responses import success_response, error_response

class BaseController:
    """Base controller providing standard response methods."""
    
    def success(self, message: str, data: Any = None):
        return success_response(message, data)
        
    def error(self, message: str, error: str = None):
        return error_response(message, error)
