# Shoot'em UP - Spielanleitung für Windows

Willkommen bei Shoot'em UP! Hier sind die Schritte, um das Spiel auf einem Windows-System zu kompilieren und auszuführen:

## Voraussetzungen

Stelle sicher, dass du die folgenden Tools installiert hast:
- CMake
- Einen C++ Compiler wie Visual Studio mit C++ Unterstützung

## Anleitung

1. **CMake ausführen:**
   Öffne die Eingabeaufforderung und navigiere zum Hauptverzeichnis des Projekts. Führe dann folgenden Befehl aus, um das Projekt zu konfigurieren:
   ```shell
   cmake -S . -B build
   ```

2. **In das Build-Verzeichnis wechseln:**
   ```shell
   cd .\build
   ```

3. **Das Projekt kompilieren:**
   Führe den folgenden Befehl aus, um das Projekt im Release-Modus zu kompilieren:
   ```shell
   cmake --build . --config Release
   ```

4. **Spiel starten:**
   Navigiere in das Release-Verzeichnis und starte das Spiel:
   ```shell
   cd .\Release
   .\Game.exe
   ```

