#!/bin/bash

echo "Calcul du volume de la sphère en parallèle..."

# Nombre de threads parallèles
num_threads=10

# Tableau pour stocker les résultats
declare -a results

calculate_average() {
    local total=0
    for result in "${results[@]}"; do
        total=$(echo "$total + $result" | bc)
    done
    echo "scale=10; $total / $num_threads" | bc
}

# Heure de début
start_time=$(date +%s.%N)

# Lancement des threads avec les bons paramètres
for ((i=0; i<$num_threads; i++)); do
  ./tp5 5 "./status/status_sm_2-$i" > "resultat_thread$i.txt" &
done

# Attente de la fin de tous les threads
wait

# Récupération des résultats et nettoyage des fichiers temporaires
for ((i=0; i<$num_threads; i++)); do
    result=$(grep -oP '\d+\.\d+' "resultat_thread$i.txt")
    results[$i]=$result
    rm "resultat_thread$i.txt"
done

# Heure de fin
end_time=$(date +%s.%N)

# Temps total écoulé
all_time=$(echo "$end_time - $start_time" | bc)
echo "Temps total : $all_time secondes"

# Calcul et affichage de la moyenne
average=$(calculate_average)
echo "Volume moyen pour tous les threads : $average"

echo "Calcul terminé 👀🔥."
