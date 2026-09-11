import os
import math
import requests

# --- Step 1: Get embedding for a piece of text ---
def get_embedding(text):
    api_key = os.environ["HF_API_KEY"]
    url = "https://router.huggingface.co/hf-inference/models/sentence-transformers/all-MiniLM-L6-v2/pipeline/feature-extraction"

    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    body = {"inputs": text}

    response = requests.post(url, headers=headers, json=body)
    embedding = response.json()
    return embedding


# --- Step 2: Compare two embeddings ---
def cosine_similarity(a, b):
    dot_product = 0.0
    magnitude_a = 0.0
    magnitude_b = 0.0

    for i in range(len(a)):
        dot_product += a[i] * b[i]
        magnitude_a += a[i] * a[i]
        magnitude_b += b[i] * b[i]

    magnitude_a = math.sqrt(magnitude_a)
    magnitude_b = math.sqrt(magnitude_b)

    return dot_product / (magnitude_a * magnitude_b)


# --- Step 3: Find the best matching document using embeddings ---
def find_best_match_by_embedding(question, documents):
    question_embedding = get_embedding(question)

    best_document = documents[0]
    best_score = -1.0

    for document in documents:
        document_embedding = get_embedding(document)
        score = cosine_similarity(question_embedding, document_embedding)
        print(f"Score for '{document}': {score}")

        if score > best_score:
            best_score = score
            best_document = document

    return best_document


# --- Step 4: Call Groq with the retrieved context ---
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


# --- Main pipeline ---
documents = [
    "Our office is located in Bangalore.",
    "Our refund policy allows returns within 30 days.",
    "Shipping takes 3-5 business days."
]

question = "How long for a refund"

best_match = find_best_match_by_embedding(question, documents)

print("Question:", question)
print("Best matching document:", best_match)

prompt = f"""Context: {best_match}

Question: {question}

Answer using only the context above."""

answer = call_groq(prompt)
print("LLM Answer:", answer)