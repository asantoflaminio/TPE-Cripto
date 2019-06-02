

// aca vamos a hacer el skeleton


// Construccion de image shares. Es decir distribucion de las sombras a partir de la imagen original.

void distribute(int k, int n, image_t* secret_image, image_t* watermark_image){

	/*
	Armar la matriz de la imagen secreta
	No se si lo que obtiene van a ser ints o que
	*/
	int secret[][]; //hay que ver como obtenemos el tamaño
					// ese tamaño tambien se lo mandamos en rows y cols a esta funcion
	obtainImageMatrix(secret_image, rows, cols, secret);

	/*
	construir matriz random A de m*k
	con m > 2 (k - 1) - 1
	diria de usar m =4 cuando k = 2
	y m = 6 si k = 4
	Aca habria que hacer un if de si k es tanto entonces m es tanto
	*/
	int a_matrix[m][k];
	randomFill(m,k, a_matrix);

	/* aca hay que calcular S projection usando A y operaciones matriciales.
	El tamaño de secret_projection creo que va a ser el mismo que de secret */
	int secret_projection[][];
	calculateProjection(m, k, a_matrix, rows, cols, secret_projection);

	//tamaño igual a secret y secret_projection
	// matriz r es la reste entre secret_proyeccion y secret
	int r_matrix[][];
	substract( rows de r, cols de r, secret, secret_projection, r_matrix);


	/*
	Aca viene lo de elegir n (siendo n el recibido como paramtro) vectores random Xj de dimension k*1.

	Habria que hacer los n vectores xj y llamar a randomfill en cada uno
	Supongo q las xj las guardaremos cada una en un vector
	*/
	for(...){
		// xj
		randomFill(k, 1, xj);
		//guardar xj en vector
	}


	/*
	Aca hay que calcular las matrices v que surgen de hacer v = A*Xj
	Tambien vamos a tener n cantidad de matrices v.
	Supongo q las vj las guardaremos cada una en un vector
	*/
	for(...){
		//vj
		multiply(m,k, k, 1, a_matrix, xj, vj);
		// guardar vj en vector
	}

	/* 
	construccion de matrices G. Tenemos j matrices siendo j la cantidad de xj ???
	ahora construimos m/k techo valores dentro de esas amtrices
	ver bien la formula del paper. Los valores de I se obtienen de la matriz R

	*/
	for(...){
		//Matrices G
		//Esto puede q sea medio quilombo
	}

	/* 
	ahora usando la watermark calculamos Rw = W - secret_projection
	Para eso primero tenemos que leer W
	rw tendria mismo tamaño que ry secret
	*/
	int watermark[][]; //mismo tema q con secret de como obtenemos el tamaño
	obtainImageMatrix(watermark_image, rows, cols, watermark);
	int rw_matrix[][]
	substract( rows de r, cols de r, secret, secret_projection, r_matrix);


	/*
		Ahora viene lo de hacer las sombras
		Usamos concatenacion (lo tenemos hecho) de vj y Gj para obtener cada Shj
		nuevamente tendria vector para almacenar todos los shj
	*/
	for(...){
		int shj
		concat (m /*(creo q son m filas)*/, 1, columnas de gj, vj, gj, shj);
		//agrego al vector
	}


	// en el paper dice Rw queda publica ??

	/*

	///////////////////////////////////////////////////////////////
	EL TEMA ES ACA VIENE LO DE LA O LAS PORTADORAS Y ESTEGANOGRAFIA 
	///////////////////////////////////////////////////////////////

	sera ahcer un write image de las sombras encriptadas con esteganografia
	*/

}


void recover(int k, int n, image_t* output_image, image_t* watermark_image){

	/*

	///////////////////////////////////////////////////////////////
	EL TEMA ES ACA VIENE LO DE LA O LAS PORTADORAS Y ESTEGANOGRAFIA 
	///////////////////////////////////////////////////////////////

	sera ahcer un read image de las portadoras con sombras encriptadas con esteganografia

	y elegimos k sombras, supongamos las primeras k 
	*/

	//Tenemos sh1, sh2,..., shk

	/*
	Obtengo la matriz B que se obtiene concatenando las primeras columnas de todas las shadows.
	rows va a ser igual a las filas de las shadows
	*/
	int B[rows][cantidad de shadows];
	for(...){
		vamos concatenando a B todas las primeras columnas de las shadows
	}


	/*
	ahroa vamos a obtener secret_projection que surge de poryeccion de B
	*/
	int secret_projection[rows][rows]; //no estoy seguro de si el tamaño es igual a rows de B
	calculateProjection(rows, cantida de shadows, b_matrix], int rows, int cols, secret_projection);


	/* ahora construimos matrices Gj que es
		cada sombra excepto la primer columna */
	for(...){
		Gj
		copiamos a Gj excepto columna 0
	}

	/*
	ahora construimos R

	es medio quilombo. se hace  apartir de las matrices G
	hacemos gauss jordan con 

	Matriz de dos columnas, la primera llena de 1 y la segunda empieza en 1 hsta k.
	Mutliplicadoa por los I que quiero saber.
	eso igual a G(x,y) cada una (ver paper es quilombo)
	del resultado de gauss jordan sacamos como vamos llenando R
	*/
	for(...){
		//creo q conviene ir por cada fila voy a tener m/k resultados de gauss.
		// hay que ver bien q le mando a gauss
		//de esos resultados lleno cada uno de los valores de la fila 
	}


	/* 
	una vez que tenemos R solo sumamos secret_projection + R y nos queda S
	para la Watermark hacemos secret_proejction + Rw q era publica ??
	*/

	obtainRw ???

	int secret[][]
	add(rows, rows, secret_projection, r_matrix, secret);
	int watermark[][]
	add(rows, rows, secret_projection, rw_matrix, watermark);
}





/* obtiene la matriz de la imagen secreta y la guarda en answer.
	recibe una instancia de la estructura que resulta de la lectura de la imagen */
void obtainImageMatrix(image_t* image, int rows, int cols, int answer[rows][cols]) {

}

/*llena con valores random */
/* diria de llenar entre 0 y 9, igual le pregunto a Ana */
void randomFill(int rows, int cols, int answer[rows][cols]) {

}

/* calcula la proyeccion usando la matrix a

*/
void calculateProjection(int a_rows, int a_cols, int a_matrix[rows][cols], int rows, int cols, int answer[rows][cols]) {
	/* en principio aca hay que llamar a las operaciones con matrices que ya tenemos hechos
	 tenemos que arreglar q la inversa devuelve con float y %251
	 y ver en q orden se hacen la multiplciciones
	La formula es answer = A * (A_trasp * A)^inversa * A_trasp
	*./
}