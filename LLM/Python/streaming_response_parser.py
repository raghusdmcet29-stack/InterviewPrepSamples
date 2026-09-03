import os
import json
import requests

api_key = os.environ.get("GROQ_API_KEY")
if not api_key:
    raise RuntimeError("Set GROQ_API_KEY environment variable before running")

url = "https://api.groq.com/openai/v1/chat/completions"

def stream_chat(prompt: str):
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    body = {
        "model": "openai/gpt-oss-120b",
        "messages": [{"role": "user", "content": prompt}],
        "stream": True
    }

    response = requests.post(url,headers=headers,json=body,stream=True)

    for line in response.iter_lines(decode_unicode=True):
        if not line or not line.startswith("data: "):
            continue

        json_string = line[len("data: "):]

        if json_string == "[DONE]":
            print("\n --- stream finsished ---")
            break

        try:
            data = json.loads(json_string)
            content = data["choices"][0]["delta"]["content"]
        except (json.JSONDecodeError, KeyError, IndexError):
            continue

        print(content,end="")
        
# Usage:
stream_chat("Count from 1 to 5.")


