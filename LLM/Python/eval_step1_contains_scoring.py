import os
import requests

test_cases = [
    {"question": "What is the capital of France?", "expected": "Paris"},
    {"question": "What is 7 times 8?", "expected": "56"},
    {"question": "What is the capital of Japan?", "expected": "Tokyo"},
]

def contains_score(reply, expected):
    return expected.lower() in reply.lower()

def call_groq(question):
    api_key = os.environ.get("GROQ_API_KEY")
    if not api_key:
        return "ERROR: GROQ_API_KEY not set"
    try:
        response = requests.post(
            "https://api.groq.com/openai/v1/chat/completions",
            headers={
                "Authorization": f"Bearer {api_key}",
                "Content-Type": "application/json",
            },
            json={
                "model": "openai/gpt-oss-120b",
                "temperature": 0,
                "messages": [{"role": "user", "content": question}],
            },
        )
        return response.json()["choices"][0]["message"]["content"]
    except Exception as e:
        return f"ERROR: {e}"

print("---- testing call_groq ----")
print(call_groq("What is the capital of France?"))   
print("---- running eval ----")
passed = 0

for test_case in test_cases:
    reply = call_groq(test_case["question"])
    ok = contains_score(reply, test_case["expected"])
    if ok:
        passed += 1
    print(f'{"PASS" if ok else "FAIL"} | {test_case["question"]} | got: {reply.strip()}')

print(f"Score: {passed}/{len(test_cases)}") 