# Simulador de Trading por Reversión a la Media (Mean Reversion)

Un pequeño proyecto personal para simular estrategias de trading sencillas tipo *mean reversion*, con un enfoque tipo HFT (High Frequency Trading)... o al menos aproximado.

## 🎯 Objetivo

Mejorar mis habilidades en C++ (y matemáticas), entender cómo se puede construir un motor de backtesting básico desde cero, y estructurar un mini proyecto de trading de forma clara y modular.

---

## 📁 Estructura del Proyecto

- `src/` → código fuente C++
- `include/` → headers (.h)
- `scripts/` → scripts en Python para visualizar resultados
- `data/` → archivos CSV generados por los bots
- `plots/` → donde se pueden guardar gráficos generados
- `Makefile` → para compilar fácilmente con `make && ./sim`
- `main.cpp` → punto de entrada de la simulación

---

## ⚙️ Estrategias Implementadas

- **Scalping básico**  
  Entra cuando el spread es pequeño, vende en el siguiente tick.

- **Bot de Reversión a la Media (MRB)**  
  Entra cuando el precio vuelve dentro de las bandas (μ ± 2σ), sale con trailing stop o take profit.

---

## 🔁 Modos de Ejecución

- `offline` → genera todo el mercado de una vez para análisis rápido.
- `live` → simula un mercado en tiempo real, tick cada 10ms.

Ejemplo:
```
./sim offline 42
```
Usa una semilla fija `42` para reproducir exactamente el mismo mercado.

---

## 📈 Visualización

Una vez finalizada la simulación, el script `plot_results.py` se ejecuta automáticamente y genera:

- el precio del mercado
- PnL (ganancias) acumulado de los dos bots
- entradas y salidas del bot MRB
- medias móviles, bandas de Bollinger, y señales

---

## 🧠 Fundamentos Matemáticos

Este simulador usa un **Movimiento Browniano Geométrico (GBM)** para modelar el precio del activo financiero:

### Modelo continuo:
    dS_t = μ S_t dt + σ S_t dW_t

Donde:
- S_t → precio del activo en el tiempo t,
- μ → tasa de crecimiento esperada,
- σ → volatilidad,
- W_t → movimiento browniano estándar (ruido aleatorio).

### Simulación numérica (discretización):
    S_{t+1} = S_t × exp[(μ - 0.5σ²) × Δt + σ × √Δt × ε]

Donde:
- ε ~ N(0, 1)
- Δt → intervalo temporal en años (ej: 1/252 para 1 día bursátil)

---

## 📊 Estadísticas

### Media móvil:
    μ = (1 / n) × Σ x_i

### Desviación estándar:
    σ = sqrt[(1 / (n - 1)) × Σ (x_i - μ)²]

Usado para calcular las **bandas de Bollinger** (μ ± 2σ) y activar señales de entrada/salida.

---

Gracias por leer 🙏 ¡Disfruta probando el código!