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
- [x] Support de 4 controllers
- [x] Améliorer la minimap pour mieux distinguer les 4 joueurs (couleurs différentes?)
- [x] Ajuster la taille des caméras stats pour s'adapter à la grille 2x2
- [x] Changer nicknames des joueurs 3 et 4 via le menu
- [x] Sauver/Charger les nicknames des joueurs 3 et 4 dans le fichier de sauvegarde.
- [x] Si moins de 4 gamepads, les gamepads sont reservés aux joueurs 3 et 4. P1 et P2 utiliseront le clavier.

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

## 🔧 À faire - Armes & Collision

### src/weapons.c
- [ ] **Line 844**: `proceed_bullets()` - Accepter un tableau de 4 joueurs
  - Actuellement signature: `void* p1, void* p2`
  - Proposé: `player_t** players` ou `void** players`
  - Impact sur toutes les fonctions de collision

## 📋 Améliorations futures (optionnel)

- [ ] Ajouter mode Team (2v2) avec gestion d'équipes
- [ ] Adapter le mode Capture the Flag pour 4 joueurs (2v2)
- [ ] Ajouter des contrôles pour PLAYER_3 et PLAYER_4 dans les autres input modes (Wii, PSP, IOKit)
- [ ] Tester et optimiser les performances avec 4 joueurs actifs

## 📝 Notes
- Le code actuel fonctionne déjà avec 4 joueurs pour le gameplay de base (FFA)
- Les modes de jeu spéciaux (Tag, Capture Flag) restent 2 joueurs pour l'instant