import os
import requests

# MARK: - Core API call (same as Step 1)
def call_groq(messages):
    url = "https://api.groq.com/openai/v1/chat/completions"
    api_key = os.environ.get("GROQ_API_KEY")
    if not api_key:
        raise RuntimeError("GROQ_API_KEY not set")

    headers = {
        "Content-Type": "application/json",
        "Authorization": f"Bearer {api_key}"
    }

    payload = {
        "model": "openai/gpt-oss-120b",
        "temperature": 0,
        "messages": messages
    }

    response = requests.post(url, headers=headers, json=payload)
    data = response.json()
    return data["choices"][0]["message"]["content"]

# MARK: - New for Step 2: summarize old messages
def summarize_messages(messages):
    conversation_text = "\n".join(f"{m['role']}: {m['content']}" for m in messages)
    instruction = f"Summarize the key facts from this conversation in 1-2 sentences:\n\n{conversation_text}"
    summary_request = [{"role": "user", "content": instruction}]
    return call_groq(summary_request)

# MARK: - New for Step 2: split into old/recent, summarize old
def apply_summarization(messages, max_window):
    if len(messages) <= max_window:
        return messages

    old_messages = messages[:-max_window]
    recent_messages = messages[-max_window:]

    summary_text = summarize_messages(old_messages)
    summary_message = {
        "role": "system",
        "content": f"The user previously told you the following facts — treat them as known information you should use when answering: {summary_text}"
    }
    return [summary_message] + recent_messages

# MARK: - Test
if __name__ == "__main__":
    conversation = [
        {"role": "user", "content": "Hi, my name is Anuraag and I'm based in Bangalore."},
        {"role": "assistant", "content": "Nice to meet you, Anuraag! How can I help today?"},
        {"role": "user", "content": "I'm looking for a good Python tutorial for beginners."},
        {"role": "assistant", "content": "Sure — I'd recommend the official Python docs tutorial, or Real Python."},
        {"role": "user", "content": "Thanks. Also, what's a good laptop for coding under $800?"},
        {"role": "assistant", "content": "A good option would be a mid-range laptop with 8GB RAM and an SSD."}
    ]

    trimmed = apply_summarization(conversation, max_window=2)

    print("--- Messages sent to model ---")
    for msg in trimmed:
        print(f"{msg['role']}: {msg['content']}")

    follow_up = trimmed + [{"role": "user", "content": "What's my name?"}]
    answer = call_groq(follow_up)
    print("\n--- Final answer ---")
    print(answer)