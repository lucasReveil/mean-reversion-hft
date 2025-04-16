import pandas as pd 
import matplotlib.pyplot as plt
import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# Path building
marketcsv_path = os.path.join(BASE_DIR, "../data/market.csv")
spreadcsv_path = os.path.join(BASE_DIR, "../data/tradesSpread.csv")
mrbcsv_path = os.path.join(BASE_DIR, "../data/tradeMRB.csv")

# csv Loading
dfMarket = pd.read_csv(marketcsv_path)
dfSpread = pd.read_csv(spreadcsv_path)
dfMrb = pd.read_csv(mrbcsv_path)

# SMA Calcul (window=100 ticks)
dfMarket['rolling_mean'] = dfMarket['price'].rolling(window=100).mean()
#  Bollinger Bands Calcul (stddev on window=100 ticks)
dfMarket['rolling_std'] = dfMarket['price'].rolling(window=100).std()
dfMarket['upper_band'] = dfMarket['rolling_mean'] + 2 * dfMarket['rolling_std']
dfMarket['lower_band'] = dfMarket['rolling_mean'] - 2 * dfMarket['rolling_std']

#Graph 1 : Market Price
plt.figure(figsize=(10, 5))
plt.plot(dfMarket['time'], dfMarket['price'],label="Price", color="blue")

# Y Axis ajustement
min_price = dfMarket['price'].min()
max_price = dfMarket['price'].max()
plt.ylim(min_price - 10, max_price + 10)
plt.xlim(0, dfMarket['time'].max())

plt.title("Market Price")
plt.xlabel("Time (ms)")
plt.ylabel("Price")
# Adding Rolling Mean and Bollinger Bands
plt.plot(dfMarket['time'], dfMarket['rolling_mean'], label="Rolling Mean (100)", color="orange", linestyle='--')
plt.plot(dfMarket['time'], dfMarket['upper_band'], label="Upper Bollinger Band", color="gray", linestyle=':')
plt.plot(dfMarket['time'], dfMarket['lower_band'], label="Lower Bollinger Band", color="gray", linestyle=':')
plt.fill_between(dfMarket['time'], dfMarket['lower_band'], dfMarket['upper_band'], color='gray', alpha=0.1)
# Adding Entries and Exits
# Long entries
long_trades = dfMrb[dfMrb['direction'] == "LONG"]
plt.scatter(long_trades['entry_time'], long_trades['buy_price'],
            color='green', marker='^', label='Long Entry')

# Short entries
short_trades = dfMrb[dfMrb['direction'] == "SHORT"]
plt.scatter(short_trades['entry_time'], short_trades['buy_price'],
            color='red', marker='v', label='Short Entry')
# Exits
plt.scatter(dfMrb['exit_time'], dfMrb['sell_price'],
            color='black', marker='x', label='Exit')
plt.legend()
plt.grid(True)

#  Graph 2 : PnL 
plt.figure(figsize=(10, 5))
plt.plot(dfSpread['trade_id'], dfSpread['pnl'], label="Cumulative PnL Spread", color="green")
plt.plot(dfMrb['trade_id'], dfMrb['pnl'], label="Cumulative PnL MRB", color="blue")

plt.title("Cumulative PnL by Strategy")
plt.xlabel("Trade #")
plt.ylabel("PnL")
plt.legend()
plt.grid(True)

# Axe X auto, padding if needed
max_trade = max(dfSpread['trade_id'].max(), dfMrb['trade_id'].max())
plt.xlim(0, max_trade + 5)

plt.tight_layout()
plt.show()
