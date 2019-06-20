# TPE-Cripto
Grupo 3 - Criptografía y Seguridad 2019 1C - ITBA
Ritorto, Bianca - 57082
Santoflaminio, Alejandro - 57042
Varela Ballesteros, Catalina - 56433


Para compilar:
En la carpeta principal del proyecto ejecutar el comando 'make'.

Luego se puede ejecutar el programa de la siguiente manera:

Para distribuir:
./ss -d -s <Imagen secreta> -m <Imagen marca> -k <Valor de k> -n <Valor de n> -dir <Directorio de shares>


Para recuperar:
./ss -r -s <Imagen secreta destino> -m <Imagen con RW> -k <Valor de k> -n <Valor de n> -dir <Directorio de shares>

Aclaracion: Al distribuir se generara la imagen que contiene la transformacion de la marca, la cual se llamara generated_watermark.bmp. 
			Este archivo es el que debe ser utilizado en la recuperacion.

------------------------------------------------------------------------------------------------------------------------------------------
Ejemplos de ejecucion serian:

./ss -d -s "./Archivos de Prueba-4-8/Secreto.bmp" -m "./Archivos de Prueba-4-8/Marca.bmp" -k 4 -n 8 -dir "./Archivos de Prueba-4-8/shares/"



./ss -d -s "recovered_secret.bmp" -m "generated_watermark.bmp" -k 4 -n 8 -dir "./Archivos de Prueba-4-8/shares/"
