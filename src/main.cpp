#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <iostream>
#include <assert.h>
#include <chrono>
#include <mutex>
#include <omp.h>
#include "CLHEP/Random/MTwistEngine.h"

#define NB_STATUS 10
#define NB_TIRAGES 10

std::mutex mutex;

/**
 * @brief Sauvegarde les états du générateur et vérifie leur reproductibilité.
 *
 * @param s Le moteur MTwistEngine utilisé pour générer les nombres aléatoires.
 */
void SaveEtRestoreStatus(CLHEP::MTwistEngine *s)
{
  double saveTirages[NB_STATUS][NB_TIRAGES];
  // Tableau pour stocker les tirages générés avant la sauvegarde.
  double flat;
  std::cout << "Début de la sauvegarde des états..." << std::endl;

  // Sauvegarde des états dans NB_STATUS nombre de fichiers
  for (int status = 0; status < NB_STATUS; status++)
  {
    std::cout << "Sauvegarde dans status_sm_" << status << "..." << std::endl;
    s->saveStatus(("./status/status_sm_" + std::to_string(status)).c_str());
    // Génère quelques nombres et les sauvegarde pour le Test
    for (int tirage = 0; tirage < NB_TIRAGES; tirage++)
    {
      saveTirages[status][tirage] = s->flat();
    }
  }
  std::cout << std::endl;
  // Vérification de la reproductibilité des états sauvegardés
  for (int status = 0; status < NB_STATUS; status++)
  {
    std::cout << "Comparaison pour restaurer l'état " << status << " : " << std::endl;
    // Restaure l'état à partir du fichier sauvegardé
    s->restoreStatus(("./status/status_sm_" + std::to_string(status)).c_str());

    for (int tirage = 0; tirage < NB_TIRAGES; tirage++)
    {
      flat = s->flat(); // Tirage restauré après restauration de l'état.
      // L'assertion échouera si les valeurs ne correspondent pas
      assert(flat == saveTirages[status][tirage]);

      // Affichage des valeurs pour vérification visuelle
      std::cout << flat << " = " << saveTirages[status][tirage] << std::endl;
    }
  }
}

/**
 * @brief Sauvegarde les états du générateur après un grand nombre de tirages.
 *
 * @param s Le moteur MTwistEngine utilisé pour générer les nombres aléatoires.
 */
void saveStatusGrandNmbre(CLHEP::MTwistEngine *s)
{
  std::cout << "Début de la sauvegarde des états séparés par 2 000 000 000 nombres..." << std::endl;

  // Sauvegarde des états dans NB_STATUS fichiers
  for (int save = 0; save < NB_STATUS; save++)
  {
    std::cout << "Sauvegarde dans status_sm_2-" << save << "..." << std::endl;
    // Sauvegarde l'état du moteur dans un fichier spécifique après avoir généré un grand nombre de tirages
    s->saveStatus(("./status/status_sm_2-" + std::to_string(save)).c_str());

    // Génère un grand nombre de tirages (2 milliards) pour séparer les états
    for (long tirage = 0; tirage < 2'000'000'000; tirage++)
    {
      s->flat();
    }
  }
}

/**
 * @brief Calcule l'approximation du volume d'une sphère à l'aide de la méthode de Monte-Carlo.
 *
 * @param s Le moteur MTwistEngine utilisé pour générer les nombres aléatoires.
 * @param nbPoints Le nombre total de points à générer pour calculer l'approximation.
 */
double calculerVolumeSphere(CLHEP::MTwistEngine *s, int nbPoints)
{
  unsigned long pointsDansSphère = 0;
  double x, y, z;

  for (long i = 0; i < nbPoints; i++)
  {
    x = s->flat() * 2 - 1;
    y = s->flat() * 2 - 1;
    z = s->flat() * 2 - 1;

    // Vérifie si le point est dans la sphère de rayon 1
    if (x * x + y * y + z * z <= 1)
    {
      pointsDansSphère++;
    }
  }

  return 8.0 * pointsDansSphère / nbPoints;
}

/**
 * @brief Effectue 10 réplications du calcul du volume de la sphère de manière séquentielle.
 *
 * @param s Le moteur MTwistEngine utilisé pour générer les nombres aléatoires.
 * @param nbPoints Le nombre de points à générer pour chaque réplication (1 milliard).
 */
void calculerVolumeSphereSequentiel(CLHEP::MTwistEngine *s, long nbPoints)
{
  std::cout << "Début du calcul du volume de la sphère de manière séquentielle..." << std::endl;
  double sommeVolumes = 0;
  auto debut = std::chrono::high_resolution_clock::now();

  for (int replication = 0; replication < 10; replication++)
  {
    // Calculer le volume pour cette réplication
    sommeVolumes += calculerVolumeSphere(s, nbPoints);
  }

  auto fin = std::chrono::high_resolution_clock::now();
  auto temps = std::chrono::duration_cast<std::chrono::milliseconds>(fin - debut);
  std::cout << "Volume moyen de la sphère calculé : " << sommeVolumes / 10 << " fait en " << temps.count() << " ms." << std::endl;
}

/**
 * @brief Effectue un calcul parallèle pour estimer le volume de la sphère.
 *
 * @param s Le moteur MTwistEngine utilisé pour générer les nombres aléatoires.
 * @param nbPoints Le nombre total de points à générer pour chaque tâche parallèle.
 * @param fileName Le nom du fichier contenant l'état du générateur.
 */
void calculerVolumeSphereParallel(CLHEP::MTwistEngine *s, long nbPoints, const std::string &fileName = "./status/status_sm_2-0") 
{
    auto start = std::chrono::high_resolution_clock::now();

    double pi;

    s->restoreStatus(fileName.c_str());

    pi = calculerVolumeSphere(s, nbPoints);

    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = end - start;
    double timeInMs = duration.count() * 1000.0;

    mutex.lock();
    std::cout << "Volume estimé de la sphère : " << pi << std::endl;
    std::cout << "Temps d'exécution : " << timeInMs << " ms" << std::endl;
    mutex.unlock();
}


/**
 * @brief Version V2 - Effectue un calcul parallèle pour estimer le volume de la sphère en utilisant OpenMP.
 *
 * @param s Le moteur MTwistEngine utilisé pour générer les nombres aléatoires.
 * @param nbPoints Le nombre total de points à générer pour chaque tâche parallèle.
 * @param fileName Le nom du fichier contenant l'état du générateur.
 */
void calculerVolumeSphereParallelV2(CLHEP::MTwistEngine *s, long nbPoints, const std::string &fileName = "./status/status_sm_2-0")
{
  double pi;

  auto start = std::chrono::high_resolution_clock::now();

  s->restoreStatus(fileName.c_str());

// Début de la parallélisation avec OpenMP
#pragma omp parallel for reduction(+ : pi)
  for (long i = 0; i < nbPoints; i++)
  {
    double x = s->flat() * 2 - 1;
    double y = s->flat() * 2 - 1;
    double z = s->flat() * 2 - 1;

    if (x * x + y * y + z * z <= 1)
    {
      pi += 1.0;
    }
  }

  pi = 8.0 * pi / nbPoints;
  
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  double timeInMs = duration.count() * 1000.0;

  std::lock_guard<std::mutex> lock(mutex);
  std::cout << "Pi estimé: " << pi << std::endl;
  std::cout << "Temps d'exécution: " << timeInMs << " ms" << std::endl;
}

/**
 * @brief Génère un peptide aléatoire de longueur donnée.
 * 
 * Cette fonction génère un peptide en sélectionnant aléatoirement des nucléotides
 * ('a', 'g', 't', 'c') en fonction des tirages d'un moteur de génération de nombres
 * aléatoires. Le peptide est construit en concatenant les nucléotides générés
 * jusqu'à atteindre la longueur spécifiée.
 * 
 * @param s Moteur MTwistEngine utilisé pour générer les nombres aléatoires.
 * @param length Longueur du peptide à générer.
 * 
 */
std::string generateRandomPeptide(CLHEP::MTwistEngine *s, int length) {
    std::string peptide = "";
    for (int i = 0; i < length; ++i) {
        // Sélectionne aléatoirement un nucléotide parmi 'a', 'g', 't', 'c'
        int randomIndex = static_cast<int>(s->flat() * 4);
        char nucleotide = "gatc"[randomIndex];
        peptide += nucleotide;
    }
    return peptide;
}

/**
 * @brief Génère des peptides aléatoires jusqu'à obtenir le peptide cible "gattaca".
 * 
 * Cette fonction génère des peptides aléatoires en utilisant la fonction
 * generateRandomPeptide jusqu'à ce que le peptide généré corresponde à la chaîne cible "gattaca".
 * Elle affiche le nombre d'essais nécessaires pour obtenir le peptide correct, ainsi
 * que le temps total écoulé pour accomplir cette tâche.
 * 
 * @param s Moteur MTwistEngine utilisé pour générer les nombres aléatoires.
 */
void generateGattacaPeptide(CLHEP::MTwistEngine *s) {
    const std::string target = "gattaca";
    int length = target.length();
    int attempts = 0;
    std::string peptide;

    auto start = std::chrono::high_resolution_clock::now();

    // Tant que le peptide généré n'est pas égal à "gattaca"
    do {
        peptide = generateRandomPeptide(s, length);
        attempts++;
    } while (peptide != target);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Affichage des résultats
    std::lock_guard<std::mutex> lock(mutex);
    std::cout << "Peptide généré : " << peptide << std::endl;
    std::cout << "Nombre d'essais pour obtenir '" << target << "': " << attempts << std::endl;
    std::cout << "Temps écoulé : " << duration.count() << " ms" << std::endl;
}



int main(int argc, char **argv)
{
  CLHEP::MTwistEngine *s = new CLHEP::MTwistEngine();
  int choice = std::stoi(argv[1]);

  switch (choice)
  {
  case 2:
    SaveEtRestoreStatus(s);
    break;
  case 3:
    saveStatusGrandNmbre(s);
    break;
  case 4:
    calculerVolumeSphereSequentiel(s, 1'000'000'000);
    break;
  case 5:
    calculerVolumeSphereParallel(s, 1'000'000'000, argv[2]);
    break;
  case 6:
    calculerVolumeSphereParallelV2(s, 1'000'000'000);
    break;
  case 7:
    generateGattacaPeptide(s);
    break;
  default:
    std::cerr << "Option invalide. Veuillez choisir entre 2, 3, 4, 5 et 6." << std::endl;
    break;
  }

  delete s;
  return 0;
}
