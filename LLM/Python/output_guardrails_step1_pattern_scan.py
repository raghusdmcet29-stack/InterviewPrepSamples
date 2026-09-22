import re
import os
import requests
import asyncio

def contains_leaked_secret(response: str) -> bool :
    patterns = [
        r"sk-[A-Za-z0-9]{20,}",      # OpenAI-style key
        r"gsk_[A-Za-z0-9]{20,}",     # Groq-style key
        r"hf_[A-Za-z0-9]{20,}"       # Hugging Face-style key
    ]
    for pattern in patterns:
        if re.search(pattern,response):
            return True

    return False   


def redact_leaked_secrets(response: str) -> str:
    patterns = [
        r"sk-[A-Za-z0-9]{20,}",
        r"gsk_[A-Za-z0-9]{20,}",
        r"hf_[A-Za-z0-9]{20,}"
    ]
    result = response
    for pattern in patterns:
        result = re.sub(pattern,"[REDACTED]",result)
    return result


async def get_safe_response(prompt: str) -> str:
    messages = [{"role": "user", "content": prompt}]
    raw_response = call_groq(messages)  # no await — call_groq is synchronous
    safe_response = redact_leaked_secrets(raw_response)
    return safe_response


def call_groq(messages):
    api_key = os.environ["GROQ_API_KEY"]

    url = "https://api.groq.com/openai/v1/chat/completions"

    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    body = {
        "model": "openai/gpt-oss-120b",
        "temperature": 0,
        "messages": messages
    }
    response = requests.post(url, headers=headers, json=body)
    data = response.json()
    return data["choices"][0]["message"]["content"]




async def main():
    simulated_leak = "Here's your Groq client setup: client.apiKey = gsk_aBcD1234EfGh5678IjKl9012"
    cleaned = redact_leaked_secrets(simulated_leak)
    print(cleaned)

    real_result = await get_safe_response("What's the capital of France?")
    print(real_result)

if __name__ == "__main__":
    asyncio.run(main())