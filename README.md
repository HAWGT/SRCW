# 🔓 SonicUnlocker

> Unlocks everything in **Sonic Racing CrossWorlds** while removing red exclamation flags.

If you want to change which features will be unlocked, change the values of SRCWConfig in SRCW/Features.h before compiling it yourself. Then follow the steps in 'Usage' from 2.
- True : unlocks the feature.
- False : leaves it as it is.

---

## ⚠️ Disclaimer

- Do **not** use not owned (dlc for example) or unreleased characters/tracks/gadgets/titles/etc. in online or time attack mode, as it may result in restrictions or bans.
- This project is intended for **educational and research purposes only**
- Use at your own risk (strongly advised to make a backup of your save file before using)
- Not affiliated with SEGA or Sonic Team

---

## 🌭🦔💨🌀 Usage

0. *(optional)* Backup your game files (code folder + crashpad_db). Path : %APPDATA%\SEGA\SonicRacingCrossWorlds\steam

1. Download the latest release from the **Releases** tab  
2. Place `umpdc.dll` into `SonicRacingCrossWorlds\UNION\Binaries\Win64`
3. Launch the game  
4. Wait 30 seconds just to make sure the program runs fully (there is 22 steps and 750ms delay between each step). Everything should now be unlocked.

5. *(optional)* Then you can remove `umpdc.dll` from `SonicRacingCrossWorlds\UNION\Binaries\Win64`. Everything will still be unlocked, except DLC characters, tracks, etc. if you haven't [bought them](https://store.steampowered.com/dlc/2486820). 

---

## 🛠️ Building

> Requires Visual Studio (C++)

1. Clone the repository:

git clone https://github.com/HAWGT/SRCW.git

(or git clone https://github.com/GathoChocolat/SRCWUnlocker.git)

3. Open the solution in Visual Studio  
4. Build in **Release x64**
5. Output DLL will be located in:

/x64/Release/umpdc.dll

---

## 🧠 How It Works

- Hooks into game memory at runtime  
- Overrides progression checks  
- Injects unlock states directly  
- Uses a timer system to prevent excessive writes  

---

## 🤝 Contributing

Pull requests are welcome.

If you're adding features:
- Keep modules isolated  
- Follow existing structure  
- Document new functionality  

---

## ⭐ Support

If you find this useful:
- Star the repo ⭐  
- Share it with others  

---

## 📜 License

MIT License (or your preferred license)

---

## 👤 Authors

#### **Glitxhhh**
---
#### **GathoChocolat**
---
#### **HAWGT**
---
