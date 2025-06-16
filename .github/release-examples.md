# 📖 Exemples d'utilisation du workflow de release

Ce document présente des exemples concrets d'utilisation du workflow de release pour différents scénarios.

## 🎯 Scénarios de release

### Scénario 1: Première release du projet

**Situation** : Le projet Zappy est terminé et prêt pour sa première release publique.

**Action** :
- Type de version : `nouvelle-version`
- Pré-release : Non

**Résultat** :
- Version générée : `v1.0.0` (si aucun tag n'existe)
- Changelog automatique avec description complète du projet
- Archive complète avec tous les composants

### Scénario 2: Ajout d'une nouvelle fonctionnalité

**Situation** : L'équipe a ajouté un mode spectateur dans la GUI et de nouvelles commandes pour l'IA.

**Action** :
- Type de version : `nouvelle-feature`
- Pré-release : Non

**Résultat** :
- Version générée : `v1.0.0` → `v1.1.0`
- Changelog avec liste des nouvelles fonctionnalités
- Compatibilité maintenue avec les versions précédentes

### Scénario 3: Correction de bugs critiques

**Situation** : Des bugs de crash ont été corrigés dans le serveur et la GUI.

**Action** :
- Type de version : `resolution-bug`
- Pré-release : Non

**Résultat** :
- Version générée : `v1.1.0` → `v1.1.1`
- Changelog focalisé sur les corrections
- Release rapide pour les utilisateurs affectés

### Scénario 4: Refonte majeure de l'architecture

**Situation** : L'équipe a réécrit le serveur avec une nouvelle architecture et changé l'API.

**Action** :
- Type de version : `nouvelle-version`
- Pré-release : Non

**Résultat** :
- Version générée : `v1.1.1` → `v2.0.0`
- Changelog avec avertissement sur les breaking changes
- Documentation des migrations nécessaires

### Scénario 5: Version de test avant release

**Situation** : L'équipe veut tester une nouvelle version avant la release officielle.

**Action** :
- Type de version : `nouvelle-feature`
- Pré-release : **Oui**

**Résultat** :
- Version générée : `v1.1.0` → `v1.2.0-pre`
- Marquée comme pré-release sur GitHub
- Tests par les early adopters

## 📋 Guide pas à pas

### Étapes détaillées pour créer une release

1. **Préparation**
   ```bash
   # Vérifier l'état du projet
   git status
   git pull origin main
   
   # Tester la compilation
   make clean && make all
   make tests_run
   ```

2. **Accès au workflow**
   - Aller sur GitHub
   - Cliquer sur l'onglet "Actions"
   - Trouver "🚀 Release Zappy"
   - Cliquer sur "Run workflow"

3. **Configuration**
   - **Branch** : Sélectionner `main` (ou la branche de release)
   - **Type de version** : Choisir selon le scénario
   - **Pré-release** : Cocher si nécessaire

4. **Suivi de l'exécution**
   - Observer les logs en temps réel
   - Vérifier chaque étape :
     - ✅ Permissions
     - ✅ Calcul de version
     - ✅ Compilation
     - ✅ Création d'archive
     - ✅ Publication

5. **Vérification post-release**
   - Vérifier la page des releases
   - Télécharger et tester l'archive
   - Valider le changelog

## 🔢 Exemples de calcul de version

| Version actuelle | Type de release | Nouvelle version | Utilisation |
|------------------|-----------------|------------------|-------------|
| `v0.0.0` | `nouvelle-version` | `v1.0.0` | Première release |
| `v1.0.0` | `nouvelle-feature` | `v1.1.0` | Nouvelle fonctionnalité |
| `v1.1.0` | `resolution-bug` | `v1.1.1` | Correction de bug |
| `v1.5.3` | `nouvelle-version` | `v2.0.0` | Breaking change |
| `v2.1.7` | `nouvelle-feature` | `v2.2.0` | Feature compatible |

## 📦 Contenu type d'une release

### Archive `zappy-v1.2.0.tar.gz`
```
📁 zappy-v1.2.0/
├── 🔧 zappy_server      # Serveur de jeu (Linux x64)
├── 🎨 zappy_gui         # Interface graphique
├── 🤖 zappy_ai          # Client IA Python
├── 📁 lib/              # Bibliothèques statiques
│   ├── libgui.a
│   ├── libserver.a
│   ├── libraylib_cpp.a
│   └── libraygui_cpp.a
├── 📁 plugins/          # Bibliothèques dynamiques
│   ├── libraygui.so
│   └── libraylibcpp.so
├── 📋 README.md         # Documentation
├── ⚙️ install.sh        # Script d'installation
└── 🏷️ VERSION          # Numéro de version
```

### Installation automatique
```bash
# Décompresser
tar -xzf zappy-v1.2.0.tar.gz
cd zappy-v1.2.0

# Installer
sudo ./install.sh

# Utiliser
zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 5 -f 100
zappy_gui -p 4242 -h localhost
zappy_ai -p 4242 -n team1
```

## 🕒 Timeline type d'une release

```
⏰ 00:00 - Déclenchement du workflow
├── 00:01 - Vérification des permissions ✅
├── 00:02 - Calcul de la nouvelle version ✅
├── 00:03 - Installation des dépendances (⏳ 10 min)
├── 00:13 - Compilation du projet (⏳ 5 min)
├── 00:18 - Vérification des binaires ✅
├── 00:19 - Création de l'archive ✅
├── 00:20 - Upload des artefacts ✅
├── 00:21 - Création du tag Git ✅
└── 00:22 - Publication de la release ✅ 🎉
```

## 🚨 Gestion des erreurs courantes

### Erreur: "Binaire non trouvé"
```
❌ Binaire zappy_server introuvable
```
**Solution** : Vérifier le Makefile du serveur, s'assurer que la compilation produit le bon nom de binaire.

### Erreur: "Tag déjà existant"
```
❌ Tag v1.2.0 already exists
```
**Solution** : Le tag existe déjà, choisir un type de version approprié ou supprimer le tag si nécessaire.

### Erreur: "Permissions insuffisantes"
```
❌ Resource not accessible by integration
```
**Solution** : Vérifier les permissions du token GitHub dans les settings du repository.

### Erreur: "Dépendance manquante"
```
❌ Package libraylib-dev not found
```
**Solution** : Ajouter la dépendance manquante dans la section d'installation du workflow.

## 📈 Bonnes pratiques

### Timing des releases
- **PATCH** : Immédiatement pour les hotfixes critiques
- **MINOR** : En fin de sprint/itération (hebdomadaire/bimensuel)
- **MAJOR** : En fin de milestone (mensuel/trimestriel)

### Communication
- Annoncer les releases majeures à l'avance
- Documenter les breaking changes
- Fournir des guides de migration si nécessaire

### Tests
- Toujours tester localement avant la release
- Utiliser les pré-releases pour les tests utilisateurs
- Maintenir une suite de tests automatisés

### Documentation
- Tenir à jour le README.md
- Documenter les nouvelles APIs
- Fournir des exemples d'utilisation

## 🎯 Métriques de succès

Une release réussie doit avoir :
- ✅ Compilation sans erreur
- ✅ Tous les binaires générés
- ✅ Tests unitaires passants
- ✅ Archive complète créée
- ✅ Tag Git créé correctement
- ✅ Release GitHub publiée
- ✅ Changelog informatif
- ✅ Téléchargement fonctionnel

---

*Ces exemples couvrent les cas d'usage les plus courants du workflow de release Zappy. Pour des situations spécifiques, consulter la documentation complète dans `docs/release-workflow.md`.*
