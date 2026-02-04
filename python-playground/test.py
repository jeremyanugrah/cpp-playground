import sys
import requests

print(f"Python Executable: {sys.executable}")

try:
    r = requests.get('https://api.github.com')
    print(f"GitHub Status: {r.status_code}")
    print("Success! Request library is working.")
except Exception as e:
    print(e)
