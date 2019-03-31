Here's a brief preview of the representation: https://www.youtube.com/watch?v=XEjjA1ysRXM
You'll need the following libraries to run the app: OpenGL, glew, freeglut, SOIL, and SFML audio.
SFML audio will be automatically downloaded via makefile.

# Hotkeys:
Arrow_up and arrow_down to zoom in/out.
0 to show/hide orbits.
L to activate/deactivate the point light.
C to switch cameras.
1, 2, 3, 5, ..., 9 to switch between planets.
4 key to focus on a special object, in this case a cube that has a different color for each of its faces.
Arrow keys to move the current camera.
Esc to close the program.

-----------------------------------------------------------------------------------------
## Nome da dupla: Samir e Pedro

# Considerações sobre o trabalho:

	Só relembrando, é preciso instalar a biblioteca SFML audio para a rodar a música (comandos no makefile);

	De acordo com a especificação do trabalho:
		Câmera:
			Câmera na posição logo atrás do objeto especial, olhando para o objeto em que ele orbita. Nesse modo, ao pressionar as setinhas arrow_up e arrow_down a câmera deve se aproximar do planeta ou se afastar (respectivamente)

	Essa câmera pode ser encontrada apertando a tecla 4, onde a camera fica fiza atrás ddo cubo (satélite especial), e aponta sempre para marte

# Comandos:

Tecla O: Habilita/Desabilita órbitas
Tecla L: Ativa/desativa iluminação
Tecla C: Muda a câmera
Teclas 1, 2, 3, ... 9: Tour entre os planetas
Setinhas: Movimentam a câmera atual
Esc: Fecha o programa
