#include <DigiKeyboard.h>
#include <avr/pgmspace.h>

// --- LED Pin ---
#define LED_PIN 1  // Built-in LED on Digispark

// --- Key codes ---
#define KEY_ENTER       40
#define KEY_BACKSPACE   42
#define KEY_T           23
#define KEY_W           26
#define KEY_CTRL        MOD_CONTROL_LEFT
#define KEY_ALT         MOD_ALT_LEFT
#ifndef KEY_F4
#define KEY_F4          61
#endif

// --- Search Query Definitions ---
#define SEARCH_COUNT 25

const char query0[]  PROGMEM = "python tutorial for beginners";
const char query1[]  PROGMEM = "machine learning basics";
const char query2[]  PROGMEM = "cloud computing explained";
const char query3[]  PROGMEM = "docker container guide";
const char query4[]  PROGMEM = "git commands cheat sheet";
const char query5[]  PROGMEM = "react vs angular comparison";
const char query6[]  PROGMEM = "sql database optimization";
const char query7[]  PROGMEM = "cybersecurity best practices";
const char query8[]  PROGMEM = "raspberry pi projects ideas";
const char query9[]  PROGMEM = "blockchain technology explained";
const char query10[] PROGMEM = "kubernetes deployment tutorial";
const char query11[] PROGMEM = "javascript array methods";
const char query12[] PROGMEM = "linux command line basics";
const char query13[] PROGMEM = "rest api design principles";
const char query14[] PROGMEM = "microservices architecture patterns";
const char query15[] PROGMEM = "neural networks introduction";
const char query16[] PROGMEM = "continuous integration ci cd";
const char query17[] PROGMEM = "data structures and algorithms";
const char query18[] PROGMEM = "aws cloud services overview";
const char query19[] PROGMEM = "mongodb vs postgresql";
const char query20[] PROGMEM = "tensorflow keras tutorial";
const char query21[] PROGMEM = "software design patterns";
const char query22[] PROGMEM = "agile scrum methodology";
const char query23[] PROGMEM = "web development roadmap";
const char query24[] PROGMEM = "network protocols tcp ip";

const char* const queries[] PROGMEM = {
  query0, query1, query2, query3, query4, query5, query6, query7, query8, query9,
  query10, query11, query12, query13, query14, query15, query16, query17, query18, query19,
  query20, query21, query22, query23, query24
};

// --- Settings ---
#define MAX_SEARCHES        25
#define TYPE_DELAY_MIN      80
#define TYPE_DELAY_MAX      160
#define TYPE_JITTER         40
#define MICRO_PAUSE_CHANCE  6
#define MICRO_PAUSE_MS      200
#define TYPO_CHANCE         12
#define PAGE_WAIT_MIN       3000
#define PAGE_WAIT_MAX       5000
#define MAX_QUERY_LEN       96

bool usedQueries[SEARCH_COUNT] = {false};
byte cnt = 0;
bool ledState = false;
unsigned long lastLedToggle = 0;

// --- LED Helper ---
void toggleLED() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void blinkingDelay(int ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
    if (millis() - lastLedToggle >= 100) {  // Blink every 100ms
      toggleLED();
      lastLedToggle = millis();
    }
    DigiKeyboard.delay(10);
  }
}

// --- Helpers ---
void typeCStr(const char* text) {
  for (const char* p = text; *p; ++p) {
    // LED blink during typing
    if (millis() - lastLedToggle >= 100) {
      toggleLED();
      lastLedToggle = millis();
    }
    
    if (isAlpha(*p) && random(TYPO_CHANCE) == 0) {
      char wrongChar = (char)random('a', 'z' + 1);
      DigiKeyboard.print(wrongChar);
      blinkingDelay(100);
      DigiKeyboard.sendKeyStroke(KEY_BACKSPACE);
      blinkingDelay(150);
    }

    DigiKeyboard.print(*p);

    int delayMs = random(TYPE_DELAY_MIN, TYPE_DELAY_MAX + 1);
    delayMs += random(-TYPE_JITTER, TYPE_JITTER + 1);
    blinkingDelay(max(40, delayMs));

    if (random(MICRO_PAUSE_CHANCE) == 0) {
      blinkingDelay(MICRO_PAUSE_MS);
    }
  }
}

void typeRandomQuery() {
  byte unusedCount = 0;
  for (byte i = 0; i < SEARCH_COUNT; i++) if (!usedQueries[i]) unusedCount++;
  if (unusedCount == 0) for (byte i = 0; i < SEARCH_COUNT; i++) usedQueries[i] = false;

  byte idx;
  byte attempts = 0;
  do {
    idx = random(SEARCH_COUNT);
    attempts++;
  } while (usedQueries[idx] && attempts < SEARCH_COUNT * 2);

  usedQueries[idx] = true;

  char buffer[MAX_QUERY_LEN];
  strncpy_P(buffer, (PGM_P)pgm_read_word(&queries[idx]), MAX_QUERY_LEN - 1);
  buffer[MAX_QUERY_LEN - 1] = '\0';

  typeCStr(buffer);
}

void doSearch() {
  DigiKeyboard.sendKeyStroke(KEY_T, KEY_CTRL);
  blinkingDelay(700);

  typeRandomQuery();
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  blinkingDelay(random(PAGE_WAIT_MIN, PAGE_WAIT_MAX + 1));

  DigiKeyboard.sendKeyStroke(KEY_W, KEY_CTRL);
  blinkingDelay(500);
}

// --- Main ---
void setup() {
  pinMode(LED_PIN, OUTPUT);  // Initialize LED pin
  digitalWrite(LED_PIN, LOW);
  
  DigiKeyboard.sendKeyStroke(0);
  randomSeed(analogRead(0));
  
  // Blink during initial delay
  blinkingDelay(5000);
}

void loop() {
  if (cnt < MAX_SEARCHES) {
    doSearch();
    cnt++;
  } else {
    DigiKeyboard.sendKeyStroke(KEY_F4, KEY_ALT);
    blinkingDelay(1000);
    
    // Keep LED blinking forever after completion
    while(1) {
      toggleLED();
      DigiKeyboard.delay(100);
    }
  }
}

bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
