# MiniProjetC
 mini projet de C

projet : creation d'un miniplatormer style doodleJump
base : exemple raylib platformer with 2D camera

//--------------------------------------
//Comment lancer le jeu
//--------------------------

Apres avoit telecharger le dossier MiniProjetC complet :

Cas 1 : Lancer l'executable ( fonctionne sous window, apparamment ne fonctionne pas sous MacOs)

Cas 2 : La solution presentée au dessus ne fonctionne pas, vous pouvez esseyer de recompiler le projet :
soit :

	- telecharger raylib
	
	- ouvrir NotePad ++ for raylib
	
	- a partir de NotePad++ for raylib, ouvrir miniProjetMzin.c
	
	- compiler le code avec F6
	
Normalement, la compilation lance le jeu et genere egalement un excutable que vous pourrez utiliser

Sinon : 

	-ajouter dans le dossier lib la librairie de votre systeme d exploitation ( contient celle pour window de base )
	
	-gcc -o miniProjetmain.exe miniProjetmain.c  -lraylib -lopengl32 -lgdi32 -lwinmm -L lib/ -I .

//---------------------------------------
//Comment l'utiliser
//---------------------

Les touches indiquées en jeu correspondent a un claver azerty

Une fois l'application lancée, vous pouvez commencer a jouer en appuyant sur Entrée

but du jeu : survivre le plus longtemps possible en sautant de platforme en platforme

Le menu vous permet de:
	-changer l'apparence du cube manipulé en jeu
	-Voir et reset les scores

//----------------------------------------
//Description des fichiers
//------------------------------

raymath.h : librairie de raylib utilisé notamment pour les vecteurs

storageDate : sauvegarde les scores

miniProjetmain.exe : executable du jeu

miniProjetmain.c : code source du jeu

ressource : contient le fichier audio utilisé dans le jeu


//--------------------------------------
//Resumé des améliorations :
//---------------------------

	le cube rebondit
	le cube peut passer d'un bord à l'autre de l'ecran
	la map est generée aléatoirement à chaque nouvelle partie
	les platformes disparaissent et ne sont plus actives quand elles sont en dehors de l'ecran ( en bas )
	arriver à la derniere platforme de la map entraine une victoire
	tomber de la map entraine un GameOver
	la map descend toute seule
	la vitesse de descente de la map augmente avec le score
	son de collision a chaque platforme sur laquelle rebondit le cube
	le score est calcule en temps reel -> correspond au temps en jeu
	en jeu , le joueur peut arreter la partie ( provoque un GameOver )
	depuis l'ecran de GameOver, le joueur peut retourner au menu ou rejouer
	hiscore et lastscore affichés sur l ecran de menu et de GameOver
	reset des scores possible depuis l'ecran de menu

	menu principal-> lancer une partie
	menu principal-> menu de changement de skin
	menu principal-> reset des scores

	menu skin -> choix d 'une nouvelle couleur pour le cube, a choisir parmi celles proposées
	menu skin -> revenir au menu principal

//--------------------------------------------
//Point d'amelioration
//-----------------------------

	Génaration infini de la map
	Complexification de la map au cours du temps ( densite de platforme moins elevé, platforme mobile ... )
	Musique de fond
	Implémenter l'utilisation de la souris
	Rajouter les textures : pour le background, les platformes et le cube
