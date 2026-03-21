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
ProcessEvent: 0x1AF6470
StaticFindObject: 0x1B1F7C0
StaticLoadObject: 0x1B232F0
BoneMatrix: 0x4071D70
SetOutlineMode: 0x40064F0
FMemoryMalloc: 0x16D7BC0
PlayFinisher: 0x61DAE00
get_spread_values: 0x6E95E00
get_spread_angles: 0x627B290
tovector_and_normalize: 0x17D7350
toangle_and_normalize: 0x17D1B50
GetFiringLocationAndDirection: 0x6A41070
TriggerVeh: 0x170F4B6
gworld: 0xC0CC120
fname_pool: 0xC25FB80
uworld_pointer: 0x80
game_instance: 0x1D8
gamestate: 0x178
persistent_level: 0x38
localplayers_array: 0x40
localplayer: 0x40
player_controller: 0x38
player_camera_manager: 0x520
camera_private: 0x17B0
camera_manager: 0x540
control_rotation: 0x4E0
outline_mode: 0x358
outline_component: 0x0D80
attach_children: 0x1A0
attach_children_count: 0x1A8
acknowledge_pawn: 0x510
apawn: 0x510
player_state: 0x480
player_state_array: 0x478
spawned_character: 0x0B48
root_component: 0x288
root_position: 0x170
my_hud: 0x518
damage_handler: 0x0C68
health: 0x200
cached_life: 0x200
shieldtype: 0x118
shieldlife: 0x124
shieldmaxlife: 0x128
damagesections: 0x1E8
inventory: 0x09A8
current_equippable: 0x248
magazine_ammo: 0x11D8
max_ammo: 0x130
auth_resource_amount: 0x110
time_remaining_to_explode: 0x05A8
planted_at_site: 0x05B8
current_defuser: 0x05C0
defuse_progress: 0x05D0
current_defuse_section: 0x05F0
mesh: 0x04E8
component_to_world: 0x2D0
BoundsScale: 0x47C
bone_array: 0x738
bone_array_cache: 0x748
bone_count: 0x740
LocalBounds: 0x730
last_submit_time: 0x480
last_render_time: 0x484
relative_location: 0x170
relative_rotation: 0x188
team_component: 0x690
team_id: 0xF8
was_ally: 0x0F58
CharacterPortraitMinimapComponent: 0x1210
ShooterCharacterMinimapComponent: 0x1218
VisibilityComponent: 0x538
CharactersWithVisibility: 0x118
bIsVisible: 0x521
bLocalObserver: 0x550
dormant: 0xFC
actor_array: 0xA0
actors_count: 0xA8
actor_id: 0x18
unique_id: 0x38
outline_ally_color: 0x9044340
outline_enemy_color: 0x9045C20
```
