# Open-pode
Open Pode est un projet de conception d'un hexapode entièrement open source dont la partie mécanique est imprimable en
3D.

## Vidéos
Quelques vidéos de l'avancement du projet sont disponibles sur notre [chaîne Youtube](https://www.youtube.com/channel/UCUNnqteGSC3-gRxhA2PmjtQ).

## Participants
* Guillaume Sarthou (4ème année INSA Toulouse)
* Julien Combattelli (4ème année INSA Toulouse)

## Contacts
Vous pouvez nous contacter à l'adresse suivante : open.pode@gmail.com

## Matériel utilisé
Open Pode embarque une carte Raspberry Pi 2 avec le système d'exploitation Raspbian. Le contrôle est pour l'instant effectué avec une manette Dualshock 4 (de Playstation 4).

## Compilation
Pour compiler les sources, vous pouvez les télécharger puis les compiler directement sur votre Raspberry Pi (RPi).
Nous vous conseillons cependant de les compiler sur un ordinateur puis de transférer l'executable sur la RPi via ssh.

Nous utilisons l'IDE Eclipse combiné aux cross-compilateurs suivants :
* Windows : [Windows toolchain for RPi](http://gnutoolchains.com/raspberry/)
* Linux   : [Official toolchain for RPi](https://github.com/raspberrypi/tools/tree/master/arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf)

## Bugs connus
#### Impossible de connecter la manette
Connectez vous sur la RPi et dans un terminal saisissez la commande suivante :
```
cat ~/.cache/ds4drv.log
```
Si ce fichier contient le message suivant :
```
[info][bluetooth] Scanning for devices
[info][bluetooth] Found device <Adresse MAC de la manette>
[error][bluetooth] Unable to connect to detected device: Failed to set operational mode: [Errno 107] Transport endpoint is not connected
```
Tapez la commande suivante :
```
bluez-test-device remove <Adresse MAC de la manette>
```
Puis redémarrez la Raspberry Pi.

## A faire
- [x] Implement 2D and 3D Vector struct
- [ ] Replace all C arrays with std::array
- [x] Implement circular movements
- [ ] Translate README into English
- [ ] Optimise movements

## Changements importants
* mise en place du controlleur de mouvement (movement_controller)
* mise en place de l'inclinaison 2D : tangage(pitch) et roulis(roll)
  * calcul des coeficients
  * déploiment des coeficients sur les classes enfants
  * prises en compte de l'inclinaison 2D dans las mouvement (z_paw_position_determination)
* prise en compte de butées mécaniques (début)
  * dans servo
  * dans paw
  * dans side
* prise en compte des mouvements de rotation
  * apassage du mode linéaire au mode rotationnel par appuie sur L3
