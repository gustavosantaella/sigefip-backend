from nest.core import Controller, Get, Post, Delete
from fastapi.responses import HTMLResponse
from fastapi import Request

from .auth_service import AuthService
from .auth_model import EmailModel, VerifyCodeModel, UpdatePasswordModel


@Controller("auth", tag="auth")
class AuthController:

    def __init__(self, auth_service: AuthService):
        self.auth_service = auth_service

    @Delete("/delete-account/{user_id}")
    def delete_account(self, user_id: str):
        return self.auth_service.delete_account(user_id)

    @Post("/confirm-email")
    def confirm_email(self, data: VerifyCodeModel):
        return self.auth_service.verify_code(data.code)

    @Get("/reset-password")
    def reset_password_page(self, code: str):
        html = f"""
        <!DOCTYPE html>
        <html lang="es">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Restablecer Contraseña - SIGEFIP</title>
            <style>
                * {{
                    margin: 0;
                    padding: 0;
                    box-sizing: border-box;
                }}
                body {{
                    font-family: 'Segoe UI', system-ui, -apple-system, sans-serif;
                    min-height: 100vh;
                    display: flex;
                    align-items: center;
                    justify-content: center;
                    background: linear-gradient(135deg, #0f172a 0%, #1e293b 50%, #0f172a 100%);
                    color: #e2e8f0;
                }}
                .container {{
                    background: rgba(30, 41, 59, 0.8);
                    backdrop-filter: blur(20px);
                    border: 1px solid rgba(99, 102, 241, 0.2);
                    border-radius: 16px;
                    padding: 40px;
                    width: 100%;
                    max-width: 420px;
                    box-shadow: 0 25px 50px rgba(0, 0, 0, 0.4);
                }}
                .logo {{
                    text-align: center;
                    margin-bottom: 32px;
                }}
                .logo h1 {{
                    font-size: 28px;
                    font-weight: 700;
                    background: linear-gradient(135deg, #818cf8, #6366f1);
                    -webkit-background-clip: text;
                    -webkit-text-fill-color: transparent;
                }}
                .logo p {{
                    color: #94a3b8;
                    margin-top: 8px;
                    font-size: 14px;
                }}
                .form-group {{
                    margin-bottom: 20px;
                }}
                label {{
                    display: block;
                    font-size: 13px;
                    font-weight: 500;
                    color: #94a3b8;
                    margin-bottom: 6px;
                    text-transform: uppercase;
                    letter-spacing: 0.5px;
                }}
                input {{
                    width: 100%;
                    padding: 12px 16px;
                    background: rgba(15, 23, 42, 0.6);
                    border: 1px solid rgba(99, 102, 241, 0.3);
                    border-radius: 10px;
                    color: #e2e8f0;
                    font-size: 15px;
                    transition: border-color 0.2s, box-shadow 0.2s;
                    outline: none;
                }}
                input:focus {{
                    border-color: #6366f1;
                    box-shadow: 0 0 0 3px rgba(99, 102, 241, 0.15);
                }}
                input::placeholder {{
                    color: #475569;
                }}
                button {{
                    width: 100%;
                    padding: 14px;
                    background: linear-gradient(135deg, #6366f1, #818cf8);
                    color: white;
                    border: none;
                    border-radius: 10px;
                    font-size: 15px;
                    font-weight: 600;
                    cursor: pointer;
                    transition: opacity 0.2s, transform 0.1s;
                    margin-top: 8px;
                }}
                button:hover {{
                    opacity: 0.9;
                }}
                button:active {{
                    transform: scale(0.98);
                }}
                button:disabled {{
                    opacity: 0.5;
                    cursor: not-allowed;
                }}
                .message {{
                    text-align: center;
                    padding: 12px;
                    border-radius: 8px;
                    margin-top: 16px;
                    font-size: 14px;
                    display: none;
                }}
                .message.success {{
                    background: rgba(34, 197, 94, 0.15);
                    color: #4ade80;
                    border: 1px solid rgba(34, 197, 94, 0.3);
                }}
                .message.error {{
                    background: rgba(239, 68, 68, 0.15);
                    color: #f87171;
                    border: 1px solid rgba(239, 68, 68, 0.3);
                }}
            </style>
        </head>
        <body>
            <div class="container">
                <div class="logo">
                    <h1>SIGEFIP</h1>
                    <p>Restablecer contraseña</p>
                </div>
                <form id="resetForm">
                    <div class="form-group">
                        <label>Nueva contraseña</label>
                        <input type="password" id="password" placeholder="Mínimo 6 caracteres" required minlength="6" />
                    </div>
                    <div class="form-group">
                        <label>Confirmar contraseña</label>
                        <input type="password" id="confirmPassword" placeholder="Repite tu contraseña" required minlength="6" />
                    </div>
                    <button type="submit" id="submitBtn">Restablecer contraseña</button>
                </form>
                <div class="message" id="message"></div>
            </div>

            <script>
                const form = document.getElementById('resetForm');
                const message = document.getElementById('message');
                const submitBtn = document.getElementById('submitBtn');
                const code = "{code}";

                form.addEventListener('submit', async (e) => {{
                    e.preventDefault();
                    const password = document.getElementById('password').value;
                    const confirmPassword = document.getElementById('confirmPassword').value;

                    if (password !== confirmPassword) {{
                        showMessage('Las contraseñas no coinciden', 'error');
                        return;
                    }}

                    submitBtn.disabled = true;
                    submitBtn.textContent = 'Procesando...';

                    try {{
                        const res = await fetch('/auth/update-password', {{
                            method: 'POST',
                            headers: {{ 'Content-Type': 'application/json' }},
                            body: JSON.stringify({{ code: code, new_password: password }})
                        }});
                        const data = await res.json();

                        if (data.error) {{
                            showMessage(data.error, 'error');
                        }} else {{
                            showMessage('Contraseña actualizada exitosamente. Ya puedes cerrar esta página.', 'success');
                            form.style.display = 'none';
                        }}
                    }} catch (err) {{
                        showMessage('Error de conexión. Intenta de nuevo.', 'error');
                    }} finally {{
                        submitBtn.disabled = false;
                        submitBtn.textContent = 'Restablecer contraseña';
                    }}
                }});

                function showMessage(text, type) {{
                    message.textContent = text;
                    message.className = 'message ' + type;
                    message.style.display = 'block';
                }}
            </script>
        </body>
        </html>
        """
        return HTMLResponse(content=html)

    @Post("/update-password")
    def update_password(self, data: UpdatePasswordModel):
        return self.auth_service.update_password(data.code, data.new_password)