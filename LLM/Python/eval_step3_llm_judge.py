import os
import requests

def call_groq(question):
    response = requests.post(
        "https://api.groq.com/openai/v1/chat/completions",
        headers={
            "Authorization": f"Bearer {os.environ['GROQ_API_KEY']}",
            "Content-Type": "application/json",
        },
        json={
            "model": "openai/gpt-oss-120b",
            "temperature": 0,
            "messages": [{"role": "user", "content": question}],
        },
    )
    return response.json()["choices"][0]["message"]["content"]

def build_judge_prompt(question, expected, answer):
    return f"""You are grading an answer to a question.
Decide if the answer means the same as the expected answer.
Reply with only one word: PASS or FAIL.

Question: {question}
Expected answer: {expected}
Answer to grade: {answer}"""

def judge_score(question, expected, answer):
    prompt = build_judge_prompt(question, expected, answer)
    reply = call_groq(prompt)
    cleaned = reply.strip().upper()
    return 1 if cleaned == "PASS" else 0


test_cases = [
    {"question": "What is the capital of France?", "expected": "Paris"},
    {"question": "What is 7 x 8?", "expected": "56"},
    {"question": "What is the capital of Japan?", "expected": "Tokyo"},
]

passed = 0
for case in test_cases:
    answer = call_groq(case["question"])
    score = judge_score(case["question"], case["expected"], answer)
    passed += score
    print("PASS" if score == 1 else "FAIL", "|", case["question"], "|", answer)

print(f"Score: {passed}/{len(test_cases)}")
