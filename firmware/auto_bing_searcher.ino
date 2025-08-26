#include <DigiKeyboard.h>
#include <avr/pgmspace.h>

// --- Key codes ---
#define KEY_ENTER       40
#define KEY_BACKSPACE   42
#define KEY_T           23  // 't'
#define KEY_W           26  // 'w'
#define KEY_CTRL        MOD_CONTROL_LEFT
#define KEY_ALT         MOD_ALT_LEFT
#ifndef KEY_F4
#define KEY_F4          61  // HID: F1=58, F2=59, F3=60, F4=61
#endif

// --- Search Query Definitions ---
#define SEARCH_COUNT 25   // Only 25 searches now

const char query0[]  PROGMEM = "how to boil an egg";
const char query1[]  PROGMEM = "why is the sky blue";
const char query2[]  PROGMEM = "easy magic tricks";
const char query3[]  PROGMEM = "famous inventors list";
const char query4[]  PROGMEM = "best books to read";
const char query5[]  PROGMEM = "how to learn guitar";
const char query6[]  PROGMEM = "what animals live in the ocean";
const char query7[]  PROGMEM = "top tourist attractions";
const char query8[]  PROGMEM = "how to draw a dragon";
const char query9[]  PROGMEM = "world map countries";
const char query10[] PROGMEM = "interesting space facts";
const char query11[] PROGMEM = "how to plant a tree";
const char query12[] PROGMEM = "who invented the internet";
const char query13[] PROGMEM = "fun facts about animals";
const char query14[] PROGMEM = "rainbow colors in order";
const char query15[] PROGMEM = "what is artificial intelligence";
const char query16[] PROGMEM = "how deep is the ocean";
const char query17[] PROGMEM = "how many bones in the human body";
const char query18[] PROGMEM = "simple science experiments";
const char query19[] PROGMEM = "how to make slime at home";
const char query20[] PROGMEM = "famous quotes about life";
const char query21[] PROGMEM = "what causes thunder and lightning";
const char query22[] PROGMEM = "how to write a poem";
const char query23[] PROGMEM = "popular board games";
const char query24[] PROGMEM = "how do airplanes fly";

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
#define MICRO_PAUSE_CHANCE  6     // ~1 in 6 chars
#define MICRO_PAUSE_MS      200
#define TYPO_CHANCE         12    // ~1 in 12 chars will be mistyped and corrected
#define PAGE_WAIT_MIN       3000
#define PAGE_WAIT_MAX       5000
#define MAX_QUERY_LEN       96

bool usedQueries[SEARCH_COUNT] = {false};
byte cnt = 0;

// --- Helpers ---
void typeCStr(const char* text) {
  for (const char* p = text; *p; ++p) {
    // Occasionally make a typo and correct it
    if (isAlpha(*p) && random(TYPO_CHANCE) == 0) {
      char wrongChar = (char)random('a', 'z' + 1);
      DigiKeyboard.print(wrongChar);
      DigiKeyboard.delay(100);
      DigiKeyboard.sendKeyStroke(KEY_BACKSPACE);
      DigiKeyboard.delay(150);
    }

    DigiKeyboard.print(*p);

    int delayMs = random(TYPE_DELAY_MIN, TYPE_DELAY_MAX + 1);
    delayMs += random(-TYPE_JITTER, TYPE_JITTER + 1);
    DigiKeyboard.delay(max(40, delayMs));

    if (random(MICRO_PAUSE_CHANCE) == 0) {
      DigiKeyboard.delay(MICRO_PAUSE_MS);
    }
  }
}

void typeRandomQuery() {
  // Count unused
  byte unusedCount = 0;
  for (byte i = 0; i < SEARCH_COUNT; i++) if (!usedQueries[i]) unusedCount++;
  if (unusedCount == 0) for (byte i = 0; i < SEARCH_COUNT; i++) usedQueries[i] = false;

  // Pick an unused index
  byte idx;
  byte attempts = 0;
  do {
    idx = random(SEARCH_COUNT);
    attempts++;
  } while (usedQueries[idx] && attempts < SEARCH_COUNT * 2);

  usedQueries[idx] = true;

  // Copy from PROGMEM safely
  char buffer[MAX_QUERY_LEN];
  strncpy_P(buffer, (PGM_P)pgm_read_word(&queries[idx]), MAX_QUERY_LEN - 1);
  buffer[MAX_QUERY_LEN - 1] = '\0';

  typeCStr(buffer);
}

void doSearch() {
  // New tab
  DigiKeyboard.sendKeyStroke(KEY_T, KEY_CTRL);
  DigiKeyboard.delay(700);

  // Type query
  typeRandomQuery();
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(random(PAGE_WAIT_MIN, PAGE_WAIT_MAX + 1));

  // Close tab (Ctrl+W)
  DigiKeyboard.sendKeyStroke(KEY_W, KEY_CTRL);
  DigiKeyboard.delay(500);
}

// --- Main ---
void setup() {
  DigiKeyboard.sendKeyStroke(0);
  randomSeed(analogRead(0));
  DigiKeyboard.delay(5000); // Give time to open Edge manually
}

void loop() {
  if (cnt < MAX_SEARCHES) {
    doSearch();
    cnt++;
  } else {
    DigiKeyboard.sendKeyStroke(KEY_F4, KEY_ALT); // Close browser
    DigiKeyboard.delay(1000);
    for(;;); // Stop
  }
}

bool isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
