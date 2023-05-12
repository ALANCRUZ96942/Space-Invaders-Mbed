# Space Invaders en la placa de desarrollos MBED NXP LPC1768
## Proyecto final de la asignatura Sistemas Electrónicos Digitales
### Universidad Autónoma de Madrid
***

Para ejecutar el proyeco, debe abrirse el archivo Proyecto_final.uvprojx el cual abre todo el proyecto. Estando ahí, se construye todo el proyecto con la tecla F7 o dando clic en el ícono de Build. Ya conectada la placa mediante USB se da clic en Debug → Start/Stop Debug Session y finalmente Debug → Run para iniciar el programa.

La primera pantalla que se muestra es la pantalla de inicio o pausa, a partir de esta, se inicia el juego llevando el jostick hacia arriba.
La nave del jugador puede ser controlada mediante dos modos:
  -Modo potenciómetro: la posición de la nave se controla mediante el primer potenciómetro de la placa.
  -Modo acelerómetro: la posición de la nave se controla mediante el movimiento de la placa hacia la derecha o hacia la izquierda.
Este modo se cambia llevando el jostick hacia abajo y para regresar al modo anterior, solamente se vuelve a realizar esta misma acción.


### Metodología
***
Para la elaboración del juego, lo primero que se ralizó fue separar en ficheros distintos la función main y un header que incluye los ficheros auxiliares, así como variables y funciones que se utilizan a lo largo del juego.
Posteriormente se decidió como iban a interactuar cada una de estas funciones y la actualización de pantalla, pues el funcionamiento del juego gira en torno a ello. Se pensó en orientarlo a una programación orientada a objetos, sin embargo debido a la naturaleza del proyecto y como se trabaja en lenguaje C, se trabajó en una programacion procedimental o estructurada, donde todo está basado en métodos que se llaman a lo largo de la función principal.

De esta manera, primeramente se configuró la interrupción el timmer 0 como bandera de refresco de pantalla a 125 ms, es decir cada 125 ms la pantalla se actualizará en función de las acciones en él; podriamos decir que esta es la tasa de refresco. Posteriormente se realizó la lógica del juego en la función main utilizando nombres de métodos que se declararían después, por ejemplo "read_peripherals()" que es el método que lee el jostick y actualiza variables con respecto a este.

Una vez creada la lógica del videojuego se realizaron los métodos correspondientes, por ejemplo la configuración para el ADC y su lectura para el potenciómetro, la implementación del acelerómetro, la creación y eliminación de enemigos

### Implementaciones
***

### Problemas surgidos
***
En este apartado se enlisitan los retos más imporantes surgidos en el proyecto 

  #### Creación - eliminación de enemigos
  #### Bugs
  #### 
  
### Conclusión
***
Finalmente podemos concluir que en este proyecto integramos todos los conocimientos adquiridos en el curso tanto teóricos como prácticos y específicamente haciendo uso de código realizado en practicas anteriores, lo que facilito la implementación final del videojuego. De igual forma la búsqueda de soluciones para todos los problemas que fueron surgiendo a lo largo del desarrollo. 


  


