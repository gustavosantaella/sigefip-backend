from pydantic import BaseModel


class EmailModel(BaseModel):
    email: str


class VerifyCodeModel(BaseModel):
    code: str


class UpdatePasswordModel(BaseModel):
    code: str
    new_password: str
