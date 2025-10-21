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
- [x] `proceed_bullets()` Refactoriser pour 4 joueurs
- [x] Deathmatch pour 4 joueurs
- [x] Game of Tag pour 4 joueurs FFA
- [x] Game of Tag pour 4 joueurs 2 vs 2
- [x] Game of Tag pour 4 joueurs FFA
- [x] Capture the Flag pour 4 joueurs (2v2)


## 📋 Améliorations futures (optionnel)
- [ ] Ajouter des contrôles pour PLAYER_3 et PLAYER_4 dans les autres input modes (Wii, PSP, IOKit)
- [ ] Tester et optimiser les performances avec 4 joueurs actifs
