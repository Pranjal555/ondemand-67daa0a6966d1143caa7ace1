
import requests

# Replace these placeholders with actual values
API_KEY = "<replace_api_key>"
EXTERNAL_USER_ID = "<replace_external_user_id>"
QUERY = "Put your query here"
PLUGIN_IDS = [
    "plugin-1712327325", "plugin-1713962163", "plugin-1713954536",
    "plugin-1713958591", "plugin-1713958830", "plugin-1713961903",
    "plugin-1713967141"
]
ENDPOINT_ID = "predefined-openai-gpt4o"
RESPONSE_MODE = "sync"  # Change to "stream" if needed
REASONING_MODE = "medium"

# Create Chat Session
def create_chat_session():
    url = "https://api-dev.on-demand.io/chat/v1/sessions"
    headers = {"apikey": API_KEY}
    body = {"pluginIds": [], "externalUserId": EXTERNAL_USER_ID}

    response = requests.post(url, json=body, headers=headers)
    if response.status_code == 201:
        session_id = response.json().get("data", {}).get("id")
        if session_id:
            return session_id
        else:
            raise ValueError("Session ID not found in response")
    else:
        raise Exception(f"Failed to create chat session: {response.status_code}, {response.text}")

# Submit Query
def submit_query(session_id):
    url = f"https://api-dev.on-demand.io/chat/v1/sessions/{session_id}/query"
    headers = {"apikey": API_KEY}
    body = {
        "endpointId": ENDPOINT_ID,
        "query": QUERY,
        "pluginIds": PLUGIN_IDS,
        "responseMode": RESPONSE_MODE,
        "reasoningMode": REASONING_MODE
    }

    if RESPONSE_MODE == "sync":
        response = requests.post(url, json=body, headers=headers)
        if response.status_code == 200:
            return response.json()
        else:
            raise Exception(f"Failed to submit query: {response.status_code}, {response.text}")
    elif RESPONSE_MODE == "stream":
        # Handle Server-Sent Events (SSE) using requests
        with requests.post(url, json=body, headers=headers, stream=True) as response:
            if response.status_code == 200:
                for line in response.iter_lines():
                    if line:
                        print(line.decode('utf-8'))
            else:
                raise Exception(f"Failed to submit query (stream mode): {response.status_code}, {response.text}")
    else:
        raise ValueError("Invalid response mode")

# Main Execution
if __name__ == "__main__":
    try:
        # Step 1: Create Chat Session
        session_id = create_chat_session()
        print(f"Chat session created successfully. Session ID: {session_id}")

        # Step 2: Submit Query
        result = submit_query(session_id)
        if RESPONSE_MODE == "sync":
            print("Query Response:", result)
        else:
            print("Streaming response received.")
    except Exception as e:
        print(f"Error: {e}")
