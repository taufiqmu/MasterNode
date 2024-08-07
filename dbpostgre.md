Certainly! Here's a simplified version of the ESP32 code to send data to a Supabase PostgreSQL database via its REST API. This example uses the Arduino framework for simplicity. The code will connect to WiFi, then send a simple POST request with data to Supabase.

### Prerequisites

1. **Create a Supabase Project** and a table (e.g., `sensor_data` with a `value` column).
2. **Obtain Supabase URL and API Key** from the Supabase dashboard.

### Simplified ESP32 Code

**Code for ESP32 (Arduino Framework)**:

```cpp
#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Replace with your Supabase details
const char* supabaseURL = "https://your-project-id.supabase.co/rest/v1/sensor_data";
const char* supabaseApiKey = "your_anon_key";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    http.begin(supabaseURL);
    http.addHeader("Authorization", "Bearer " + String(supabaseApiKey));
    http.addHeader("Content-Type", "application/json");
    
    String payload = "{\"value\": 123}"; // Example payload
    int httpResponseCode = http.POST(payload);
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.println("Error on HTTP request");
    }
    
    http.end();
  }

  delay(10000); // Wait for 10 seconds before sending the next request
}
```

### Explanation

1. **WiFi Connection**: The ESP32 connects to your WiFi network using the SSID and password provided.
2. **HTTP Request**:
   - **URL**: Set to your Supabase REST API endpoint.
   - **Headers**: Includes `Authorization` with the Supabase API key and `Content-Type` set to `application/json`.
   - **Payload**: Contains the data you want to send. Modify this as needed based on your table schema.

3. **Loop**: Sends the data every 10 seconds. Adjust the delay if needed.

### Notes

- **Replace Placeholders**: Make sure to replace `your_SSID`, `your_PASSWORD`, `your-project-id`, and `your_anon_key` with your actual values.
- **Payload Format**: Ensure that the payload matches the format expected by your Supabase table.

This code provides a basic example of sending data to a Supabase database. Depending on your specific needs, you might need to handle more complex scenarios such as authentication, error handling, or data formatting.