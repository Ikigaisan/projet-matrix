import pandas as pd
import matplotlib.pyplot as plt

# Lire le CSV avec des noms de colonnes appropriés
df = pd.read_csv("results_compare.csv", names=["fonction", "taille", "mono", "multi"])

# Nombre de sous-graphes que tu souhaites afficher (par exemple, une colonne de 3 graphes)
n_fonctions = len(df["fonction"].unique())
cols = 1  # Nombre de colonnes (1 par défaut, tu peux changer à plusieurs si tu préfères)
rows = n_fonctions  # Le nombre de lignes sera égal au nombre de fonctions

# Créer un subplot pour chaque fonction
fig, axes = plt.subplots(rows, cols, figsize=(8, 6 * rows))  # Ajuste la taille de la figure si nécessaire

# Pour chaque fonction différente
for i, nom_fonction in enumerate(df["fonction"].unique()):
    sous_df = df[df["fonction"] == nom_fonction]
    ax = axes[i] if rows > 1 else axes  # Accéder au bon sous-graphe
    ax.plot(sous_df["taille"], sous_df["mono"], marker="o", label=f"{nom_fonction} mono")
    ax.plot(sous_df["taille"], sous_df["multi"], marker="s", label=f"{nom_fonction} multi")
    
    ax.set_title(f"Comparaison des performances - {nom_fonction}")
    ax.set_xlabel("Taille des données")
    ax.set_ylabel("Temps d'exécution (s)")
    ax.legend()
    ax.grid()

plt.tight_layout()  # Ajuste la disposition des sous-graphiques
plt.show()
