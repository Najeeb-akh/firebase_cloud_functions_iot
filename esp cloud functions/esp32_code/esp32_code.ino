#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// WiFi credentials
const char* WIFI_SSID = "Nezam’s House";
const char* WIFI_PASSWORD = "******";

// Firebase project API Key
#define API_KEY "AI*********"

// Firebase Realtime Database URL
#define DATABASE_URL "https://proplanner-****-default-rtdb.firebaseio.com/"

// Firebase user email and password
#define USER_EMAIL "naj@a.com"
#define USER_PASSWORD "123456"

// Define Firebase Data objects
FirebaseData fbdo;
FirebaseData fbdoWord; // Separate object for word stream
FirebaseAuth auth;
FirebaseConfig config;

// Button pin
const int BUTTON_PIN = 2;
int lastButtonState = HIGH;

// Variables to track button counter
int buttonCounter = 0;
String buttonCounterWord = "";

// Track authentication status
bool signedIn = false;

// Timer variables for reading the word value
unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 2000; // Read every 2 seconds

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("ESP32 Firebase Counter Demo with Word Translation");

  // Initialize button pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Configure Firebase */
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  // Set email and password for authentication
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the callback function for the token generation task
  config.token_status_callback = tokenStatusCallback;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Waiting for authentication...");
}

void loop() {
  // Check if Firebase is ready and signed in
  if (Firebase.ready() && !signedIn) {
    Serial.println("Firebase authentication successful!");
    signedIn = true;
  }

  if (signedIn) {
    // Read the current button state
    int buttonState = digitalRead(BUTTON_PIN);

    // Check if button was just pressed (change from HIGH to LOW)
    if (buttonState == LOW && lastButtonState == HIGH) {
      Serial.println("Button pressed! Incrementing counter...");

      // Increment the counter in Firebase
      incrementCounter();

      // Debounce delay
      delay(50);
    }

    // Save the current button state for the next loop
    lastButtonState = buttonState;

    // Read the button_counter_word at regular intervals
    unsigned long currentTime = millis();
    if (currentTime - lastReadTime >= READ_INTERVAL) {
      readButtonCounterWord();
      lastReadTime = currentTime;
    }
  }
}

void incrementCounter() {
  // Update our local counter
  buttonCounter++;
  if (buttonCounter > 100) {
    buttonCounter = 1; // Reset if over 100
  }
  
  // Path to the counter in Firebase RTDB
  // We'll use /button_counter directly to match your cloud function
  String path = "/button_counter";

  // Update the value with our counter
  if (Firebase.RTDB.setInt(&fbdo, path, buttonCounter)) {
    Serial.print("Counter updated to: ");
    Serial.println(buttonCounter);
  } else {
    Serial.print("Counter update failed: ");
    Serial.println(fbdo.errorReason());
  }
}

void readButtonCounterWord() {
  // Path to the word counter in Firebase RTDB
  String path = "/button_counter_word";

  // Read the current word value
  if (Firebase.RTDB.getString(&fbdoWord, path)) {
    buttonCounterWord = fbdoWord.stringData();
    
    // Print both number and word to serial monitor
    Serial.println("--------------------");
    Serial.print("Button counter: ");
    Serial.println(buttonCounter);
    Serial.print("As word: ");
    Serial.println(buttonCounterWord);
    Serial.println("--------------------");
  } else {
    Serial.print("Failed to read word counter: ");
    Serial.println(fbdoWord.errorReason());
  }
}