# PRESENTION DU PROJET

L’objectif du projet réside dans la conception et le développement d’un moteur de workflows simplifié pour l’obtention de la convention de stage d’un étudiant passe par différentes activités. 
Dans cet exemple, nous avons 7 activités différentes et 6 transitions (spécifiés dans un fichier xml correspondant à un processus de demande de stage). Chaque activité est réalisée par un performer qui correspond au login d’un utilisateur connecté. Le parsing des fichiers XML se fait par la méthode SAX avec la bibliothèque libXML2. 
L'éxecution du code se fait e trois parties:

`serveur`: programme principal qui gère à la fois les clients connectés et le processus administrateur

`client`: Lancement de thread de gestion des clients connecté au serveur en TCP

`administrateur`: Le processus système administrateur communique avec le processus principale via des file de messages IPC en analysant les arguments passés en paramètre. 

# Contenu du projet
Le projet est constitué de trois bibliothèques et des programmes serveur et administrateur. Chacune de ces entités est développée dans un répertoire propre. Le projet s’organise donc autourdes six répertoires suivants :

`Socket` - pour les sources permettant de générer la bibliothèque C libsck qui contiendra toutes les méthodes liées au réseau

`Threads` - pour la bibliothèque C libthrd qui contiendra toutes les méthodes permettant d’encapsuler l’utilisation des threads.

`IPC` - pour la bibliothèque C libipc

`Workflow engine` - pour le programme serveur et les modules de traitement des commandes client et administrateur

`Workflows admin` - pour le programme d’administration du serveur

`Models` - pour les différents modèles de processus existants

Un fichier `Makefile` aglobal pour tout le code et des makefiles scpécifiques à chaque réêrtoire 

Une documentation en html via Doxygen à télécharger sur le wiki


# 1°) COMPILATION

Compiler tout le projet sur le répertoire principal du projet avec:

    make clean all

oubien Compiler chaque repertoire avec en mode debugavec:
    
    make clean debug


# 2°) EXECUTION DU PROGRAMME COTE SERVEUR

Se placer sur le répertoire `Workflow_engine` et executer: 

    `./main -p <numPort> -u <fichier> ` 

ou

    ./main --port <numPort> --users file <fichier>`

Ces options permettent de scpécifier éventuellemnt un numéro de port différent de celui par défaut (port 4000) et/ou un autre fichier lors du démarrage du serveur

# 3°) UTILISATION DU PROGRAMME COTE CLIENT

Se connecter au serveur en TCP/IP via l'utilitaire netcac (nc) 

    `nc localhost <numPort>` 

A ce moment vous devrez suivre les étapes indiquées par le serveur et lui donner votre login et mot de passe

Une fois connecté, vous pouvez intéragir avec le serveur via les commandes suivantes:

`aide`  : pour connaître les commandes disponibles

`ToDo` : pour passer en mode gestion de la To-Do List, i.e., gérer les activités assignées à l’utilisateur

`connected users` : pour connaître tous les utilisateurs connectés

`ls processes` : pour lister l’identifiant et l’état de tous les processus. 

`ls processes -ETAT ` : pour ne lister que les processus dans cet état

`ls process <id-process>` : pour récupérer toutes les informations associées à un processus particulier

`create process <file>` : pour créer une nouvelle instance de processus

`exit` : pour quitter l’application et se déconnecter

###                     Dans le mode ToDo
Lorsque qu’un utilisateur est rentré dans le mode ToDo, il peut utiliser les commandes suivantes :

`ls` : pour lister toutes les activités assignées à l’utilisateur (récupération du nom et de l’identifiant). 

`ls <id-activity>`: pour récupérer en plus l’état de l’activité, les entrées et les sorties

`valider` : pour valider une activité. Lors de la validation, le serveur demandera à l’utilisateur préciser le numéro du processus concerne puis l'activité à valider , et enfin de donner une nouvelle valeur pour chaque variable résultat (output).

`exit` : pour quitter le mode ToDo pour revenir au mode initial

# 4°) EXECUTION DU PROGRAMME COTE ADMINISTRATEUR

Les requêtes administrateur sont: 
L’administrateur pourra utiliser les requêtes suivantes :

`./admin -u` ou `./admin --users` : pour obtenir la liste de tous les utilisateurs

`./admin -c` ou `./admin --connected` : pour obtenir la liste de tous les utilisateurs connectés

`./admin -a login:pwd:Prenom-Nom` ou `./admin --add-user login:pwd:Prenom-Nom` : pour ajouter un nouvel utilisateur

`./admin -l` ou `./admin --listen` : permet d’écouter le fonctionnement du moteur, de workflow
