# Space Invaders en la placa de desarrollo MBED NXP LPC1768
## Proyecto final de la asignatura Sistemas Electrónicos Digitales
## Universidad Autónoma de Madrid
### Alan Alberto Cruz Garcia y Antonio David Guaman Salcedo
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

Una vez creada la lógica del videojuego se realizaron los métodos correspondientes, por ejemplo la configuración para el ADC y su lectura para el potenciómetro, la implementación del acelerómetro, la creación y eliminación de enemigos , entre otros.

### Implementaciones
***
Las funciones implementadas se describen a continuación

-Juego siguiendo la lógica de jugabilidad de Space Invaders original versión 10 enemigos.
-Pantalla de inicio/pausa, ganadora y perdedora a partir de imágenes.
-Dibujos de nave y enemigos hechos pixel por pixel.
-LED RGB en función de la vida del jugador (Verde 3 vidas, Amarillo 2 vidas, Magenta 1 vida, Rojo 0 vidas).

-Cambio del modo de posicion de la nave del jugador (jostick abajo):
--Uso del acelerómetro para la posición de la nave del jugador.
--Uso del potenciómetro para la posición de la nave del jugador.

-Pausa del juego y continuar en donde se dejó (jostick arriba).
-Reinicio de vidas y enemigos al perder o ganar.

### Problemas surgidos
***
En este apartado se enlisitan los retos más imporantes surgidos en el proyecto 

#### Implementación de imágenes
  Para este punto, y mejorar la productividad ahorrando tiemmpo y mostrando pantallas con muy buena estética en la pantalla se optó por     utilizar un convertidor de imágenes png (https://www.mischianti.org/2021/07/14/ssd1306-oled-display-draw-images-splash-and-animations-2/) a un array plano de 4096 bits, esto funciona de la siguiente manera:
  Dada una imagen, nosotros la ajustamos a la resolución de la pantalla (128 x 32) una vez ajustada, el conversor nos arroja el array de 512 bytes es decir: (8 bits por elemento) * (512 elementos) donde los 8 bits representan una fila en el eje X de la pantalla, por lo tanto, la pantalla está dividida en 4 paginas de 8 bits cada una, existiendo 128 filas en el eje X obtenemos los 512 elementos.
  4096 elementos de buffer = (4 páginas) * (8 filas de 1 bit) * (128 columnas) Donde cada bit representa un pixel.
  
#### Creación - eliminación de enemigos
  Para esta funcionalidad se creó un array de enteros de 3 dimensiones y 10 elementos, donde cada elemento representa un enemigo, la primera dimensión la coordenada en la pantalla inicial en X donde se dibujará el enemigo, el segundo la coordenada en la pantalla inicial en Y y el tercero si el enemigo está vivo o eliminado, donde al inicio todos están vivos con el valor (1) y cuando son eliminados este valor se actualiza a 0. En el juego se accede en repetidas ocasiones a este array para cambiar cada uno de los elementos ya sea para moverlos o eliminarlos/restaurarlos.
  
#### Bugs
  En el juego a la hora de implementar las funcionalidades programadas no funcionaban a la primera en algunas ocaciones, como lo fue el acelerómetro donde nos dimos cuenta que su llamado debe ser siempre desde la función main y no en otra función encapsulada, de igual manera tuvimos un problema para disparar cuando se disparaba bajo los enemigos, esto surgió porque habiamos creado mas enemigos de los que se mostraban, resolviéndose unicamente con cambiar el número de elementos iterados para cuando se accede al array de enemigos y a los elementos puestos en este.
  
### Conclusión
***
Finalmente podemos concluir que en este proyecto integramos todos los conocimientos adquiridos en el curso tanto teóricos como prácticos y específicamente haciendo uso de código realizado en practicas anteriores, lo que facilito la implementación final del videojuego. De igual forma la búsqueda de soluciones para todos los problemas que fueron surgiendo a lo largo del desarrollo. 


  


