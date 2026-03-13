from pydantic import BaseModel


class EmailModel(BaseModel):
    email: str


class VerifyCodeModel(BaseModel):
    code: str


class UpdatePasswordModel(BaseModel):
    code: str
    new_password: str


class LoginModel(BaseModel):
    email: str
    password: str


class DeleteAccountRequestModel(BaseModel):
    email: str
    password: str
    reason: str = ""
