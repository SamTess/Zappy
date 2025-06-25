# Network System Wrapper - Architecture & Design Patterns

## 📋 Vue d'ensemble

Ce module implémente une encapsulation complète des fonctions réseau système (sockets, polling, adresses) en utilisant des **Design Patterns** avancés pour offrir une API propre, testable et maintenable.

## 🏗️ Architecture

```
systemWrapper/
├── interfaces/          # Contrats abstraits (ISocket, IBuffer, etc.)
├── implementation/      # Implémentations concrètes
├── factory/            # Pattern Factory pour la création d'objets
└── NetworkAPI.hpp      # Point d'entrée unifié
```

## 🤝 Utilisation des `friend` : Justification Technique

### Pourquoi utiliser `friend` ?

Les classes `friend` sont utilisées dans ce module pour permettre un **accès contrôlé** aux détails d'implémentation sans compromettre l'encapsulation. Voici les cas spécifiques :

### 1. NetworkAddress ↔ TcpSocket

```cpp
class NetworkAddress : public INetworkAddress {
private:
    sockaddr_in _addr;              // Structure système C
    friend class TcpSocket;         // Accès contrôlé
    
    const sockaddr* getSockAddr() const;    // Méthode système privée
    socklen_t getAddrSize() const;          // Méthode système privée
};
```

**Problème résolu :**
- `TcpSocket::performConnect()` a besoin des structures système `sockaddr*` et `socklen_t`
- Ces détails ne doivent **jamais** être exposés à l'utilisateur final
- `friend` permet l'accès interne tout en gardant l'interface publique clean

**Sans `friend` (❌ Mauvaise approche) :**
```cpp
// Option 1: Exposer les détails système (MAUVAIS)
class NetworkAddress {
public:
    const sockaddr* getSockAddr() const;  // Casse l'encapsulation !
};

// Option 2: Interface compliquée pour l'utilisateur (MAUVAIS)
auto addr = factory->createAddress("localhost", 8080);
socket->connect(addr->getSockAddr(), addr->getSize());  // Trop complexe !
```

**Avec `friend` (✅ Bonne approche) :**
```cpp
// Interface simple et clean pour l'utilisateur
auto socket = factory->createTcpSocket();
socket->connect("localhost", 8080);  // Simple et élégant !
```

### 2. TcpSocket ↔ SocketPoller

```cpp
class TcpSocket : public ISocket {
private:
    int _fd;                        // File descriptor système
    friend class SocketPoller;      // Accès contrôlé
    
    int getFileDescriptor() const;  // Méthode système privée
};
```

**Problème résolu :**
- `SocketPoller` a besoin du file descriptor pour les opérations `poll()`
- Le file descriptor est un détail d'implémentation système
- `friend` permet le polling sans exposer les détails Unix/Linux

**Utilisation :**
```cpp
// SocketPoller peut accéder au fd interne
void SocketPoller::addSocket(const TcpSocket& socket, int events) {
    addSocket(socket.getFileDescriptor(), events);  // Accès via friend
}
```

## 🎯 Pattern de Conception : Controlled Access

Cette utilisation des `friend` implémente le pattern **"Controlled Access"** :

1. **Encapsulation préservée** : L'utilisateur final ne voit jamais les détails système
2. **Collaboration interne** : Les classes peuvent collaborer efficacement
3. **Interface stable** : L'API publique reste simple et intuitive
4. **Testabilité** : Les interfaces permettent les mocks facilement

## 📚 Comparaison avec les Standards

Cette approche est utilisée dans :

- **STL C++** : `std::vector` et ses itérateurs
- **Boost Libraries** : Nombreux exemples dans Boost.Asio
- **Qt Framework** : Classes réseau de Qt
- **POCO Libraries** : Framework réseau C++

```cpp
// Exemple STL - même principe
template<class T>
class vector {
    friend class vector_iterator;  // Accès contrôlé aux détails internes
    // ...
};
```

## ✅ Règles Respectées

Notre utilisation des `friend` respecte les **best practices** :

1. **Relation justifiée** : Couplage logique entre les classes
2. **Encapsulation préservée** : Détails système cachés
3. **Interface simple** : API utilisateur épurée
4. **Performance** : Pas de surcoût d'abstraction
5. **Maintenabilité** : Code organisé et documenté

## 🚫 Anti-patterns Évités

Nous évitons ces erreurs courantes :

```cpp
// ❌ MAUVAIS : friend partout
class A {
    friend class B;
    friend class C;
    friend class D;  // Trop de friends = mauvais design
};

// ❌ MAUVAIS : éviter la réflexion
class Calculator {
    friend class UI;  // Relation non justifiée
};

// ❌ MAUVAIS : tout public
class NetworkAddress {
public:
    sockaddr_in _addr;  // Détails système exposés !
};
```

## 🔧 Exemple d'Usage Complet

```cpp
#include "NetworkAPI.hpp"

// Création via Factory Pattern
auto factory = Network::createFactory();
auto socket = factory->createTcpSocket();
auto poller = factory->createPoller();

// Utilisation simple - détails système cachés
socket->connect("localhost", 8080);
socket->setNonBlocking();

// Polling avancé - friend permet l'accès interne
poller->addSocket(*socket, POLLIN);
if (poller->poll(1000) > 0) {
    if (poller->hasEvent(*socket, POLLIN)) {
        std::string data = socket->receive();
    }
}
```

## 🎖️ Conclusion

L'utilisation des `friend` dans ce module :

- ✅ **Améliore l'encapsulation** (paradoxalement)
- ✅ **Simplifie l'API utilisateur**
- ✅ **Respecte les standards de l'industrie**
- ✅ **Optimise les performances**
- ✅ **Facilite la maintenance**

Cette approche représente du **C++ avancé et professionnel**, conforme aux pratiques des bibliothèques de référence.

---

**Auteurs :** Équipe Zappy
**Date :** Juin 2025
**Version :** 1.0
