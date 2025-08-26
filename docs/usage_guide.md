# Usage Guide — Auto Bing Searcher (ATtiny85)

This guide walks you through preparing, uploading, testing, and troubleshooting the `auto_bing_searcher.ino` firmware for Digispark / ATtiny85 USB boards.

> **Reminder:** Use responsibly. This project is for learning and controlled testing only.

---

## Table of contents

1. Preparation (hardware & OS checks)
2. Installing Arduino + Digispark core
3. Opening, configuring, and uploading the sketch
4. Preparing your browser and OS for testing
5. Recommended configuration tweaks
6. How to test safely (manual dry-run)
7. Troubleshooting checklist
8. Advanced tips and common modifications
9. Frequently asked problems

---

## 1) Preparation (hardware & OS checks)

* **Hardware**: Digispark / ATtiny85 USB dev board (or equivalent Digispark clone). Make sure you have a USB port and a USB cable (some boards plug directly into USB A ports).
* **OS**: Windows, macOS, or Linux can work. On Windows you may need to accept a driver prompt during the first upload; on macOS and Linux the Digispark core usually works without additional drivers.
* **Keyboard layout**: Set your OS keyboard to **US English** temporarily. DigiKeyboard HID usage assumes the US layout; other layouts will produce incorrect characters.
* **Browser**: Microsoft Edge or another Chromium-based browser is recommended for consistent Ctrl+T / Ctrl+W behavior.

---

## 2) Installing Arduino + Digispark core

1. Install the **Arduino IDE** (latest stable release) from the official Arduino website.
2. Open Arduino IDE → **File → Preferences**. In the "Additional Boards Manager URLs" field add the Digistump boards URL (follow Digistump installation instructions). If you already have other URLs, separate them with commas.
3. Open **Tools → Board → Boards Manager** and search for "Digistump" or "Digistump AVR Boards". Install the Digistump core.
4. After installation, select **Tools → Board → Digispark (Default - 16.5MHz)**.

> Tip: If you have trouble, consult the Digistump website for platform-specific driver instructions.

---

## 3) Opening, configuring, and uploading the sketch

1. Open `firmware/auto_bing_searcher.ino` in Arduino IDE.
2. Inspect the top of the sketch and adjust any constants you want to change (see section 5 for recommended values).
3. Save the sketch.
4. Click **Upload**. Digispark works a little differently than standard Arduino boards: wait for the IDE to prompt you to plug the device in, then insert the Digispark into USB.
5. The IDE will flash the device and show progress. If upload fails, unplug and try again.

**Important**: Digispark boards power on immediately when plugged in. Make sure your browser and target application are in a safe state before plugging the device during tests.

---

## 4) Preparing your browser and OS for testing

1. Open Microsoft Edge and make sure it is the **frontmost window** (focused). The sketch assumes the browser will receive keyboard input.
2. Optional: open a blank tab so the new-tab shortcut (`Ctrl+T`) behaves predictably.
3. Set OS keyboard to **US English**.
4. If your OS locks or requires password on wake/login, either disable that temporarily or ensure the session is unlocked before plugging in the device.

---

## 5) Recommended configuration tweaks

At the top of `auto_bing_searcher.ino` you will find configuration `#define` values. Example recommendations:

```cpp
#define MAX_SEARCHES 25        // number of searches
#define PAGE_WAIT_MIN 3000     // ms — smaller = faster
#define PAGE_WAIT_MAX 5000
#define TYPE_DELAY_MIN 80
#define TYPE_DELAY_MAX 160
#define TYPE_JITTER 40
#define MICRO_PAUSE_CHANCE 6
#define MICRO_PAUSE_MS 200
#define TYPO_CHANCE 12
```

* **If the device types too fast**: increase `TYPE_DELAY_MIN/TYPE_DELAY_MAX` or `MICRO_PAUSE_MS`.
* **If pages haven't loaded before Ctrl+W**: increase `PAGE_WAIT_MIN/MAX`.
* **To reduce typos**: increase `TYPO_CHANCE` (rarer typos) or set it very high to disable.

---

## 6) How to test safely (manual dry-run)

Before running full automation, test with 1–3 searches:

1. Set `MAX_SEARCHES` to `2` or `3` in the sketch and upload.
2. Open Edge and make sure a safe page is visible.
3. Plug in Digispark and watch the behavior.
4. Verify the device opens new tabs, types the query, waits, and closes tabs.
5. If behavior is correct, increase `MAX_SEARCHES` back to desired value.

---

## 7) Troubleshooting checklist

If something goes wrong, go through these steps:

* **No typing at all**: Verify the board was uploaded successfully and that the Digispark powers on (LED may flash). Try replugging.
* **Wrong characters typed**: Switch your OS keyboard layout to **US English**.
* **Backspace navigates back**: This happens when the browser content area has focus. Ensure the script can focus the omnibox; on some systems adding a short delay after opening a new tab helps. Increase `TYPE_DELAY_MIN` or `PAGE_WAIT_MIN` to allow the browser to settle.
* **Upload failing on Windows**: Reinstall Digistump drivers or consult Digistump docs. Sometimes replugging the board helps.
* **Device types too fast/slow**: Adjust `TYPE_DELAY_*` and `PAGE_WAIT_*` values.

If you still can’t fix it, capture a short video of the behavior — it helps diagnose focus and timing issues.

---

## 8) Advanced tips and common modifications

* **Leave some tabs open**: To mimic humans leaving tabs open sometimes, randomly skip `Ctrl+W` for some iterations.
* **Randomized slow sessions**: Add an occasional long pause (30–90 seconds) between searches to emulate reading.
* **Profiles**: Add `#define PROFILE_SLOW` or `PROFILE_FAST` and switch timing values conditionally to change typing behavior quickly.
* **Locale mapping**: Implement a small keyboard mapping table to support non-US layouts (complex but possible).
* **Visual debugging**: If your Digispark clone has an LED, toggle it in code at key points (e.g., before typing) to observe flow without relying on the host machine.

---

## 9) Frequently asked problems

**Q: Why does Backspace sometimes go back a page?**
A: Backspace only deletes text when the omnibox or a text field is focused. When the page body is focused it becomes a browser navigation key. Use `Ctrl+T` then `Ctrl+L` (or `Alt+D`) as a fallback to focus the address bar, or increase the delay between opening a new tab and typing.

**Q: Why are some characters wrong (e.g., @, ")?**
A: Non-US layouts change key positions. Set your OS layout to US English for the test or implement a mapping layer in firmware.

**Q: Upload fails with a driver error on Windows 10/11**
A: Try re-installing the Digistump drivers or follow Digistump troubleshooting docs. Rebooting after driver installation may be required.

---

## Final notes

* Keep the queries reasonable and short. Large query strings can exceed buffer sizes if you change the firmware without updating buffer lengths.
* Always test with a small `MAX_SEARCHES` first.
* Respect website terms of service and laws: this project is educational.

---
