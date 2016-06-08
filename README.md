Open-pode
==================

Open Pode est un projet de conception d'un hexapode entièrement open source dont la partie mécanique est imprimable en 
3D.

Vidéos
------------------
Quelques vidéos de l'avancement du projet sont disponibles sur notre chaîne Youtube :
https://www.youtube.com/channel/UCUNnqteGSC3-gRxhA2PmjtQ

Contacts
------------------
* Guillaume Sarthou (3ème année INSA Toulouse), guillaum324bis@hotmail.fr
* Julien Combattelli  (3ème année INSA Toulouse), julien.combattelli@hotmail.com

changements importants :
-mise en place du controlleur de mouvement (movement_controller)
-mise en place de l'inclinaison 2D : tangage(pitch) et roulis(roll)
	-calcul des coeficients
	-déploiment des coeficients sur les classes enfants
	-prises en compte de l'inclinaison 2D dans las mouvement (z_paw_position_determination)
-prise en compte de butées mécaniques (début)
	-dans servo
	-dans paw
	-dans side
	
Bugs connus
------------------
* S'il vous est impossible de connecter la manette à la raspberry, tapez la commande suivante :
		cat ~/.cache/ds4drv.log
  Si ce fichier contient le message suivant : 
		[info][bluetooth] Scanning for devices
		[info][bluetooth] Found device <Adresse MAC de la manette>
		[error][bluetooth] Unable to connect to detected device: Failed to set operational mode: [Errno 107] Transport endpoint is not connected
  tapez la commande suivante :
		bluez-test-device remove <Adresse MAC de la manette>
  puis redémarrez la raspberry
