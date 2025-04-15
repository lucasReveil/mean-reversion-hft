# Mean Reversion Trading Simulator

Petit projet perso pour simuler des stratégies de trading ultra simples type *mean reversion*, en mode HFT (ou presque).

## 💡 Objectif

perfectionner mon niveau en c++ (et en math), comprendre un peu les bases d’un engine de backtest maison, et structurer proprement un mini projet de trading.


## Structure

- `src/` → les sources C++
- `include/` → headers
- `scripts/` → scripts Python pour visualiser les résultats
- `data/` → fichiers CSV générés
- `plots/` → pour sauvegarder les plots (optionnel)
- `Makefile` → compilation simple (`make && ./sim`)
- `main.cpp` → point d’entrée de la simulation

## Stratégies testées

- **Scalping basic** : entre quand le spread est petit, revend au tick suivant
- **Mean Reversion Bot (MRB)** : entrée sur retour vers moyenne ± 2σ, sortie take profit ou trailing stop

## Modes

- `offline` : génère tout en une passe (plus rapide pour les tests/plots)
- `live` : tick toutes les 10ms, comme un vrai flux de marché (plus lent)

Exemple :
```
./sim offline 42
```
Seed fixé à `42` pour générer un marché identique entre les runs.

## Visualisation

Une fois la sim terminée, `plot_results.py` s'exécute automatiquement pour afficher :

- le prix du marché
- les résultats des 2 bots
- les trades du bot MRB
- les indicateurs comme la moyenne glissante, Bollinger Bands, signaux d'entrée...


### Fondations mathématiques

Le cœur de la simulation de marché repose sur le **Mouvement Brownien Géométrique (GBM)**, un modèle stochastique largement utilisé pour modéliser l’évolution des prix d’actifs financiers.  
La formule continue du GBM est donnée par :

    dS_t = μ S_t dt + σ S_t dW_t

Où :
- S_t est le prix de l’actif à l’instant t,
- μ est le taux de drift (rendement attendu),
- σ est la volatilité,
- W_t est un processus de Wiener (mouvement brownien standard).

---

#### Simulation numérique (version discrète)

Dans le code, ce processus est discrétisé pour une simulation pas à pas avec la formule :

    S_{t+1} = S_t × exp[(μ - 0.5 σ²) × Δt + σ × √Δt × ε]

Avec :
- ε ~ N(0, 1) (bruit gaussien),
- Δt est l’intervalle de temps en années (ex : 1.0/252.0 pour un jour boursier).

Ce modèle assure que S_t > 0, ce qui est crucial pour les prix d’actifs.  
Il permet de capturer à la fois la tendance moyenne (drift) et l’aléa du marché (volatilité).

---
[Wiki Mouvement Brownien](https://en.wikipedia.org/wiki/Brownian_motion)

### Statistiques utilisées

Pour le calcul de la moyenne et de la volatilité (σ) dans la stratégie de réversion à la moyenne :

**Moyenne mobile :**

    μ = (1 / n) × Σ x_i

**Écart-type (volatilité empirique) :**

    σ = sqrt[(1 / (n - 1)) × Σ (x_i - μ)²]

Ceci est utilisé pour tracer les bandes de Bollinger (μ ± 2σ) et déclencher les signaux d’achat ou de vente.
[Wiki Bandes de Bollinger](https://en.wikipedia.org/wiki/Bollinger_Bands)
