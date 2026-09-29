import re
import os
import requests

def whole_word_score(response,expected):
    escaped = re.escape(expected)
    pattern = r"(?<!\d)" + escaped + r"(?!\d)"

    if re.search(pattern,response,re.IGNORECASE):
        return 1
    return 0


#print(whole_word_score("The answer is 56.", "56"))
#print(whole_word_score("156", "56"))
#print(whole_word_score("[56]", "56"))
#print(whole_word_score("5566", "56"))

test_cases = [
    {"question": "What is the capital of France?", "expected": "Paris"},
    {"question": "What is 7 times 8?", "expected": "56"},
    {"question": "What is the capital of Japan?", "expected": "Tokyo"},
]

#for test_case in test_cases:
#    print("Q:", test_case["question"], "| expected:", test_case["expected"])

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

#answer = call_groq("What is 7 times 8?")
#print("Model said:", answer)
total = 0
for test_case in test_cases:
    answer = call_groq(test_case["question"])
    score = whole_word_score(answer, test_case["expected"])
    total += score
    print("PASS" if score == 1 else "FAIL", "|", test_case["question"], "|", answer)
print("Score:", total, "/", len(test_cases))