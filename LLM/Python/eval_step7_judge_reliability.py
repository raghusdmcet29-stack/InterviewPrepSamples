import os
import time
from collections import Counter
import requests

cases = [
    {"question": "What is the capital of France?", "answer": "Paris", "label": "PASS"},
    {"question": "What is the capital of France?", "answer": "Lyon", "label": "FAIL"},
    {"question": "What is 7 times 8?", "answer": "Fifty-six", "label": "PASS"},
    {"question": "What is 7 times 8?", "answer": "54", "label": "FAIL"},
    {"question": "What is the capital of France?", "answer": "Paris is the capital of Germany", "label": "FAIL"},
    {"question": "What is the capital of Australia?", "answer": "Sydney", "label": "FAIL"},
    {"question": "What is the capital of Australia?", "answer": "Canberra, not Sydney", "label": "PASS"},
    {"question": "What is 7 times 8?", "answer": "56.0", "label": "PASS"},
]

RUNS_PER_CASE = 5


def call_groq_once(prompt):
    key = os.environ["GROQ_API_KEY"]
    response = requests.post(
        "https://api.groq.com/openai/v1/chat/completions",
        headers={"Authorization": f"Bearer {key}", "Content-Type": "application/json"},
        json={
            "model": "openai/gpt-oss-120b",
            "temperature": 0,
            "messages": [{"role": "user", "content": prompt}],
        },
        timeout=60,
    )
    response.raise_for_status()
    return response.json()["choices"][0]["message"]["content"]


def call_groq(prompt):
    for attempt in range(1, 4):
        try:
            return call_groq_once(prompt)
        except Exception as error:
            print(f"API error (try {attempt}): {error}")
            if attempt < 3:
                time.sleep(attempt * 5)
    return None


def build_judge_prompt(question, answer):
    return (
        f"Question: {question}\nAnswer: {answer}\n"
        "Is the answer correct? Reply with exactly one word: PASS or FAIL."
    )


# Returns "PASS", "FAIL", "OTHER" (unexpected reply), or None (API failed)
def judge_verdict(question, answer):
    reply = call_groq(build_judge_prompt(question, answer))
    if reply is None:
        return None
    cleaned = reply.strip().upper()
    if cleaned in ("PASS", "FAIL"):
        return cleaned
    return "OTHER"


total_matches = 0
total_verdicts = 0
false_passes = 0
consistencies = []

for index, case in enumerate(cases):
    verdicts = []
    for _ in range(RUNS_PER_CASE):
        verdict = judge_verdict(case["question"], case["answer"])
        if verdict is not None:
            verdicts.append(verdict)
        time.sleep(2)

    if not verdicts:
        print(f"Case {index + 1}: no verdicts (API errors)")
        continue

    top = max(Counter(verdicts).values())
    consistency = top / len(verdicts) * 100
    consistencies.append(consistency)

    matches = sum(1 for v in verdicts if v == case["label"])
    total_matches += matches
    total_verdicts += len(verdicts)
    if case["label"] == "FAIL":
        false_passes += sum(1 for v in verdicts if v == "PASS")

    print(f'Case {index + 1}: {case["question"]} | "{case["answer"]}" | label {case["label"]}')
    print(f"  verdicts: {' '.join(verdicts)}")
    print(f"  consistency {top}/{len(verdicts)} = {consistency:.1f}%")

if total_verdicts == 0:
    print("No verdicts collected")
    raise SystemExit(1)

agreement = total_matches / total_verdicts * 100
avg_consistency = sum(consistencies) / len(consistencies)
print("")
print(f"Agreement with labels: {total_matches}/{total_verdicts} = {agreement:.1f}%")
print(f"Average consistency: {avg_consistency:.1f}%")
print(f"False passes: {false_passes}")