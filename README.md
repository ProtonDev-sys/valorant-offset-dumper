# valorant-offset-dumper

## Signatures

| Name | Kind | Pattern |
| --- | --- | --- |
| `ProcessEvent` | `DirectRva` | `40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC 50 01 00 00` |
| `StaticFindObject` | `DirectRva` | `40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 58 FC FF FF 48 81 EC A8 04 00 00 0F 29 B4 24 90 04 00 00` |
| `StaticLoadObject` | `DirectRva` | `40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 D8 FD FF FF 48 81 EC 28 03 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 18 02 00 00` |
| `BoneMatrix` | `DirectRva` | `48 8B C4 55 53 56 57 41 54 41 56 41 57 48 8D A8 A8 FE FF FF 48 81 EC 20 02 00 00 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 98 44 0F 29 48 88 44 0F 29 90 78 FF FF FF 44 0F 29 98 68 FF FF FF 44 0F 29 A0 58 FF FF FF 44 0F 29 A8 48 FF FF FF` |
| `SetOutlineMode` | `DirectRva` | `48 89 5C 24 10 48 89 6C 24 18 57 48 83 EC 20 41 0F B6 D8` |
| `FMemoryMalloc` | `DirectRva` | `48 89 5C 24 08 57 48 83 EC 20 48 8B F9 8B DA 48 8B 0D ?? ?? ?? ?? 48 85 C9 75 ?? E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 8B 01 44 8B C3 48 8B D7 48 8B 40 28 48 8B 5C 24 30 48 83 C4 20 5F 48 FF 25 ?? ?? ?? ?? CC CC CC CC CC CC CC CC CC CC CC 48 85 C9` |
| `PlayFinisher` | `DirectRva` | `48 8B C4 48 89 58 10 48 89 70 18 55 57 41 56 48 8D A8 08 FF FF FF 48 81 EC E0 01 00 00 0F 29 70 D8 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 C0 00 00 00 48 8B F9` |
| `get_spread_values` | `DirectRva` | `48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 83 EC 70 0F 29 70 D8 0F 29 78 C8 0F 28 F3` |
| `get_spread_angles` | `DirectRva` | `40 53 48 83 EC 30 48 8B C1 48 8B DA` |
| `tovector_and_normalize` | `DirectRva` | `48 89 5C 24 08 57 48 83 EC 40 F2 0F 10 0D ?? ?? ?? ??` |
| `toangle_and_normalize` | `DirectRva` | `48 89 5C 24 08 57 48 83 EC 50 0F 29 74 24 40 48 8B DA 0F 29 7C 24 30` |
| `GetFiringLocationAndDirection` | `DirectRva` | `48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 40 48 8B 01 41 0F B6 F9` |
| `TriggerVeh` | `DirectRva` | `48 8B 41 08 C3 48 8D 05 ?? ?? ?? ?? C3 CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 C1 08` |
| `gworld` | `RipRelative32` | `48 8B 15 ?? ?? ?? ?? 66 66 66 0F 1F 84 00 00 00 00 00 48 63 C1 48 8D 0C 40 48 8D 04 CA 4C 39 08 74 ?? 8B 48 10 83 F9 FF 75 ?? EB ??` |
| `fname_pool` | `RipRelative32` | `48 8D 35 ?? ?? ?? ?? 74 05 48 8B C6 EB 0F 48 8B CE E8 85 A2 01 01` |

## Current Offsets

```text
[SUCCESS] -> ProcessEvent: 0x1B437A0
[SUCCESS] -> StaticFindObject: 0x1B6D070
[SUCCESS] -> StaticLoadObject: 0x1B70BA0
[SUCCESS] -> BoneMatrix: 0x40C1190
[SUCCESS] -> SetOutlineMode: 0x4055840
[SUCCESS] -> FMemoryMalloc: 0x1724140
[SUCCESS] -> PlayFinisher: 0x624EB20
[SUCCESS] -> get_spread_values: 0x6F28690
[SUCCESS] -> get_spread_angles: 0x62EE2C0
[SUCCESS] -> tovector_and_normalize: 0x18238B0
[SUCCESS] -> toangle_and_normalize: 0x181E0B0
[SUCCESS] -> GetFiringLocationAndDirection: 0x6AC3260
[SUCCESS] -> TriggerVeh: 0x175BA46
[SUCCESS] -> gworld: 0xC19A0C0
[FAILED]  -> fname_pool: pattern not found
```
