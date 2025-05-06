import pandas as pd
import matplotlib.pyplot as plt
import math

# Lecture du csv
df = pd.read_csv("results_compare.csv", names=["fonction", "taille", "mono", "multi", "threads"])

# Paramètres pour l'affichage du plot (pour afficher tous les graphes en une page)
n_fonctions = len(df["fonction"].unique())  # Nombre de sous-graphes à afficher
cols = 2  # Nombre de colonnes
rows = math.ceil(n_fonctions / cols)  # Nombre de lignes

# Créer un subplot pour chaque fonction
fig, axes = plt.subplots(rows, cols, figsize=(12, 3.5 * rows), constrained_layout=True)
axes = axes.flatten()  # Très important : aplatir le tableau 2D

# Plot de chaque fonction
for i, nom_fonction in enumerate(df["fonction"].unique()):
    sous_df = df[df["fonction"] == nom_fonction]
    ax = axes[i]
    ax.plot(sous_df["taille"], sous_df["mono"], marker="o", label=f"{nom_fonction} mono")
    ax.plot(sous_df["taille"], sous_df["multi"], marker="s", label=f"{nom_fonction} multi")
    
    ax.set_title(f"Comparaison des performances - {nom_fonction}")
    ax.set_xlabel("Taille des données")
    ax.set_ylabel("Temps d'exécution [s]")
    ax.legend()
    ax.grid()
    

nb_threads = df["threads"].iloc[0]  # Suppose que c'est la même valeur pour toutes les lignes
fig.suptitle(f"Comparaison des performances en fonction du nombre de threads ({nb_threads} threads)", fontsize=16)

# Si nombre de fonctions est impair, cacher les axes restants non utilisés
for j in range(i + 1, len(axes)):
    fig.delaxes(axes[j])

plt.tight_layout()
plt.savefig("comparaison_performances.png", dpi=300)
#plt.show()
