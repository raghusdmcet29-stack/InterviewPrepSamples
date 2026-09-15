import os
import requests

def apply_sliding_window(messages,max_window):
    if len(messages)<= max_window:
        return messages
    return messages[-max_window:]


def call_groq(messages):
    api_key = os.environ["GROQ_API_KEY"]
    url = "https://api.groq.com/openai/v1/chat/completions"
    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {api_key}"
    }
    payload = {
        "model": "openai/gpt-oss-120b",
        "temperature": 0.2,
        "messages": messages
    }
    response = requests.post(url, headers=headers, json=payload)
    data = response.json()
    return data["choices"][0]["message"]["content"]

messages = [
    {"role": "user", "content": "Hi, my name is Anuraag"},
    {"role": "assistant", "content": "Hello Anuraag!"},
    {"role": "user", "content": "What's 2+2?"},
    {"role": "assistant", "content": "4"},
    {"role": "user", "content": "What's 10x10?"},
    {"role": "assistant", "content": "100"}
]

# New question that depends on the FIRST message
messages.append({"role": "user", "content": "What's my name?"})

trimmed = apply_sliding_window(messages, max_window=4)

print("--- Trimmed history being sent ---")
for msg in trimmed:
    print(f"{msg['role']}: {msg['content']}")

answer = call_groq(trimmed)
print("--- Model's answer ---")
print(answer)