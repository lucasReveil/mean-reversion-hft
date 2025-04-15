import pandas as pd # type: ignore
import matplotlib.pyplot as plt # type: ignore
import os

# Récupère le chemin absolu vers le dossier du script
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# Construit le chemin complet vers les CSV
marketcsv_path = os.path.join(BASE_DIR, "../data/market.csv")
spreadcsv_path = os.path.join(BASE_DIR, "../data/tradesSpread.csv")
mrbcsv_path = os.path.join(BASE_DIR, "../data/tradeMRB.csv")

# Chargement
dfMarket = pd.read_csv(marketcsv_path)
dfSpread = pd.read_csv(spreadcsv_path)
dfMrb = pd.read_csv(mrbcsv_path)

# Calcul de la moyenne mobile (window=100 ticks)
dfMarket['rolling_mean'] = dfMarket['price'].rolling(window=100).mean()
# Calcul des bandes de Bollinger (écart-type glissant sur 100 ticks)
dfMarket['rolling_std'] = dfMarket['price'].rolling(window=100).std()
dfMarket['upper_band'] = dfMarket['rolling_mean'] + 2 * dfMarket['rolling_std']
dfMarket['lower_band'] = dfMarket['rolling_mean'] - 2 * dfMarket['rolling_std']

#FENÊTRE 1 : Prix du marché
plt.figure(figsize=(10, 5))
plt.plot(dfMarket['time'], dfMarket['price'],label="Price", color="blue")

# Axe Y adapté
min_price = dfMarket['price'].min()
max_price = dfMarket['price'].max()
plt.ylim(min_price - 10, max_price + 10)
plt.xlim(0, dfMarket['time'].max())

plt.title("Market Price")
plt.xlabel("Time (ms)")
plt.ylabel("Price")
# Tracé des bandes et de la moyenne
plt.plot(dfMarket['time'], dfMarket['rolling_mean'], label="Rolling Mean (100)", color="orange", linestyle='--')
plt.plot(dfMarket['time'], dfMarket['upper_band'], label="Upper Bollinger Band", color="gray", linestyle=':')
plt.plot(dfMarket['time'], dfMarket['lower_band'], label="Lower Bollinger Band", color="gray", linestyle=':')
plt.fill_between(dfMarket['time'], dfMarket['lower_band'], dfMarket['upper_band'], color='gray', alpha=0.1)
# Ajout des points d'entrée/sortie
# Long entries
long_trades = dfMrb[dfMrb['direction'] == "LONG"]
plt.scatter(long_trades['entry_time'], long_trades['buy_price'],
            color='green', marker='^', label='Long Entry')

# Short entries
short_trades = dfMrb[dfMrb['direction'] == "SHORT"]
plt.scatter(short_trades['entry_time'], short_trades['buy_price'],
            color='red', marker='v', label='Short Entry')
# Sorties (communes aux longs et shorts)
plt.scatter(dfMrb['exit_time'], dfMrb['sell_price'],
            color='black', marker='x', label='Exit')
plt.legend()
plt.grid(True)

#  FENÊTRE 2 : PnL 
plt.figure(figsize=(10, 5))
plt.plot(dfSpread['trade_id'], dfSpread['pnl'], label="Cumulative PnL Spread", color="green")
plt.plot(dfMrb['trade_id'], dfMrb['pnl'], label="Cumulative PnL MRB", color="blue")

plt.title("Cumulative PnL by Strategy")
plt.xlabel("Trade #")
plt.ylabel("PnL")
plt.legend()
plt.grid(True)

# Axe X auto, padding si besoin
max_trade = max(dfSpread['trade_id'].max(), dfMrb['trade_id'].max())
plt.xlim(0, max_trade + 5)

plt.tight_layout()
plt.show()
