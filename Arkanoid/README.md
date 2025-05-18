# Arkanoid Game

This is a C++ implementation of the classic Arkanoid game using SFML.

---

## 🕹️ Features
- Destructible and indestructible blocks, multi-HP blocks, speed-up blocks
- Various block colors and bonus blocks: ExpandPaddle, CompressPaddle, SlowBall, FastBall, StickyPaddle, OneTimeFloor, RandomBounce
- Sprite-based bonuses, sticky paddle, random bounce
- One-time floor bonus, UI screens (Start, End)
- Score and lives display (text and heart sprites)
- Clean OOP architecture with layers: Game, Board, Block, Ball, Paddle, Bonus, ResourceHolder

---

## 🛠️ Requirements
- Windows, Visual Studio 2022 (17.7+), CMake 3.15+
- [vcpkg](https://github.com/microsoft/vcpkg) with SFML and TGUI installed
- SFML, TGUI via vcpkg
- C++20

---

## 📦 Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/<your-username>/arkanoid.git
   cd arkanoid
   ```
2. Bootstrap and integrate vcpkg (if not already):
   ```bash
   git submodule update --init --recursive
   ```
3. Configure and build with CMake (in x64 Release):
   ```bash
   cmake --preset x64-release
   cmake --build --preset x64-release
   ```
4. Run the executable:
   ```bash
   out/build/x64-release/Arkanoid.exe
   ```

---

## 🎮 Game Controls
- **Move Paddle**: Left mouse button + drag horizontally
- **Launch Ball**: Release left mouse button from Start state or after death
- **Retry**: Click **RETRY** on Game Over screen

---

## 📂 Project Structure
```
arkanoid/                # Root directory
├── CMakeLists.txt       # Main CMake build script
├── CMakePresets.json    # CMake configuration presets
├── README.md            # Project overview and instructions
├── LICENSE              # Project license (MIT)
├── .gitignore           # Files and folders to ignore in Git
├── maps/                # Level definitions (text files)
├── assets/              # Game assets
│   ├── fonts/           # Font files (e.g. .ttf)
│   ├── textures/        # General textures
│   └── sprites/         # Sprites (e.g. bonuses, hearts)
├── src/                 # Source code
│   ├── Game.hpp         # Main game class
│   ├── Game.cpp
│   ├── CollisionUtils.hpp  # Collision helper functions
│   ├── Pos.hpp          # Logical position struct
│   ├── ResourceHolder.hpp  # Asset manager
│   ├── entities/        # Core game entities
│   │   ├── Block.hpp
│   │   ├── Block.cpp
│   │   ├── Ball.hpp
│   │   ├── Ball.cpp
│   │   ├── Paddle.hpp
│   │   ├── Paddle.cpp
│   │   └── bonuses/
│   │       ├── Bonus.hpp
│   │       ├── Bonus.cpp
│   └── UI/              # User interface classes
│       ├── StartScreen.hpp
│       ├── StartScreen.cpp
│       ├── EndScreen.hpp
│       ├── EndScreen.cpp
│       ├── Button.hpp
│       └── Button.cpp
└── out/                 # Build output (ignored by Git)
    ├── build/
    └── install/
```

---

## 🤝 Contributing

1. Fork this repo  
2. Create a feature branch:
   ```bash
   git checkout -b feature/YourFeature
   ```
3. Commit your changes:
   ```bash
   git add .
   git commit -m "Add awesome feature"
   git push origin feature/YourFeature
   ```
4. Open a Pull Request and describe your work.

---

Enjoy playing!

