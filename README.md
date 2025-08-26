# 🔍 Auto Bing Searcher (ATtiny85 + DigiKeyboard)

This project uses an **ATtiny85 USB board** with the **DigiKeyboard** library to automate Bing/Edge searches.  
It types queries into the browser with **human-like typing patterns** (typos, random delays, micro-pauses),  
opens and closes tabs automatically, and stops after a configurable limit.

---

## ✨ Features
- ✅ Runs **25 random Bing searches** (limit adjustable).
- ✅ **New tab per search**, then **auto-close** (`Ctrl+W`).
- ✅ Mimics **human typing**: variable speeds, typos + corrections, micro-pauses.
- ✅ Final **Alt+F4** closes the browser after all searches.
- ✅ Lightweight: fits on **ATtiny85 Digispark USB boards**.

---

## 📂 Repository Layout
- `firmware/auto_bing_searcher.ino` → Arduino sketch to flash onto ATtiny85.
- `docs/usage_guide.md` → Extended setup instructions.
- `LICENSE` → Open source license (default MIT).
- `README.md` → Overview and quickstart.

---

## 🛠️ Requirements
- **Digispark ATtiny85 USB board**
- **Arduino IDE** with [Digispark drivers](https://digistump.com/wiki/digispark/tutorials/connecting)
- `DigiKeyboard.h` library (comes with Digispark core)

---

## 🚀 Setup & Flashing
1. Clone this repo:
   ```bash
   git clone https://github.com/YOUR-USERNAME/auto-bing-searcher.git
   cd auto-bing-searcher/firmware
