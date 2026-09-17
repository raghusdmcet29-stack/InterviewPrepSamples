import os
import requests

def count_tokens(text):
    return len(text.split())

def apply_token_trimming(messages,max_tokens):
    trimmed = messages.copy()
    total_tokens = sum(count_tokens(m["content"]) for m in trimmed)

    while total_tokens > max_tokens and trimmed:
        removed = trimmed.pop(0)
        total_tokens -= count_tokens(removed["content"])


    return trimmed

def call_groq(messages):
    api_key = os.environ["GROQ_API_KEY"]
    response = requests.post(
        "https://api.groq.com/openai/v1/chat/completions",
        headers={"Authorization": f"Bearer {api_key}"},
        json={
            "model": "openai/gpt-oss-120b",
            "messages": messages,
            "temperature": 0
        }
    )
    return response.json()["choices"][0]["message"]["content"]

# Hand-traced example (matches Swift Piece 2 test)
live_history = [
    {"role": "user", "content": "My name is Anuraag and I live in Bangalore"},
    {"role": "user", "content": "I work as a Technical Lead in macOS development"},
    {"role": "assistant", "content": "That's great, macOS development is a solid specialty"},
    {"role": "user", "content": "What's my name?"}
]

# Tight budget - should forget the name
live_trimmed_tight = apply_token_trimming(live_history, max_tokens=10)
print("\nMessages sent to model (tight budget):")
for msg in live_trimmed_tight:
    print(f"  {msg['role']}: {msg['content']}")
answer_tight = call_groq(live_trimmed_tight)
print(f"Model answer: {answer_tight}")

# Larger budget - should remember the name
live_trimmed_large = apply_token_trimming(live_history, max_tokens=40)
print("\nMessages sent to model (larger budget):")
for msg in live_trimmed_large:
    print(f"  {msg['role']}: {msg['content']}")
answer_large = call_groq(live_trimmed_large)
print(f"Model answer: {answer_large}")