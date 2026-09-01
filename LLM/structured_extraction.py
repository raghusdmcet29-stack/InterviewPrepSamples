import os
import json
import requests
from pydantic import BaseModel, ValidationError

# --- Schema definition ---
# This is the "contract" the LLM's output must satisfy.
class ExtractedEvent(BaseModel):
    event_name: str
    date: str
    location: str
    attendee_count: int


API_KEY = os.environ.get("GROQ_API_KEY")
API_URL = "https://api.groq.com/openai/v1/chat/completions"
MODEL = "openai/gpt-oss-120b"


def call_llm(prompt: str) -> str:
    headers = {
        "Authorization": f"Bearer {API_KEY}",
        "Content-Type": "application/json",
    }
    body = {
        "model": MODEL,
        "temperature": 0.2,
        "messages": [{"role": "user", "content": prompt}],
    }
    response = requests.post(API_URL, headers=headers, json=body)
    response.raise_for_status()
    return response.json()["choices"][0]["message"]["content"]


def extract_event(text: str) -> ExtractedEvent:
    prompt = f"""Extract event details from this text and respond with ONLY valid JSON, no other text.
Schema: {{"event_name": string, "date": string, "location": string, "attendee_count": integer}}

Text: {text}"""

    raw_output = call_llm(prompt)
    print(f"Raw LLM output: {raw_output}")

    data = json.loads(raw_output)
    return ExtractedEvent(**data)


if __name__ == "__main__":
    sample_text = "The annual tech conference DevSummit will be held on March 15, 2027 in Austin, Texas, expecting around 500 attendees."
    result = extract_event(sample_text)
    print(f"Validated result: {result}")