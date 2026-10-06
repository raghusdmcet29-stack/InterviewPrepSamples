import os
import re
import time
import unicodedata
import requests

test_cases = [
    {"question": "What is the capital of France?", "accepted": ["Paris"]},
    {"question": "What is the capital of Japan?", "accepted": ["Tokyo"]},
    {"question": "What is the capital of Italy?", "accepted": ["Rome"]},
    {"question": "What is 7 times 8?", "accepted": ["56", "fifty-six", "fifty six"]},
    {"question": "What is 9 times 6?", "accepted": ["54", "fifty-four", "fifty four"]},
    {"question": "What is 12 plus 13?", "accepted": ["25", "twenty-five", "twenty five"]},
    {"question": "What is 100 minus 37?", "accepted": ["63", "sixty-three", "sixty three"]},
    {"question": "What is the capital of Canada?", "accepted": ["Ottawa"]},
    {"question": "How many days are in a week?", "accepted": ["7", "seven"]},
    {"question": "How many sides does a triangle have?", "accepted": ["3", "three"]},
]


def normalize(text):
    result = unicodedata.normalize("NFKC", text).lower()
    for dash in ["\u2010", "\u2011", "\u2012", "\u2013", "\u2014", "\u2212"]:
        result = result.replace(dash, "-")
    result = result.replace("-", " ")
    result = re.sub(r"\s+", " ", result)
    return result.strip()


def matches_whole_word(response, form):
    clean_response = normalize(response)
    pattern = r"(?<![a-z0-9])" + re.escape(normalize(form)) + r"(?![a-z0-9])"
    return re.search(pattern, clean_response) is not None


def words_and_digits_score(response, accepted):
    for form in accepted:
        if matches_whole_word(response, form):
            return 1
    return 0


def digits_only_score(response, accepted):
    return 1 if matches_whole_word(response, accepted[0]) else 0


def pass_rate_report(scores):
    total = len(scores)
    if total == 0:
        return "No test cases to score"
    passes = sum(scores)
    rate = passes / total * 100
    return f"{passes}/{total} = {rate:.1f}%"


def call_groq(question, prompt):
    api_key = os.environ.get("GROQ_API_KEY")
    if not api_key:
        return "ERROR: GROQ_API_KEY not set"
    body = {
        "model": "openai/gpt-oss-120b",
        "temperature": 0,
        "messages": [
            {"role": "system", "content": prompt},
            {"role": "user", "content": question},
        ],
    }
    headers = {"Authorization": f"Bearer {api_key}", "Content-Type": "application/json"}
    try:
        r = requests.post("https://api.groq.com/openai/v1/chat/completions",
                          json=body, headers=headers, timeout=60)
        return r.json()["choices"][0]["message"]["content"]
    except Exception as e:
        return f"ERROR: {e}"


def run_eval(prompt, use_words):
    scores = []
    for case in test_cases:
        response = call_groq(case["question"], prompt)
        if use_words:
            score = words_and_digits_score(response, case["accepted"])
        else:
            score = digits_only_score(response, case["accepted"])
        scores.append(score)
        verdict = "PASS" if score == 1 else "FAIL"
        print(f"{verdict}: {case['question']} -> {response}")
    return scores


# Offline checks first (no API call)
acc = ["56", "fifty-six", "fifty six"]
print("1:", words_and_digits_score("The answer is 56.", acc))
print("2:", words_and_digits_score("Fifty-six.", acc))
print("3:", words_and_digits_score("156", acc))
print("4:", words_and_digits_score("Fifty six", acc))
print("5:", digits_only_score("Fifty-six.", acc))

prompt_a = "Answer briefly."
prompt_b = "Answer in words only, never use digits."

a_scores = run_eval(prompt_a, True)
print("Prompt A, words+digits scorer: " + pass_rate_report(a_scores))

b_old_scores = run_eval(prompt_b, False)
print("Prompt B, digits-only scorer: " + pass_rate_report(b_old_scores))

b_new_scores = run_eval(prompt_b, True)
print("Prompt B, words+digits scorer: " + pass_rate_report(b_new_scores))