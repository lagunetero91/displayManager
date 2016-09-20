# displayManager

Este programa de arduino va realizando la cuenta y mostrando en un display de 7 segmentos los números del 0 al 9 y luego del 9 al 0.

El programa soporta comandos enviados por el puerto serie al Arduino. Dichos comandos son:

reset --> pone el display a 0.
pause on --> para la cuenta del display.
pause off --> reanuda la cuenta del display.
go to X --> pone el valor X en el display.
speed +/- --> aumenta o decrementa la velocidad de la cuenta.
