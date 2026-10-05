import os
import re
import requests

# --- Test set (same 10 cases as the Swift version) ---
test_cases = [
    {"question": "What is the capital of France?", "expected": "Paris"},
    {"question": "What is 7 times 8?", "expected": "56"},
    {"question": "What is the capital of Japan?", "expected": "Tokyo"},
    {"question": "What is 9 times 9?", "expected": "81"},
    {"question": "What is the capital of Italy?", "expected": "Rome"},
    {"question": "What is 12 plus 15?", "expected": "27"},
    {"question": "What is the capital of Spain?", "expected": "Madrid"},
    {"question": "What is 100 minus 37?", "expected": "63"},
    {"question": "What is the capital of Egypt?", "expected": "Cairo"},
    {"question": "What is 6 times 7?", "expected": "42"},
]

# --- Report functions ---
def pass_count(scores):
    return sum(scores)


def pass_rate_report(scores):
    total = len(scores)
    if total == 0:
        return "No test cases to score"
    passes = pass_count(scores)
    percent = passes / total * 100
    return f"{passes}/{total} = {percent:.1f}%"

# --- Scoring (same whole-word rule as Step 2) ---
def whole_word_score(response, expected):
    pattern = r"(?<!\d)" + re.escape(expected) + r"(?!\d)"
    return 1 if re.search(pattern, response, re.IGNORECASE) else 0


# --- Model call (synchronous, key from environment) ---
def call_groq(question, prompt):  # CHANGED: added prompt
    api_key = os.environ["GROQ_API_KEY"]
    response = requests.post(
        "https://api.groq.com/openai/v1/chat/completions",
        headers={
            "Authorization": f"Bearer {api_key}",
            "Content-Type": "application/json",
        },
        json={
            "model": "openai/gpt-oss-120b",
            "temperature": 0,
            "messages": [
                {"role": "system", "content": prompt},  # CHANGED: new system message
                {"role": "user", "content": question},
            ],
        },
        timeout=60,
    )
    response.raise_for_status()
    return response.json()["choices"][0]["message"]["content"]

# CHANGED: small test instead of the loop
def run_eval(prompt):
    scores = []
    for tc in test_cases:
        answer = call_groq(tc["question"], prompt)
        scores.append(whole_word_score(answer, tc["expected"]))
    return scores


prompt_a = "Answer briefly."
prompt_b = "Answer in words only, never use digits."

scores_a = run_eval(prompt_a)
scores_b = run_eval(prompt_b)

print(f"Prompt A: {pass_rate_report(scores_a)}")
print(f"Prompt B: {pass_rate_report(scores_b)}")