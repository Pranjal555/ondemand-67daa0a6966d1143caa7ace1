Below is the C source code to consume the provided APIs using the `libcurl` library:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Replace these placeholders with actual values
#define API_KEY "<replace_api_key>"
#define EXTERNAL_USER_ID "<replace_external_user_id>"
#define QUERY "Put your query here"
#define RESPONSE_MODE "sync" // Change to "stream" if needed

// Helper function to handle response data
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    fwrite(ptr, size, nmemb, stdout); // Print response to stdout
    return total_size;
}

// Function to create a chat session
char* create_chat_session() {
    CURL *curl;
    CURLcode res;
    char *session_id = NULL;

    curl = curl_easy_init();
    if (curl) {
        const char *url = "https://api-dev.on-demand.io/chat/v1/sessions";
        const char *body = "{\"pluginIds\":[],\"externalUserId\":\"" EXTERNAL_USER_ID "\"}";

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "apikey: " API_KEY);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            // Extract session ID from response (simplified for demonstration)
            // In production, use a JSON parser to extract 'data.id'
            session_id = strdup("extracted_session_id"); // Replace with actual extraction logic
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return session_id;
}

// Function to submit a query
void submit_query(const char *session_id) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        char url[256];
        snprintf(url, sizeof(url), "https://api-dev.on-demand.io/chat/v1/sessions/%s/query", session_id);

        const char *body = "{\"endpointId\":\"predefined-openai-gpt4o\",\"query\":\"" QUERY "\","
                           "\"pluginIds\":[\"plugin-1712327325\",\"plugin-1713962163\",\"plugin-1713954536\","
                           "\"plugin-1713958591\",\"plugin-1713958830\",\"plugin-1713961903\",\"plugin-1713967141\"],"
                           "\"responseMode\":\"" RESPONSE_MODE "\",\"reasoningMode\":\"medium\"}";

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "apikey: " API_KEY);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

int main() {
    char *session_id = create_chat_session();
    if (session_id) {
        printf("Session ID: %s\n", session_id);
        submit_query(session_id);
        free(session_id);
    } else {
        fprintf(stderr, "Failed to create chat session.\n");
    }

    return 0;
}
```

### Notes:
1. **Dependencies**: Ensure `libcurl` is installed on your system. Compile the code using `gcc` with the `-lcurl` flag:
   ```bash
   gcc -o chat_api chat_api.c -lcurl
   ```
2. **Session ID Extraction**: Replace the placeholder `strdup("extracted_session_id")` with actual logic to parse the JSON response and extract the `data.id` field. Use a JSON parsing library like `cJSON` or `jansson` for this purpose.
3. **Streaming Mode**: If `responseMode` is set to `stream`, additional logic is required to handle Server-Sent Events (SSE). This example assumes synchronous responses.