# 🔍 Auto Bing Searcher (ATtiny85 + DigiKeyboard)

This repository contains an Arduino sketch for an ATtiny85 (Digispark-style) USB board that automates web searches in a desktop browser (designed for Microsoft Edge / Bing). The device opens a new tab, types a human-like search query, presses Enter, waits for the results page, closes the tab, and repeats for a fixed number of searches. After the final search it cleanly closes the browser window and stops.

> ⚠️ **Use responsibly.** This project is for education and automation testing only. Don't use it to spam services, violate terms of use, or automate actions that are illegal or unethical.

---

## Repository layout

```
auto-bing-searcher/
│
├── firmware/
│   └── auto_bing_searcher.ino     # Main Arduino sketch for ATtiny85
│
├── docs/
│   └── usage_guide.md             # Extended instructions & troubleshooting
│
├── LICENSE                        # MIT (or your chosen license)
├── README.md                      # Project overview (this file)
└── .gitignore                     # Ignore build/temp files
```

---

## Features

* Performs **25 searches** by default (configurable).
* Opens a **new tab** for each search so the omnibox has focus.
* **Types** queries with human-like spacing, jitter, and rare typos that are corrected.
* **Closes each tab** (`Ctrl+W`) after the results load to avoid tab bloat.
* After all searches, sends **Alt+F4** to close the browser and stops.
* Designed to run on **Digispark / ATtiny85** boards (uses `DigiKeyboard.h`).

---

## Quickstart

1. **Clone the repo**

   ```bash
   git clone https://github.com/YOUR-USERNAME/auto-bing-searcher.git
   cd auto-bing-searcher/firmware
   ```

2. **Open** `auto_bing_searcher.ino` in the Arduino IDE.

3. **Install Digispark (if you haven't):**

   * Add the Digispark core to Arduino (Digistump instructions) and install drivers.
   * Make sure `DigiKeyboard.h` is available (comes with Digispark core).

4. **Board selection:**

   * In Arduino IDE choose: `Tools → Board → Digispark (Default - 16.5MHz)`.

5. **Prepare your browser:**

   * Open Microsoft Edge (or another Chromium-based browser).
   * Ensure the browser is the **focused window** before plugging in the device.
   * Recommended: set keyboard layout to **US (English)** so DigiKeyboard maps correctly.

6. **Upload:**

   * Plug in the Digispark when prompted by the Arduino IDE and upload the sketch.
   * The device waits \~5 seconds (setup time) then starts performing searches.

---

## Configuration (constants in the `.ino`)

Inside `auto_bing_searcher.ino` you will find a set of `#define` values you can tweak:

```cpp
#define MAX_SEARCHES 25        // Number of searches to perform
#define PAGE_WAIT_MIN 3000     // Minimum wait after pressing Enter (milliseconds)
#define PAGE_WAIT_MAX 5000     // Maximum wait after pressing Enter (milliseconds)
#define TYPE_DELAY_MIN 80      // Minimum inter-character typing delay (ms)
#define TYPE_DELAY_MAX 160     // Maximum inter-character typing delay (ms)
#define TYPE_JITTER 40         // Random jitter added/subtracted from base delay
#define MICRO_PAUSE_CHANCE 6   // Roughly 1-in-N chars will cause a short pause
#define MICRO_PAUSE_MS 200     // Length of the micro-pause (ms)
#define TYPO_CHANCE 12         // Roughly 1-in-N characters will be mistyped and corrected
```

Change these values to make typing slower/faster or to change how "human" the typing appears.

---

## How it works (high level)

1. The sketch opens a **new tab** (Ctrl+T) to ensure a clean omnibox.
2. It selects or focuses the omnibox/address bar (Alt+D / Ctrl+L fallback may be used depending on browser).
3. It types a random query from `PROGMEM` using safe `pgm_read_word` and `strncpy_P`.
4. During typing it introduces: per-character jitter, occasional micro-pauses, and rare typos which are immediately corrected using backspace.
5. Presses Enter to search, waits for results to load, then closes the tab with `Ctrl+W`.
6. Repeats until the configured limit is reached, then sends `Alt+F4` to close the browser and halts.

---

## Troubleshooting

* **Backspace causing browser to go back:**

  * Backspace navigates back only if the browser area has focus (not the omnibox). To avoid this, the sketch opens a new tab and attempts to focus the omnibox before typing. If you still see backspace navigation, check that the browser window lost focus when the device started — ensure Edge is frontmost when the device is plugged in.

* **Typing wrong characters or layout mismatch:**

  * DigiKeyboard HID mapping assumes a **US keyboard layout**. If your system uses a different layout (e.g. AZERTY, QWERTZ), keys will be mis-mapped. Switch your OS keyboard to US English before testing.

* **Sketch unstable / resets / garbled text:**

  * Make sure the board selection and drivers are correct. Keep the query length reasonable; the sketch uses a fixed-size stack buffer for query text.

* **No effect after plugging device in:**

  * Ensure you uploaded the correct sketch and that the Digispark board powers on. Some OSes need drivers or permission to use HID devices.

---

## Ethical & Legal Notes

Automated interactions with web services may violate their Terms of Service. Use this project only for learning, automated testing on your own machines, or where you have permission. Do not use this project for disruptive or harmful automation, spamming, or any activity that would violate laws or platform rules.

---

## Extending the project

* Add more queries to `PROGMEM`.
* Randomize between closing tabs or leaving some open to mimic different human behavior.
* Add configurable profiles (faster vs slower typing) or a physical switch on the device.
* Support different keyboard layouts by implementing a layout mapping table.

---

## Contributing

Feel free to open issues or PRs. If you add features, please keep the firmware small and ATtiny-friendly (avoid dynamic allocation and large global `String` usage).

---

## License

This repository is released under the **MIT License**. See the `LICENSE` file for details.

---

If you want, I can also create the `LICENSE` file (MIT) and the `docs/usage_guide.md` with step-by-step Digispark setup and debugging tips. Which one would you like next?
