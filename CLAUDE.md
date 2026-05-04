# AmanecerRojo — Unreal Engine 5.7 Game Studio Configuration

Juego 2.5D de acción narrativa basado en la trilogía *Red Rising* de Pierce Brown.
Motor: Unreal Engine 5.7 | RTX 3080 | Estilo visual Octopath Traveler.

## Technology Stack

- **Engine**: Unreal Engine 5.7
- **Language**: C++ (core systems) + Blueprint (content, prototyping)
- **2D Characters**: PaperZD — sprites billboard en entorno 3D fotorrealista
- **Version Control**: Git → github.com/fcodearts/AmanecerRojoUE
- **MCP**: Remote Control API en 127.0.0.1:30010 (Tools/mcp/server.py)

## Engine Reference

@docs/engine-reference/unreal/VERSION.md

## Current Best Practices

@docs/engine-reference/unreal/current-best-practices.md

## Visual Concept

El contraste estético es el núcleo del juego:
- **Entorno** (El Instituto de Marte): Fotorrealista. Quixel Megascans + Nanite + Lumen.
- **Personajes** (Darrow, Sevro, El Chacal): Sprites 2D artísticos en Billboard Mode via PaperZD.

## Project Structure

```
Content/
├── Characters/        # PaperZD sprites, flipbooks, animaciones
│   └── Darrow/
├── Environment/       # Quixel assets, niveles, meshes del Instituto
│   └── Marte/
├── UI/                # Widgets UMG (HUD, diálogos, menús)
├── Data/              # DataTables, DataAssets, JSON de diálogos
│   └── Dialogos/
├── VFX/               # Niagara systems (Aura Dorada, impactos)
├── Audio/             # MetaSound sources, audio assets
└── Blueprints/        # Blueprints de gameplay
Source/
└── AmanecerRojo/      # C++ — sistemas core
    ├── Core/          # GameInstance, GameMode, PlayerController
    ├── Player/        # ACharacter del jugador + input
    ├── Combat/        # UCombatComponent
    ├── Dialogue/      # UDialogueSubsystem (parser JSON)
    └── Save/          # USaveGame subclass
Tools/
├── mcp/server.py      # Servidor MCP → Remote Control API
└── *.py               # Scripts de utilidad Python para el editor
```

## Rendering Configuration (RTX 3080)

- Global Illumination: **Lumen**
- Reflections: **Lumen**
- Shadows: **Virtual Shadow Maps**
- Anti-Aliasing: **TSR**
- Hardware Ray Tracing: **ON**

## Plugins Activos

| Plugin | Propósito |
|---|---|
| PaperZD | Sprites 2D animados para personajes |
| Paper2D | Base de sprites (requerido por PaperZD) |
| Niagara | Partículas — Aura Dorada de Darrow |
| RemoteControl | MCP bridge al editor |
| RemoteControlWebInterface | HTTP server en puerto 30010 |
| AIAssistant | Asistente IA en editor |

## Systems Map (Godot → UE5)

| Sistema original (Godot) | Equivalente UE5 | Archivo |
|---|---|---|
| `GlobalStats` (Autoload) | `UGameInstance` subclass | `Source/AmanecerRojo/Core/` |
| `Player` (CharacterBody3D) | `ACharacter` + PaperZD | `Source/AmanecerRojo/Player/` |
| `IsometricCamera` | `BP_IsometricCamera` (Pawn + SpringArm) | `Content/Blueprints/` |
| `DialogueManager` (Autoload) | `UGameInstanceSubsystem` | `Source/AmanecerRojo/Dialogue/` |
| `CombatSystem` (componente) | `UCombatComponent` | `Source/AmanecerRojo/Combat/` |
| `Enemy` (CharacterBody3D) | `ACharacter` + BehaviorTree | `Source/AmanecerRojo/Enemy/` |
| `HUD` | Widget Blueprint UMG | `Content/UI/WBP_HUD` |
| `DialogueUI` | Widget Blueprint UMG | `Content/UI/WBP_DialogueUI` |
| `SaveManager` | `USaveGame` subclass | `Source/AmanecerRojo/Save/` |
| `AudioManager` | MetaSound + `UAudioComponent` | `Source/AmanecerRojo/Audio/` |

## Narrative Data (JSON reused from Godot)

Los archivos JSON de Godot se reutilizan directamente:
- `Content/Data/Dialogos/capitulo_1_instituto.json`
- `Content/Data/Dialogos/capitulo_2_aulladores.json`
- `Content/Data/Dialogos/capitulo_3_mustang.json`
- `Content/Data/Enemigos/enemigos.json`

## Key Stats (GameInstance)

```cpp
FuerzaRojo, TecnicaDorado, Prestigio, ConflictoInterno, HpActual, ActoActual
```

## Agents Activos

Unreal: `unreal-specialist`, `ue-blueprint-specialist`, `ue-gas-specialist`, `ue-umg-specialist`, `ue-replication-specialist`
Arte: `technical-artist`, `art-director`
Audio: `audio-director`, `sound-designer`
Narrativa: `narrative-director`, `writer`
Diseño: `game-designer`, `systems-designer`, `gameplay-programmer`
QA: `qa-lead`, `qa-tester`
Producción: `producer`, `lead-programmer`, `technical-director`

## Coding Standards

- **Español** en todo: variables de gameplay, nombres de flipbooks, flags de diálogo
- Nombres C++ en inglés siguiendo convenciones UE5 (`A`, `U`, `F`, `E`, `I` prefijos)
- `UPROPERTY()` obligatorio en todas las referencias UObject
- `TObjectPtr<>` en lugar de raw pointers
- No Tick donde se puede usar eventos o timers
- GAS para todo sistema de habilidades/combate

## MCP Usage

El servidor MCP (Tools/mcp/server.py) expone el editor UE5 via Remote Control API:
```bash
# Verificar conexión
curl http://127.0.0.1:30010/remote/info

# El servidor MCP se configura en .claude/settings.json
# Claude Code lo usa automáticamente al abrir el proyecto
```

## Convenciones Narrativas

- Protagonista: Darrow (Rojo → Dorado de Hierro)
- Conflicto central: venganza vs. justicia
- Stats clave: `FuerzaRojo`, `TecnicaDorado`, `Prestigio`, `ConflictoInterno`, `ActoActual`
- Todo texto visible al jugador en **español**
