# MiniProjetC
 mini projet de C

projet : creation d'un miniplatormer style doodleJump
base : exemple raylib platformer with 2D camera 
Resumé des améliorations :

	le cube rebondit 
	le cube peut passer d'un bord a l'autre de l'ecran
	la map est generé aléatoirement a chaque nouvelle partie
	les platformes disparaissent et ne sont plus actives qd elles sont en dehors de l'ecran ( en bas ) 
	arriver à la derniere platforme de la map entraine une victoire
	tomber de la map entraine un GameOver
	la map descend toute seule
	la vitesse de descente de la map augmente avec le score
	son de collision a chaque platforme sur laquelle rebondit le cube
	le score est calcule en temps reel -> correspond au temps en jeu
	en jeu , le joueur peut arreter la partie ( provoque un GameOver )
	depuis l'ecran de GameOver, le joueur peut retourner au menu ou rejouer
	hiscore et lastscores afficher sur l ecran de menu et de GameOver
	reset des scores possible depuis l'ecran de menu
	menu -> lancer une partie
	menu -> menu de changmt de skin
	menu -> reset des scores

	skin -> choix d 'une nouvelle couleur pour le cube, a choisir parmi celles proposées
	skin -> revenir au menu principal
