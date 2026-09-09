import os
import requests

documents = [
    "Our refund policy allows returns within 30 days.",
    "Our office is located in Bangalore.",
    "Shipping takes 3-5 business days."
]

def overlap_score(question, document):
    question_words = set(question.lower().split())
    document_words = set(document.lower().split())
    return len(question_words.intersection(document_words))


def find_best_match(question, documents):
    best_document = documents[0]
    best_score = 0

    for document in documents:
        score = overlap_score(question, document)
        if score > best_score:
            best_score = score
            best_document = document

    return best_document

question = "How long for a refund"
best_match = find_best_match(question, documents)

print(f"Question: {question}")
print(f"Best matching document: {best_match}") 

prompt = f"""Context: {best_match}

Question: {question}

Answer using only the context above."""

def call_groq(prompt):
    api_key = os.environ["GROQ_API_KEY"]

    url = "https://api.groq.com/openai/v1/chat/completions"
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    body = {
        "model": "openai/gpt-oss-120b",
        "messages": [{"role": "user", "content": prompt}],
        "temperature": 0.2
    }

    response = requests.post(url, headers=headers, json=body)
    data = response.json()
    return data["choices"][0]["message"]["content"]

answer = call_groq(prompt)
print(f"LLM Answer: {answer}")
