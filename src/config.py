import os
from dotenv import load_dotenv
from supabase import create_client, Client

load_dotenv()

SUPABASE_URL = os.getenv("PUBLIC_SUPABASE_URL")
SUPABASE_KEY = os.getenv("SUPABASE_SERVICE_ROLE_KEY")

print(f"SUPABASE CONFIG: URL={SUPABASE_URL}")
if SUPABASE_KEY:
    print(f"SUPABASE CONFIG: KEY loaded (prefix: {SUPABASE_KEY[:10]}...)")
else:
    print("SUPABASE CONFIG: SERVICE_ROLE_KEY IS MISSING!")

supabase: Client = create_client(SUPABASE_URL, SUPABASE_KEY)
