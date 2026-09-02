import os
import json
import requests

tools = [
    {
        "type": "function",
        "function": {
            "name": "get_weather",
            "description": "Returns current weather for a city",
            "parameters": {
                "type": "object",
                "properties": {
                    "city": {"type": "string", "description": "The city name"}
                },
                "required": ["city"]
            }
        }
    },
    {
        "type": "function",
        "function": {
            "name": "convert_currency",
            "description": "Converts an amount from one currency to another",
            "parameters": {
                "type": "object",
                "properties": {
                    "amount": {"type": "string"},
                    "from": {"type": "string"},
                    "to": {"type": "string"}
                },
                "required": ["amount", "from", "to"]
            }
        }
    }
]


def call_llm_native(user_request: str) -> dict:
    api_key = os.environ.get("GROQ_API_KEY")
    if not api_key:
        raise RuntimeError("GROQ_API_KEY not set in environment")

    url = "https://api.groq.com/openai/v1/chat/completions"
    headers = {
        "Authorization": f"Bearer {api_key}",
        "Content-Type": "application/json"
    }
    body = {
        "model": "openai/gpt-oss-120b",
        "temperature": 0.2,
        "messages": [
            {"role": "user", "content": user_request}
        ],
        "tools": tools,
        "tool_choice": "auto"
    }

    response = requests.post(url, headers=headers, json=body)
    response.raise_for_status()
    data = response.json()

    choices = data.get("choices", [])
    if not choices:
        raise RuntimeError("No choices returned from API")

    tool_calls = choices[0]["message"].get("tool_calls")
    if not tool_calls:
        raise RuntimeError("Model did not return a tool call")

    return tool_calls[0]


def get_weather(city: str) -> str:
    return f"It's sunny and 28°C in {city}."


def convert_currency(amount: str, from_currency: str, to_currency: str) -> str:
    return f"{amount} {from_currency} is approximately {amount} {to_currency} (mock conversion, no real rate applied)."


def dispatch_native(tool_call: dict) -> str:
    function = tool_call["function"]
    name = function["name"]
    arguments = json.loads(function["arguments"])

    if name == "get_weather":
        return get_weather(arguments["city"])
    elif name == "convert_currency":
        return convert_currency(arguments["amount"], arguments["from"], arguments["to"])
    else:
        return f"Error: unknown tool '{name}'"


if __name__ == "__main__":
    user_request = "What's the weather like in Bangalore?"
    try:
        tool_call = call_llm_native(user_request)
        print(f"LLM chose tool: {tool_call['function']['name']}, raw arguments: {tool_call['function']['arguments']}")

        result = dispatch_native(tool_call)
        print(f"Result: {result}")
    except Exception as e:
        print(f"Error: {e}")