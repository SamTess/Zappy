# 🚀 CI/CD Release Workflow pour Zappy

Ce document explique comment utiliser le workflow GitHub Actions pour créer des releases automatiques du projet Zappy.

## 📋 Vue d'ensemble

Le workflow de release permet de :
- Générer automatiquement des versions sémantiques
- Compiler tous les composants du projet
- Créer des packages de release
- Publier sur GitHub Releases
- Générer un changelog automatique

## 🎯 Types de versioning

Le système suit le **versioning sémantique** (SemVer) avec le format `v{MAJOR}.{MINOR}.{PATCH}` :

### 🆕 Nouvelle version (MAJOR)
- **Option** : `nouvelle-version`
- **Effet** : `v1.2.3` → `v2.0.0`
- **Usage** : Changements majeurs, breaking changes
- **Exemple** : Refonte complète de l'architecture, changements d'API incompatibles

### ✨ Nouvelle feature (MINOR)
- **Option** : `nouvelle-feature`
- **Effet** : `v1.2.3` → `v1.3.0`
- **Usage** : Nouvelles fonctionnalités compatibles
- **Exemple** : Nouveau mode de jeu, nouvelles commandes IA, améliorations GUI

### 🐛 Résolution de bug (PATCH)
- **Option** : `resolution-bug`
- **Effet** : `v1.2.3` → `v1.2.4`
- **Usage** : Corrections de bugs, améliorations mineures
- **Exemple** : Correction de crashes, optimisations de performance

## 🚀 Comment créer une release

### 1. Accéder au workflow
1. Aller sur GitHub → onglet **Actions**
2. Sélectionner le workflow **🚀 Release Zappy**
3. Cliquer sur **Run workflow**

### 2. Configurer la release
- **Branch** : Sélectionner la branche (généralement `main`)
- **Type de version** : Choisir parmi :
  - `nouvelle-version` (MAJOR)
  - `nouvelle-feature` (MINOR)
  - `resolution-bug` (PATCH)
- **Pré-release** : Cocher si c'est une version de test

### 3. Déclencher la release
Cliquer sur **Run workflow** pour lancer le processus.

## 📦 Contenu des releases

Chaque release contient :

### 🗂️ Archive `zappy-vX.Y.Z.tar.gz`
```
zappy-vX.Y.Z/
├── zappy_server          # Serveur de jeu (C)
├── zappy_gui            # Interface graphique (C++)
├── zappy_ai             # Client IA (Python)
├── lib/                 # Bibliothèques statiques
│   ├── libgui.a
│   ├── libserver.a
│   ├── libraylib_cpp.a
│   └── libraygui_cpp.a
├── plugins/             # Bibliothèques dynamiques
│   ├── libraygui.so
│   └── libraylibcpp.so
├── README.md           # Documentation
├── install.sh          # Script d'installation
└── VERSION             # Numéro de version
```

### 📋 Script d'installation
```bash
# Rendre le script exécutable
chmod +x install.sh

# Installer Zappy sur le système
sudo ./install.sh
```

## 🔧 Fonctionnement technique

### 1. 🔐 Vérification des permissions
- Contrôle des droits de création de release
- Validation de l'utilisateur et du repository

### 2. 📊 Calcul de version
- Récupération du dernier tag sémantique
- Calcul automatique de la nouvelle version
- Gestion du cas de première release (`v0.0.0` → `v1.0.0`)

### 3. 🏗️ Compilation
- Installation des dépendances système
- Compilation de tous les composants :
  - Serveur C avec Make
  - GUI C++ avec raylib
  - Client IA Python
- Vérification de l'intégrité des binaires

### 4. 📦 Package
- Création d'une archive complète
- Inclusion de tous les binaires et bibliothèques
- Génération du script d'installation
- Création du changelog automatique

### 5. 🚀 Publication
- Création du tag Git
- Publication sur GitHub Releases
- Upload de l'archive
- Génération des notes de release

## 📝 Changelog automatique

Le changelog est généré automatiquement et contient :
- Type de release et description
- Liste des commits depuis la dernière version
- Contenu de la release
- Instructions d'installation

### Exemple de changelog
```markdown
## 🎮 Zappy v1.2.0

### 📅 Date de release: 12/06/2025 à 14:30

### ✨ Nouvelle fonctionnalité
Cette version ajoute de nouvelles fonctionnalités tout en maintenant la compatibilité.

### 📋 Changements
- Ajout du mode spectateur dans la GUI
- Amélioration de l'IA avec de nouveaux algorithmes
- Optimisation des performances réseau
- Correction de bugs mineurs

### 📦 Contenu de la release
- **Binaires**: zappy_server, zappy_gui, zappy_ai
- **Bibliothèques**: libgui.a, libserver.a, libraylib_cpp.a, libraygui_cpp.a
- **Plugins**: libraygui.so, libraylibcpp.so
- **Documentation**: Guide d'utilisation et API
```

## ⚙️ Configuration avancée

### Variables d'environnement
Le workflow utilise plusieurs variables configurables :

```yaml
env:
  BINARIES: "zappy_gui zappy_server zappy_ai"
  LIBRARIES: "libgui.a libserver.a libraylib_cpp.a libraygui_cpp.a"
  DYNAMIC_LIBS: "libraygui.so libraylibcpp.so"
```

### Timeouts
- Build total : 20 minutes
- Installation dépendances : 10 minutes
- Compilation : 15 minutes

### Artefacts
- Rétention : 90 jours
- Format : tar.gz compressé
- Upload automatique sur GitHub

## 🐛 Dépannage

### Erreurs communes

#### ❌ Binaire non trouvé
```
❌ Binaire zappy_server introuvable
```
**Solution** : Vérifier que la compilation s'est bien déroulée dans le Makefile

#### ❌ Bibliothèque manquante
```
⚠️ Bibliothèque statique libgui.a introuvable
```
**Solution** : S'assurer que `make all` compile toutes les bibliothèques

#### ❌ Permissions insuffisantes
```
Error: Resource not accessible by integration
```
**Solution** : Vérifier les permissions du token GitHub dans les settings du repo

### Logs de débogage
Chaque étape du workflow produit des logs détaillés :
- 🔍 Vérification des permissions
- 📊 Calcul de version avec détails
- 🏗️ Logs de compilation complets
- ✅ Vérification des binaires et bibliothèques
- 📦 Contenu de l'archive de release

## 🎯 Bonnes pratiques

### Avant de créer une release
1. **Tester localement** : `make all && make tests_run`
2. **Vérifier les changements** : Revoir les commits depuis la dernière version
3. **Choisir le bon type** : Évaluer l'impact des changements
4. **Documenter** : Mettre à jour README.md si nécessaire

### Gestion des versions
- **PATCH** : Pour les hotfixes urgents
- **MINOR** : Pour les sprints/itérations normales  
- **MAJOR** : Pour les milestones importantes

### Timing
- Éviter les releases en fin de semaine
- Préférer les heures ouvrées pour le support
- Tester les pré-releases avant les releases finales

## 📞 Support

En cas de problème avec le workflow de release :
1. Consulter les logs dans l'onglet Actions
2. Vérifier la configuration dans `.github/workflows/release.yml`
3. Contacter l'équipe de développement

---

*Ce workflow a été conçu pour automatiser et standardiser le processus de release du projet Zappy tout en maintenant la qualité et la traçabilité.*
