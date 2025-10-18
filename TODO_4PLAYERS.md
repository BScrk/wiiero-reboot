# TODO Liste pour le Mode 4 Joueurs - Wiiero

## ✅ Déjà fait
- [x] Modification de `player_id` enum pour 4 joueurs (PLAYER_1, PLAYER_2, PLAYER_3, PLAYER_4)
- [x] Configuration des 4 caméras en grille 2x2 dans `game.h` et `game.c`
- [x] Initialisation des 4 joueurs dans `wiiero_load()`
- [x] Update de `wiiero_update_world()` pour les 4 joueurs
- [x] Update de `wiiero_blit_world()` pour afficher les 4 caméras
- [x] Update de `wiiero_restart_game()` avec boucle générique pour 4 joueurs
- [x] Extension de `game_nicknames` à 4 entrées (P1, P2, P3, P4)
- [x] Refactorisation de `blit_bullets()` pour accepter un tableau de caméras
- [x] Refactorisation de `blit_dynamics_objs()` pour accepter un tableau de caméras
- [x] Update de `update_minimap()` pour afficher les 4 joueurs
- [x] Fonction helper `get_player_camera_id()` pour mapper joueur -> caméra
- [x] Update de `player_is_aiming()` pour mode FFA
- [x] Update de `wiiero_set_round_stats()` pour afficher stats des 4 joueurs

## 🔨 À faire - Gameplay Core

### src/game.c
- [ ] **Line 272**: `proceed_bullets()` - Refactoriser pour accepter un tableau de tous les 4 joueurs au lieu de juste 2
  - Actuellement passe seulement PLAYER_1 et PLAYER_2
  - Besoin de gérer collisions pour tous les joueurs

- [ ] **Line 1248**: `wiiero_round_stats()` - Vérifier les actions des 4 joueurs, pas seulement PLAYER_1 et PLAYER_2
  - Actuellement vérifie seulement 2 joueurs pour sortir des stats

- [ ] **Line 402, 474, 497**: Modes de jeu - Refactoriser pour plus de 2 joueurs
  - `wiiero_got_game_mode()` (Game of Tag)
  - `wiiero_deathm_game_mode()` (Deathmatch)
  - `wiiero_cflag_game_mode()` (Capture the Flag)
  - ⚠️ Pour l'instant, ces modes restent 2 joueurs seulement

## 🎮 À faire - Contrôles & Input

### src/input_sdl.h
- [ ] **Line 91**: `MAX_GAMEPADS` - Augmenter à 4 pour supporter 4 manettes
  - Actuellement limité à 2 manettes
  - Modifier aussi le tableau `gamepads[]`

- [ ] **Line 243**: Ajouter les contrôles clavier pour PLAYER_3 et PLAYER_4
  - Définir de nouvelles touches (P3_ACTION_KEY_*, P4_ACTION_KEY_*)
  - Ajouter la gestion dans `player_event_update()`

## 🎨 À faire - UI & Menus

### src/game.c
- [ ] **Line 734**: Ajouter callbacks `option_change_p3_name_cb()` et `option_change_p4_name_cb()`
  - Pour permettre le changement de noms des joueurs 3 et 4

- [ ] **Line 989**: Ajouter entrées de menu pour les noms PLAYER_3 et PLAYER_4
  - Nécessite d'update l'enum `OPTION_MAX` dans `menu.h`

### src/menu.h
- [ ] **Line 69**: Ajouter `OPTION_P3_NAME` et `OPTION_P4_NAME` dans l'enum
  - Mettre à jour `OPTION_MAX` en conséquence

## 💾 À faire - Configuration

### src/game.c
- [ ] **Line 1279**: `wiiero_load_config()` - Charger les nicknames de PLAYER_3 et PLAYER_4 depuis le fichier config
  - Format fichier config à étendre

- [ ] **Line 1322**: `wiiero_save_config()` - Sauvegarder les nicknames de PLAYER_3 et PLAYER_4
  - Synchroniser avec le load

## 🔧 À faire - Armes & Collision

### src/weapons.c
- [ ] **Line 844**: `proceed_bullets()` - Accepter un tableau de 4 joueurs
  - Actuellement signature: `void* p1, void* p2`
  - Proposé: `player_t** players` ou `void** players`
  - Impact sur toutes les fonctions de collision

## 📋 Améliorations futures (optionnel)

- [ ] Ajouter mode Team (2v2) avec gestion d'équipes
- [ ] Adapter le mode Capture the Flag pour 4 joueurs (4 drapeaux ou 2v2)
- [ ] Améliorer la minimap pour mieux distinguer les 4 joueurs (couleurs différentes?)
- [ ] Ajouter des contrôles pour PLAYER_3 et PLAYER_4 dans les autres input modes (Wii, PSP, IOKit)
- [ ] Tester et optimiser les performances avec 4 joueurs actifs
- [ ] Ajuster la taille des caméras stats pour s'adapter à la grille 2x2

## 📝 Notes
- Le code actuel fonctionne déjà avec 4 joueurs pour le gameplay de base (FFA)
- Les modes de jeu spéciaux (Tag, Capture Flag) restent 2 joueurs pour l'instant
- Seuls 2 joueurs peuvent utiliser des manettes actuellement
- Les menus/options affichent toujours sur les caméras PLAYER_1 et PLAYER_2 uniquement
