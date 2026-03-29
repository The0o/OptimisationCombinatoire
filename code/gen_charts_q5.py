import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
import csv
from collections import defaultdict

OUT = '/Users/toutouille/Documents/OptimisationCombinatoire/code/'

q5 = defaultdict(dict)
with open(OUT + 'bench_Q5.csv') as f:
    for row in csv.DictReader(f, delimiter=';'):
        key = (row['Instance'], row['Probleme'])
        q5[key][row['Algorithme']] = float(row['Score'])

q4 = {}
with open(OUT + 'bench_Q4.csv') as f:
    for row in csv.DictReader(f, delimiter=';'):
        key = (row['Instance'], row['Probleme'])
        q4[key] = float(row['Score'])

def get_gains(pb):
    gains_ils = {'k1':[], 'k2':[], 'k3':[], 'k4':[]}
    gains_vns = []
    for key in q5:
        inst, p = key
        if p != pb: continue
        hc = q4.get(key, 0)
        if hc <= 0: continue
        for k in [1,2,3,4]:
            s = q5[key].get(f'ILS_k{k}', 0)
            gains_ils[f'k{k}'].append((s - hc) / hc * 100)
        vns = q5[key].get('VNS_kmax5', 0)
        gains_vns.append((vns - hc) / hc * 100)
    return gains_ils, gains_vns

# GRAPHIQUE 1 : Boxplot gains distribution
fig, axes = plt.subplots(1, 2, figsize=(13, 6))
fig.suptitle('Q5 – Distribution des gains (%) par rapport au Hill Climber Q4\n(65 instances DIMACS)', fontsize=13, fontweight='bold')
for ax, pb in zip(axes, ['MCP', 'WMCP']):
    gains_ils, gains_vns = get_gains(pb)
    data = [gains_ils['k1'], gains_ils['k2'], gains_ils['k3'], gains_ils['k4'], gains_vns]
    labels = ['ILS k=1', 'ILS k=2', 'ILS k=3', 'ILS k=4', 'VNS k≤5']
    colors = ['#a8c5e8', '#5b9bd5', '#2e75b6', '#1c4e80', '#70ad47']
    bp = ax.boxplot(data, patch_artist=True, notch=False, widths=0.5)
    for patch, c in zip(bp['boxes'], colors):
        patch.set_facecolor(c); patch.set_alpha(0.8)
    for median in bp['medians']:
        median.set_color('black'); median.set_linewidth(2)
    ax.axhline(0, color='#ed7d31', linestyle='--', linewidth=1.5, label='HC Q4 (référence)')
    ax.set_title(pb, fontsize=12, fontweight='bold')
    ax.set_xticklabels(labels, fontsize=9)
    ax.set_ylabel('Gain vs HC Q4 (%)', fontsize=11)
    ax.grid(axis='y', alpha=0.3)
    ax.legend(fontsize=9)
plt.tight_layout()
plt.savefig(OUT + 'q5_gain_distribution.png', dpi=150, bbox_inches='tight')
plt.close()
print("q5_gain_distribution.png OK")

# GRAPHIQUE 2 : Effet de k (médiane sur toutes instances)
fig, axes = plt.subplots(1, 2, figsize=(12, 5))
fig.suptitle('Q5 – Validation du paramètre k : gain médian sur 65 instances', fontsize=13, fontweight='bold')
k_vals = [1, 2, 3, 4]
for ax, pb in zip(axes, ['MCP', 'WMCP']):
    gains_ils, gains_vns = get_gains(pb)
    medians = [np.median(gains_ils[f'k{k}']) for k in k_vals]
    q25 = [np.percentile(gains_ils[f'k{k}'], 25) for k in k_vals]
    q75 = [np.percentile(gains_ils[f'k{k}'], 75) for k in k_vals]
    ax.plot(k_vals, medians, marker='o', linewidth=2.5, color='#2e75b6', label='ILS (médiane)', markersize=8, zorder=3)
    ax.fill_between(k_vals, q25, q75, alpha=0.2, color='#2e75b6', label='ILS [Q25–Q75]')
    vns_med = np.median(gains_vns)
    ax.axhline(vns_med, color='#70ad47', linestyle='--', linewidth=2, label=f'VNS médiane ({vns_med:.1f}%)')
    ax.axhline(0, color='#ed7d31', linestyle=':', linewidth=1.5, label='HC Q4 = 0%')
    ax.set_title(pb, fontsize=12, fontweight='bold')
    ax.set_xlabel('k (taille perturbation)', fontsize=11)
    ax.set_ylabel('Gain médian vs HC Q4 (%)', fontsize=11)
    ax.set_xticks(k_vals)
    ax.legend(fontsize=9)
    ax.grid(alpha=0.3)
plt.tight_layout()
plt.savefig(OUT + 'q5_parametrisation_k_full.png', dpi=150, bbox_inches='tight')
plt.close()
print("q5_parametrisation_k_full.png OK")

# GRAPHIQUE 3 : Scatter ILS_k3 vs HC Q4
fig, axes = plt.subplots(1, 2, figsize=(12, 5))
fig.suptitle('Q5 – ILS k=3 vs Hill Climber Q4 (toutes instances)\nPoints au-dessus de la diagonale = amélioration', fontsize=12, fontweight='bold')
for ax, pb in zip(axes, ['MCP', 'WMCP']):
    hc_vals, ils_vals = [], []
    for key in q5:
        inst, p = key
        if p != pb: continue
        hc = q4.get(key, 0)
        ils = q5[key].get('ILS_k3', 0)
        if hc > 0 and ils > 0:
            hc_vals.append(hc); ils_vals.append(ils)
    hc_arr = np.array(hc_vals)
    ils_arr = np.array(ils_vals)
    gains = (ils_arr - hc_arr) / hc_arr * 100
    colors_scatter = ['#70ad47' if g > 0 else ('#ed7d31' if g < 0 else '#5b9bd5') for g in gains]
    ax.scatter(hc_arr, ils_arr, c=colors_scatter, alpha=0.75, s=60, edgecolors='white', linewidths=0.5)
    lim = max(max(hc_vals), max(ils_vals)) * 1.05
    ax.plot([0, lim], [0, lim], 'k--', linewidth=1, alpha=0.5, label='y = x')
    better = sum(1 for g in gains if g > 0)
    worse = sum(1 for g in gains if g < 0)
    equal = sum(1 for g in gains if g == 0)
    ax.set_title(pb, fontsize=12, fontweight='bold')
    ax.set_xlabel('Score HC Q4', fontsize=11)
    ax.set_ylabel('Score ILS k=3', fontsize=11)
    green_p = mpatches.Patch(color='#70ad47', label=f'ILS > HC ({better} inst.)')
    orange_p = mpatches.Patch(color='#ed7d31', label=f'ILS < HC ({worse} inst.)')
    blue_p = mpatches.Patch(color='#5b9bd5', label=f'Égal ({equal} inst.)')
    ax.legend(handles=[green_p, orange_p, blue_p], fontsize=9)
    ax.grid(alpha=0.3)
plt.tight_layout()
plt.savefig(OUT + 'q5_scatter_vs_q4.png', dpi=150, bbox_inches='tight')
plt.close()
print("q5_scatter_vs_q4.png OK")

# GRAPHIQUE 4 : Gain par famille (WMCP, VNS)
families = {'brock': [], 'C': [], 'c-fat': [], 'gen': [],
            'hamming': [], 'johnson': [], 'keller': [], 'MANN_a': [], 'san': []}
for key in q5:
    inst, pb = key
    if pb != 'WMCP': continue
    hc = q4.get(key, 0)
    if hc <= 0: continue
    vns = q5[key].get('VNS_kmax5', 0)
    gain = (vns - hc) / hc * 100
    for fam in families:
        if inst.startswith(fam + '/'):
            families[fam].append(gain); break
fam_names = [f for f in families if families[f]]
fam_means = [np.mean(families[f]) for f in fam_names]
fam_stds = [np.std(families[f]) for f in fam_names]
fam_colors = ['#70ad47' if m > 0 else '#ed7d31' for m in fam_means]
fig, ax = plt.subplots(figsize=(11, 5))
bars = ax.bar(fam_names, fam_means, color=fam_colors, edgecolor='white', linewidth=0.8, alpha=0.85)
ax.errorbar(fam_names, fam_means, yerr=fam_stds, fmt='none', color='black', capsize=4, linewidth=1.5)
ax.axhline(0, color='black', linewidth=1)
ax.set_title("Q5 – Gain moyen VNS vs HC Q4 par famille d'instances (WMCP)", fontsize=12, fontweight='bold')
ax.set_ylabel('Gain moyen (%)', fontsize=11)
ax.set_xlabel("Famille d'instances", fontsize=11)
for bar, mean in zip(bars, fam_means):
    ax.text(bar.get_x() + bar.get_width()/2,
            bar.get_height() + (0.3 if mean >= 0 else -1.5),
            f'{mean:.1f}%', ha='center', va='bottom', fontsize=9, fontweight='bold')
ax.grid(axis='y', alpha=0.3)
plt.tight_layout()
plt.savefig(OUT + 'q5_gain_by_family.png', dpi=150, bbox_inches='tight')
plt.close()
print("q5_gain_by_family.png OK")

# Résumé
print("\n=== RÉSUMÉ GLOBAL ===")
for pb in ['MCP', 'WMCP']:
    gains_ils, gains_vns = get_gains(pb)
    g3 = gains_ils['k3']
    gv = gains_vns
    print(f"\n{pb}:")
    print(f"  ILS k=3 : médiane={np.median(g3):+.1f}%  moyenne={np.mean(g3):+.1f}%  max={max(g3):+.1f}%  min={min(g3):+.1f}%")
    print(f"  VNS     : médiane={np.median(gv):+.1f}%  moyenne={np.mean(gv):+.1f}%  max={max(gv):+.1f}%  min={min(gv):+.1f}%")
    print(f"  ILS k=3 améliore HC: {sum(1 for g in g3 if g>0)}/{len(g3)} instances")
    print(f"  VNS     améliore HC: {sum(1 for g in gv if g>0)}/{len(gv)} instances")
